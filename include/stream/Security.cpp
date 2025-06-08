#include <iostream>
#include "../msquic/msquic.h"
#include "../globalVar.cpp"
void CreateSecurityConfiguration(bool client,HQUIC& outputConfig,QUIC_BUFFER& buffer)
{
  
    const char* Alpn = "sample";        
    buffer.Length = strlen(Alpn);
    buffer.Buffer = (uint8_t*)Alpn;
    QUIC_SETTINGS defaultSetting = QUIC_SETTINGS();
    defaultSetting.PeerBidiStreamCount = 100;  // Tăng luồng hai chiều
    defaultSetting.PeerUnidiStreamCount = 100; // Tăng luồng một chiều
    defaultSetting.IsSet.PeerBidiStreamCount = TRUE;
    defaultSetting.IsSet.PeerUnidiStreamCount = TRUE;
    if (QUIC_FAILED(msQuic_Open_instance->ConfigurationOpen(
        Register_instance ,
        &buffer,
        1,
        &defaultSetting,
        sizeof(defaultSetting),
        nullptr,
        &outputConfig)))
    {
        throw std::runtime_error("failed to create output config");
    }
    // thêm credeintial vào config , thêm lớp bảo mật = 0
    if (client)
    {
        QUIC_CREDENTIAL_CONFIG CredConfig = {};
        CredConfig.Type = QUIC_CREDENTIAL_TYPE_NONE;
        CredConfig.Flags = QUIC_CREDENTIAL_FLAG_CLIENT | QUIC_CREDENTIAL_FLAG_NO_CERTIFICATE_VALIDATION;
        if (QUIC_FAILED(msQuic_Open_instance->ConfigurationLoadCredential(outputConfig, &CredConfig))) {
            throw std::runtime_error("không thể load crienttail");
        }

    }
    else
    {
        QUIC_CREDENTIAL_CONFIG CredConfig = {};
        CredConfig.Type = QUIC_CREDENTIAL_TYPE_CERTIFICATE_FILE;
        
  
        QUIC_CERTIFICATE_FILE CertFile = {};
        CertFile.CertificateFile = "server.crt";   
        CertFile.PrivateKeyFile = "server.key";      
        
        CredConfig.CertificateFile = &CertFile;
        CheckQUICStatus(msQuic_Open_instance->ConfigurationLoadCredential(outputConfig, &CredConfig),"Khong the tiep tuc")
   
    }
}