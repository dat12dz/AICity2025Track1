#include "../msquic/msquic.h"
#include <iostream>
#include "../globalVar.cpp"
#include "./streamStart.cpp"
QUIC_STATUS connectionCallback(HQUIC Connection,void* context,QUIC_CONNECTION_EVENT* event)
{
    switch (event->Type)
    {
    case QUIC_CONNECTION_EVENT_CONNECTED:
        Log("connection has been establish");
        break;
    case QUIC_CONNECTION_EVENT_PEER_STREAM_STARTED:
        Log("peer đã mở 1 stream mới");
        msQuic_Open_instance->SetCallbackHandler(event->PEER_STREAM_STARTED.Stream,(void*)StreamHandler,nullptr);
    default:
        break;
    }
    return QUIC_STATUS_SUCCESS;
}