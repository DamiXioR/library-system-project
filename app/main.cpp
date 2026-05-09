#include <iostream>
#include <memory>

#include <Logger.hpp>
#include <ConsoleLogSink.hpp>
#include <EnvVarFetcher.hpp>

int main(int nArgs, char** args){
    EnvVar::EnvVarFetcher::loadOrThrow();
    LogSys::Logger logger;
    logger.addSink(std::make_unique<LogSys::ConsoleLogSink>(LogSys::ConsoleLogSink(LogSys::LogLevel::Debug, {LogSys::MAIN_CHANNEL})));
    logger.log(LogSys::LogLevel::Debug, LogSys::MAIN_CHANNEL, LogSys::LogMessage("Hello {}!", "World"));

    return 0;
}
