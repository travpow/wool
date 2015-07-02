
#include <iostream>
#include <cassert>
#include "WoErlangBridge.hpp"
#include "WoMessage.hpp"

using std::list;
using std::cout;
using std::endl;

WoErlangBridge* WoErlangBridge::instance_ = 0;

ErlNifFunc WoErlangBridge::nifFuncs[] = {
    {"initialize", 0, WoErlangBridge::initialize}
};

WoErlangBridge::WoErlangBridge() :
    env_(0)
{
    // A shared env for creating erlang terms.
    messageEnv_ = enif_alloc_env();    
}

WoErlangBridge::~WoErlangBridge()
{
    list<ErlNifTid>::iterator threadItr = instance_->threads_.begin();
    for (; threadItr != instance_->threads_.end(); ++threadItr)
    {
        enif_thread_join(*threadItr, NULL /* exit value */);
    }
    
    enif_free_env(messageEnv_);

    env_ = 0;
    messageEnv_ = 0;
}

int WoErlangBridge::load()
{
    if (WoErlangBridge::instance_)
    {
        cout << "Instance already allocated for WoErlangBridge bridge! Exiting." << endl;
        exit(1);
    }

    WoErlangBridge::instance_ = new WoErlangBridge;

    return true;
}

void WoErlangBridge::unload()
{
    delete WoErlangBridge::instance_;
}

ERL_NIF_TERM WoErlangBridge::initialize(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    assert(WoErlangBridge::instance_);

    if (argc != 1)
    {
        cout << "Incorrect number of argumenwo." << endl;
        exit(1);
    }

    ErlNifPid bridgePid;

    if (!enif_is_pid(env, argv[0]) ||
        !enif_get_local_pid(env, argv[0], &bridgePid))
    {
        cout << "Argument passed to initialize() was not a PID or conversion form ERL_TERM failed." << endl;
        exit(1);
    }

    instance_->setBridge(env, bridgePid);
    instance_->createThread(notifyClients);


    static const ERL_NIF_TERM atomOk = enif_make_atom(instance_->messageEnv_, "ok");
    return atomOk;
}

ERL_NIF_TERM WoErlangBridge::formatMessage(const WoMessage* message)
{
    static const ERL_NIF_TERM woMessageAtom = enif_make_atom(messageEnv_, "wo_message");
    ERL_NIF_TERM binaryTerm;

    unsigned char* binaryPtr = enif_make_new_binary(messageEnv_, message->size(), &binaryTerm);
    memcpy(binaryPtr, message->data(), message->size());

    return enif_make_tuple2(messageEnv_, woMessageAtom, binaryTerm);
}

void*  WoErlangBridge::notifyClients(void*)
{
    bool keepRunning = true;

    do
    {
        const WoMessage* message = instance_->pop();
        ERL_NIF_TERM term = instance_->formatMessage(message);
            
        if(!enif_send(env_, bridgePid_, messageEnv_, term))
        {
            cout << "Could not send message to bridge - process may no longer be alive." << endl;
            keepRunning = false;
        }

    } while (keepRunning);

    cout << "Client notification thread will exit." << endl;
}

void WoErlangBridge::setBridge(ErlNifEnv* env, ErlNifPid pid)
{
    env_ = env;
    bridgePid_ = pid;
}

void WoErlangBridge::createThread(VoidFunc callback)
{
    const char* const threadName = "bridgeThread";
    ErlNifTid tid;

    int result = enif_thread_create(const_cast<char*>(threadName), &tid, callback,
            NULL /* callback args */, NULL /* options */);

    if (result != 0)
    {
        cout << "Could not create bridge thread! errno was [" << result << ']' << endl; 
        return;
    }

    threads_.push_back(tid);
}

ERL_NIF_INIT(wo_bridge, WoErlangBridge::nifFuncs, WoErlangBridge::load, NULL, NULL, WoErlangBridge::unload);

