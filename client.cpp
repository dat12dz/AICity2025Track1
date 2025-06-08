#include <iostream>
#include "include/msquic/msquic.h"
#include "include/stream/streamStart.cpp"

#include "include/stream/connection.cpp"
#include "include/globalVar.cpp"
#include "include/stream/Security.cpp"

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
    HQUIC outputConfig;
    QUIC_BUFFER outputBuffer;
    // Create configuration handle
    CreateSecurityConfiguration(1,outputConfig,outputBuffer);

    // Start Connection
    if (QUIC_FAILED(msQuic_Open_instance->ConnectionStart(OpenedConnection,outputConfig,QUIC_ADDRESS_FAMILY_UNSPEC,"127.0.0.1",6000)))
    {
        throw std::runtime_error("Failed to start connection"); 
    }
   // HQUIC stream;
     StreamSending(OpenedConnection);
    
    msQuic_Open_instance->RegistrationClose(Register_instance);
    MsQuicClose(msQuic_Open_instance);

};
