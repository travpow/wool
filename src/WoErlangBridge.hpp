
#include <list>
#include "erl_nif.h"
#include "WoShmQueue.hpp"

typedef void* (*VoidFunc)(void*);

class WoMessage;

class WoErlangBridge
{
public:
    WoErlangBridge();
    ~WoErlangBridge();

    static int load(ErlNifEnv*, void**, ERL_NIF_TERM);
    static void unload(ErlNifEnv*, void*);
    static ERL_NIF_TERM initialize(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
    static ERL_NIF_TERM process(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);

    /**
     * Notify clients of updates to the shared memory queue.
     */
    static void* notificationThread(void*);

    static ErlNifFunc nifFuncs[];

private:
    void setBridge(ErlNifEnv* env, ErlNifPid pid);
    void createThread(VoidFunc callback);
    void notifyClients();
    ERL_NIF_TERM formatMessage(const WoMessage& message);

    // Members
    static WoErlangBridge* instance_;

    ErlNifEnv* env_;
    ErlNifEnv* messageEnv_; // shared env for ERL terms
    ErlNifPid bridgePid_;
    std::list<ErlNifTid> threads_;
    WoShmQueue queue_;
};

