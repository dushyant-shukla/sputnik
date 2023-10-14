#pragma once

#include "logger.h"

//////// RENDERER LOGGING ////////
#define ENGINE_TRACE(...) sputnik::core::Logger::GetEngineLogger()->trace(__VA_ARGS__)
#define ENGINE_INFO(...) sputnik::core::Logger::GetEngineLogger()->info(__VA_ARGS__)

#ifdef SPUTNIK_DEBUG
#define ENGINE_DEBUG(...) sputnik::core::Logger::GetEngineLogger()->debug(__VA_ARGS__)
#else
#define ENGINE_DEBUG(...)
#endif

#define ENGINE_WARN(...) sputnik::core::Logger::GetEngineLogger()->warn(__VA_ARGS__)
#define ENGINE_ERROR(...) sputnik::core::Logger::GetEngineLogger()->error(__VA_ARGS__)
#define ENGINE_CRITICAL(...) sputnik::core::Logger::GetEngineLogger()->critical(__VA_ARGS__)

//////// SCENE LOGGING ////////
#define APPLICATION_TRACE(...) sputnik::core::Logger::GetApplicationLogger()->trace(__VA_ARGS__)
#define APPLICATION_INFO(...) sputnik::core::Logger::GetApplicationLogger()->info(__VA_ARGS__)
#define APPLICATION_DEBUG(...) sputnik::core::Logger::GetApplicationLogger()->debug(__VA_ARGS__)
#define APPLICATION_WARN(...) sputnik::core::Logger::GetApplicationLogger()->warn(__VA_ARGS__)
#define APPLICATION_ERROR(...) sputnik::core::Logger::GetApplicationLogger()->error(__VA_ARGS__)
#define APPLICATION_CRITICAL(...) sputnik::core::Logger::GetApplicationLogger()->critical(__VA_ARGS__)