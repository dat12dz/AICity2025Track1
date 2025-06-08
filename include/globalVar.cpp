#pragma once
#include "./msquic/msquic.h"
#include <string>
#include <iostream>

#define Log(x) std::cout << appName << ":    " << x << '\n';
const QUIC_API_TABLE* msQuic_Open_instance; 
HQUIC Register_instance;
const char* appName;

#define CheckQUICStatus(x,errorlog) QUIC_STATUS s = x;Log("QUIC_STATUS = " << s); if (QUIC_FAILED(s)){throw std::runtime_error(errorlog);}