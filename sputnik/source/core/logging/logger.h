#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>

namespace sputnik::core
{
class Logger
{
public:
    Logger(){};

    ~Logger() {}

    static void Init();

    static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return s_engine_logger; }

    static std::shared_ptr<spdlog::logger>& GetApplicationLogger() { return s_application_Logger; }

private:
    static std::shared_ptr<spdlog::logger> s_engine_logger;
    static std::shared_ptr<spdlog::logger> s_application_Logger;
};
} // namespace sputnik::core