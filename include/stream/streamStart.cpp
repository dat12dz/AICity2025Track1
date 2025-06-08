#pragma once
#include "../msquic/msquic.h"
#include "../globalVar.cpp"
#include <iostream>

QUIC_STATUS StreamHandler(HQUIC stream,void* context,QUIC_STREAM_EVENT* Event)
{
    switch (Event->Type)
    {
    case QUIC_STREAM_EVENT_RECEIVE:
        Log("Nhan duoc " << Event->RECEIVE.BufferCount << " bytes");
        break;
    case QUIC_STREAM_EVENT_SHUTDOWN_COMPLETE:
        Log("Stream da shutdown");
    default:
        break;
    }
    return QUIC_STATUS_SUCCESS;
}

void StreamSending(HQUIC& connection)
{
    sleep(3);
    Log("bắt đầu gửi")
    HQUIC stream = nullptr;
    QUIC_STATUS status = msQuic_Open_instance->StreamOpen(
    connection, // Kết nối đang hoạt động
    QUIC_STREAM_OPEN_FLAG_NONE,
    StreamHandler, // Callback để nhận phản hồi
    nullptr,
    &stream);
 
    if (QUIC_FAILED(status))
    {
        throw std::runtime_error("khong the khoi tao StreamOpen");
    }
    // start
    Log("started stream")
    status = msQuic_Open_instance->StreamStart(
    stream,
    QUIC_STREAM_START_FLAG_IMMEDIATE);
    if (QUIC_FAILED(status))
    {
        throw std::runtime_error("khogn the khoi tao StreamStart");
    }
    sleep(3);
    const char* message = "Xin chào, peer!";
    QUIC_BUFFER buffer;
    buffer.Length = (uint32_t)strlen(message);
    buffer.Buffer = (uint8_t*)message;

    status =  msQuic_Open_instance->StreamSend(
    stream,
    &buffer,
    1, // số lượng buffer
    QUIC_SEND_FLAG_FIN, // gửi xong và đóng stream
    nullptr); // không cần context
    Log("sended");
    if (QUIC_FAILED(status))
    {
        throw std::runtime_error("khogn the khoi tao StreamSend");
    }
    sleep(5);
}