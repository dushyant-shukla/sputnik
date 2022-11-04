#include "pch.h"
#include "logger.h"

namespace sputnik::core
{
std::shared_ptr<spdlog::logger> Logger::s_engine_logger;
std::shared_ptr<spdlog::logger> Logger::s_application_Logger;

void Logger::Init()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");

    s_engine_logger = spdlog::stdout_color_mt("ENGINE");
    s_engine_logger->set_level(spdlog::level::trace);

    s_application_Logger = spdlog::stdout_color_mt("APPLICATION");
    s_application_Logger->set_level(spdlog::level::trace);
}
} // namespace sputnik::core