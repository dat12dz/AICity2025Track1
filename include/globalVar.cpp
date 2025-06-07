#pragma once
#include "./msquic/msquic.h"
#include <string>

#define Log(x) std::cout << appName << ":    " << x
const QUIC_API_TABLE* msQuic_Open_instance; 
HQUIC Register_instance;
const char* appName;
