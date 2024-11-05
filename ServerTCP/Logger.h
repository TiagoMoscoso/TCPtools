#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fmt/core.h>

using namespace std;
using namespace fmt;

static class Logger 
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
            print("[{}] {}\n", getCurrentDateTime(), message, "\033[0m");
        }
        static void LogDebug(const string& message)
        {
            print("{}[{}] {}{} \n", "\033[34m", getCurrentDateTime(), message, "\033[0m");
        }
        static void LogDanger(const string& message)
        {
            print("{}[{}] {}{} \n", "\033[31m", getCurrentDateTime(), message, "\033[0m");
        }

};