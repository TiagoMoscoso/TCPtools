#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;

class Logger
{
private:
    static string getCurrentDateTime() {
        auto now = chrono::system_clock::now();
        time_t now_time_t = chrono::system_clock::to_time_t(now); 
        stringstream ss;
        ss << put_time(localtime(&now_time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

public:
    static void LogInfo(const string& message)
    {
        cout << "\033[37m[" << getCurrentDateTime() << "] " << message << "\033[0m" << endl;
    }

    static void LogDebug(const string& message)
    {
        cout << "\033[34m[" << getCurrentDateTime() << "] " << message << "\033[0m" << endl;
    }

    static void LogDanger(const string& message)
    {
        cout << "\033[31m[" << getCurrentDateTime() << "] " << message << "\033[0m" << endl;
    }
};

#endif
