#ifndef BONSAI_LOG_H_
#define BONSAI_LOG_H_

#include <spdlog/spdlog.h>

#include "Export.h"

namespace Bonsai
{
    class BONSAI_EXPORT Log
    {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return CoreLogger; }

        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> CoreLogger;
        static std::shared_ptr<spdlog::logger> ClientLogger;
    };
}


// Core logging macros

#define BONSAI_CORE_TRACE(...) ::Bonsai::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define BONSAI_CORE_INFO(...)  ::Bonsai::Log::GetCoreLogger()->info(__VA_ARGS__)
#define BONSAI_CORE_WARN(...)  ::Bonsai::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define BONSAI_CORE_ERROR(...) ::Bonsai::Log::GetCoreLogger()->error(__VA_ARGS__)
#define BONSAI_CORE_FATAL(...) ::Bonsai::Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define BONSAI_TRACE(...) ::Bonsai::Log::GetClientLogger()->trace(__VA_ARGS__)
#define BONSAI_INFO(...)  ::Bonsai::Log::GetClientLogger()->info(__VA_ARGS__)
#define BONSAI_WARN(...)  ::Bonsai::Log::GetClientLogger()->warn(__VA_ARGS__)
#define BONSAI_ERROR(...) ::Bonsai::Log::GetClientLogger()->error(__VA_ARGS__)
#define BONSAI_FATAL(...) ::Bonsai::Log::GetClientLogger()->fatal(__VA_ARGS__)

#endif // BONSAI_LOG_H_
