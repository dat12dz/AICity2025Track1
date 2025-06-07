#pragma once
#include "../msquic/msquic.h"
#include "../globalVar.cpp"
#include <iostream>
QUIC_STATUS StreamHandler(HQUIC stream,void* context,QUIC_STREAM_EVENT* Event)
{
    switch (Event->Type)
    {
    case QUIC_STREAM_EVENT_RECEIVE:
        Log("Nhan duoc " << Event->RECEIVE.BufferCount << " bytes" << '\n');
        break;
    case QUIC_STREAM_EVENT_SHUTDOWN_COMPLETE:
        Log("Stream da shutdown" << '\n');
    default:
        break;
    }
    return QUIC_STATUS_SUCCESS;
}

void StartStream(const QUIC_API_TABLE* msQuic_Open_instance,const HQUIC& OpenedConnection,_Out_ HQUIC& outputStream)
{
    HQUIC StreamHandle;
    if (QUIC_FAILED(msQuic_Open_instance->StreamOpen(OpenedConnection,QUIC_STREAM_OPEN_FLAG_NONE,StreamHandler,nullptr,&outputStream)))
    {
        throw std::runtime_error("Fail to open stream");
    }
    if (QUIC_FAILED(msQuic_Open_instance->StreamStart(outputStream,QUIC_STREAM_START_FLAG_NONE)))
    {
        throw std::runtime_error("Fail to open stream");
    }
    QUIC_BUFFER quicBuffer;
    const char* t = "toi";
    quicBuffer.Buffer = (uint8_t*)t;
    quicBuffer.Length = 3;
    msQuic_Open_instance->StreamSend(outputStream,&quicBuffer,1,QUIC_SEND_FLAG_NONE,nullptr);
}