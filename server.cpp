#include <iostream>
#include "include/msquic/msquic.h"
#include "include/globalVar.cpp"

QUIC_STATUS QUIC_API MyListenerCallback(HQUIC /*Listener*/,void* /*Context*/, QUIC_LISTENER_EVENT* Event) 
{
    if (Event->Type == QUIC_LISTENER_EVENT_NEW_CONNECTION) {
        Log(">> nhận kết nối mới!\n");
        HQUIC connection = Event->NEW_CONNECTION.Connection;
        
    }
    return QUIC_STATUS_SUCCESS;
}
int main() {
    appName = "server";
    if (QUIC_FAILED(MsQuicOpen2(&msQuic_Open_instance)))
    {
        Log("Fail to open quic");
        return 0;
    }

    // Register quic
    QUIC_REGISTRATION_CONFIG* regConf = new QUIC_REGISTRATION_CONFIG();
    regConf->AppName = NULL;
    regConf->ExecutionProfile = (QUIC_EXECUTION_PROFILE)0;
    if (QUIC_FAILED(msQuic_Open_instance->RegistrationOpen(regConf,&Register_instance)))
    {
        Log("fail to register quic");
        return 0;
    }

    // Open a listener
 
    HQUIC Listener;
    
    if (QUIC_FAILED(msQuic_Open_instance->ListenerOpen(Register_instance,MyListenerCallback,nullptr,&Listener)))
    {
        Log("fail to open listener");
        return 0;
    };

    QUIC_ADDR addrInfo;
    memset(&addrInfo, 0, sizeof(addrInfo));
    addrInfo.Ipv4.sin_family = AF_INET;
    addrInfo.Ipv4.sin_port = htons(6000);
    addrInfo.Ipv4.sin_addr.s_addr = inet_addr("127.0.0.1");


    // "sample hay http"
    const char* Alpn = "sample";  
    QUIC_BUFFER buffer;
    buffer.Length = strlen(Alpn);
    buffer.Buffer = (uint8_t*)Alpn;

   if (QUIC_FAILED(msQuic_Open_instance->ListenerStart(Listener,&buffer,1,&addrInfo)))
   {
        Log("listener Start fail");
        return 0;
   };
    msQuic_Open_instance->RegistrationClose(Register_instance);
    MsQuicClose(msQuic_Open_instance);
}
