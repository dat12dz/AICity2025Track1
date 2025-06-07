#include <stdio.h>
#include "include/msquic/msquic.h"

const QUIC_API_TABLE* MsQuic;
HQUIC Registration;

QUIC_STATUS QUIC_API MyListenerCallback(HQUIC /*Listener*/,void* /*Context*/, QUIC_LISTENER_EVENT* Event) 
{
    if (Event->Type == QUIC_LISTENER_EVENT_NEW_CONNECTION) {
        printf(">> nhận kết nối mới!\n");

        return QUIC_STATUS_ABORTED;
    }
    return QUIC_STATUS_SUCCESS;
}
int main() {
    if (QUIC_FAILED(MsQuicOpen2(&MsQuic)))
    {
        printf("Fail to open quic");
        return 0;
    }

    // Register quic
    QUIC_REGISTRATION_CONFIG* regConf = new QUIC_REGISTRATION_CONFIG();
    regConf->AppName = NULL;
    regConf->ExecutionProfile = (QUIC_EXECUTION_PROFILE)0;
    if (QUIC_FAILED(MsQuic->RegistrationOpen(regConf,&Registration)))
    {
        printf("fail to register quic");
        return 0;
    }

    // Open a listener
 
    HQUIC Listener;
    
    if (QUIC_FAILED(MsQuic->ListenerOpen(Registration,MyListenerCallback,nullptr,&Listener)))
    {
        printf("fail to open listener");
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

   if (QUIC_FAILED(MsQuic->ListenerStart(Listener,&buffer,1,&addrInfo)))
   {
        printf("listener Start fail");
        return 0;
   };
    MsQuic->RegistrationClose(Registration);
    MsQuicClose(MsQuic);
}
