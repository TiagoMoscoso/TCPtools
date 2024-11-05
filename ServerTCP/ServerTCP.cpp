// ServerTCP.h
#ifndef SERVERTCP_H
#define SERVERTCP_H

#include "Logger.h"

class ServerTCP
{

    public:
        void start() 
        { 
            Logger::LogInfo("Server service started");

        }
};

#endif
