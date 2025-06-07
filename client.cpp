#include <iostream>
#include "include/msquic/msquic.h"
#include "include/stream/streamStart.cpp"

#include "include/stream/connection.cpp"
#include "include/globalVar.cpp"

// QUIC_STATUS QUIC_API COnnectionHandler(HQUIC connection,void* context,QUIC_CONNECTION_EVENT* event)
// {
//      switch (event->Type) {
//         case QUIC_CONNECTION_EVENT_CONNECTED:
//             std::cout << "Connection established successfully!" << std::endl;
//             break;
//         case QUIC_CONNECTION_EVENT_SHUTDOWN_INITIATED_BY_TRANSPORT:
//             std::cout << "Connection shutdown by transport. Error: " << event->SHUTDOWN_INITIATED_BY_TRANSPORT.ErrorCode << std::endl;
//             break;
//         case QUIC_CONNECTION_EVENT_SHUTDOWN_INITIATED_BY_PEER:
//             std::cout << "Connection shutdown by peer. Error: " << event->SHUTDOWN_INITIATED_BY_PEER.ErrorCode << std::endl;
//             break;
//         case QUIC_CONNECTION_EVENT_SHUTDOWN_COMPLETE:
//             std::cout << "Connection shutdown complete" << std::endl;
//             break;
//         default:
//             break;
    
//     }
//     return QUIC_STATUS_SUCCESS;
// }
HQUIC OpenedConnection;
int main()
{
    appName = "client";
    if (QUIC_FAILED(MsQuicOpen2(&msQuic_Open_instance)))
    {
        throw std::runtime_error("Không thể open quic");
    }
    QUIC_REGISTRATION_CONFIG* RegConfig = new QUIC_REGISTRATION_CONFIG();
    RegConfig->ExecutionProfile = (QUIC_EXECUTION_PROFILE)0;
    
    if (QUIC_FAILED(msQuic_Open_instance->RegistrationOpen(RegConfig ,&Register_instance)))
    {
        throw std::runtime_error("Không thể Register quic");
    }
    if (QUIC_FAILED(msQuic_Open_instance->ConnectionOpen(Register_instance,connectionCallback,nullptr,&OpenedConnection)))
    {
        throw std::runtime_error("Open connection failed"); 
    }
    
    // Create configuration handle
    const char* Alpn = "sample";        
    QUIC_BUFFER buffer;
    buffer.Length = strlen(Alpn);
    buffer.Buffer = (uint8_t*)Alpn;
    const QUIC_SETTINGS* defaultSetting = new QUIC_SETTINGS();
    HQUIC outputConfig;
    if (QUIC_FAILED(msQuic_Open_instance->ConfigurationOpen(Register_instance ,&buffer,1,nullptr,0,nullptr,&outputConfig)))
    {
        throw std::runtime_error("failed to create output config");
    }

    // thêm credeintial vào config , thêm lớp bảo mật = 0
    QUIC_CREDENTIAL_CONFIG CredConfig = {};
    CredConfig.Type = QUIC_CREDENTIAL_TYPE_NONE;
    CredConfig.Flags = QUIC_CREDENTIAL_FLAG_CLIENT | QUIC_CREDENTIAL_FLAG_NO_CERTIFICATE_VALIDATION;

    if (QUIC_FAILED(msQuic_Open_instance->ConfigurationLoadCredential(outputConfig, &CredConfig))) {
        throw std::runtime_error("không thể load crienttail");
    }

    // Start Connection
    if (QUIC_FAILED(msQuic_Open_instance->ConnectionStart(OpenedConnection,outputConfig,QUIC_ADDRESS_FAMILY_UNSPEC,"127.0.0.1",6000)))
    {
        throw std::runtime_error("Failed to start connection"); 
    }
    HQUIC stream;
    StartStream(msQuic_Open_instance,OpenedConnection,stream);
    
    msQuic_Open_instance->RegistrationClose(Register_instance);
    MsQuicClose(msQuic_Open_instance);


};
