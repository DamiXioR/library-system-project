#include <iostream>
#include <memory>

#include <Logger.hpp>
#include <ConsoleLogSink.hpp>

LogSys::Logger logger;

int main(int nArgs, char** args){
    std::cout << "START\n";
    logger.addSink(std::make_unique<LogSys::ConsoleLogSink>(LogSys::ConsoleLogSink(LogSys::LogLevel::Debug, {"Book", "DataBase"})));
    logger.log(LogSys::LogLevel::Info, "Book", LogSys::LogMessage("Hello Logs! {}", "Hello Book!"));
    logger.log(LogSys::LogLevel::Info, "Nothing", LogSys::LogMessage("Hello Logs! {}", "Hello Nothing!"));
    logger.log(LogSys::LogLevel::Debug, "DataBase", LogSys::LogMessage("Hello Logs! {}", "Hello DataBase!"));

    std::cout << "Break\n";

    logger.addSink(std::make_unique<LogSys::ConsoleLogSink>(LogSys::ConsoleLogSink(LogSys::LogLevel::Debug, {"Main"})));
    logger.log(LogSys::LogLevel::Info, "Book", LogSys::LogMessage("Hello Logs! {}", "Hello All!"));
    logger.log(LogSys::LogLevel::Info, "Nothing", LogSys::LogMessage("Hello Logs! {}", "Hello All!"));
    logger.log(LogSys::LogLevel::Debug, "Clothing", LogSys::LogMessage("Hello Logs! {}", "Hello All!"));

    std::cout << "STOP\n";
    return 0;
}
