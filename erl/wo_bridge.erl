
-module(wo_bridge).
-export([start/0]).
%% -behaviour(gen_server).
-on_load(load_nif/0).


load_nif() ->
    erlang:load_nif("./wobridge", []).

start() ->
    initialize(self()),
    receive
        X ->
            io:format("~p~n",[X])
    end.

initialize(_Pid) ->
    throw("NIF not loaded.").
