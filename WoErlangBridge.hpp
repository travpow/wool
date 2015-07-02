
#include <list>
#include "erl_nif.h"

typedef void* (*VoidFunc)(void*);

class WoMessage;

class WoErlangBridge
{
public:
    WoErlangBridge();
    ~WoErlangBridge();

    static int load();
    static void unload();
    static ERL_NIF_TERM initialize(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);

    /**
     * Notify clients of updates to the shared memory queue.
     */
    static void* notifyClients(void*);

    static ErlNifFunc nifFuncs[];

private:
    void setBridge(ErlNifEnv* env, ErlNifPid pid);
    void createThread(VoidFunc callback);
    ERL_NIF_TERM formatMessage(const WoMessage* message);

    // Members
    static WoErlangBridge* instance_;

    ErlNifEnv* env_;
    ErlNifEnv* messageEnv_; // shared env for ERL terms
    ErlNifPid bridgePid_;
    std::list<ErlNifTid> threads_;
};

