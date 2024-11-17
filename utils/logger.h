#pragma once
#include <chrono>
#include <spdlog/spdlog.h>

#define TLOG_LEVEL_TRACE 0
#define TLOG_LEVEL_DEBUG 1
#define TLOG_LEVEL_INFO 2
#define TLOG_LEVEL_WARN 3
#define TLOG_LEVEL_ERROR 4
#define TLOG_LEVEL_CRITICAL 5
#define TLOG_LEVEL_OFF 6

namespace logger
{
    enum level_enum : int
    {
        trace_level = TLOG_LEVEL_TRACE,
        debug_level = TLOG_LEVEL_DEBUG,
        info_level = TLOG_LEVEL_INFO,
        warn_level = TLOG_LEVEL_WARN,
        err_level = TLOG_LEVEL_ERROR,
        critical_level = TLOG_LEVEL_CRITICAL,
        off_level = TLOG_LEVEL_OFF,
        n_levels
    };

    template <typename Rep, typename Period>
    inline void flush_every(std::chrono::duration<Rep, Period> interval)
    {
        spdlog::flush_every(interval);
    }

    inline void set_log_level(level_enum log_level)
    {
        spdlog::default_logger()->set_level(static_cast<spdlog::level::level_enum>(log_level));
    }

    template <typename... Args>
    inline void trace(spdlog::format_string_t<Args...> fmt, Args &&...args)
    {
        spdlog::default_logger()->trace(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline void debug(spdlog::format_string_t<Args...> fmt, Args &&...args)
    {
        spdlog::default_logger()->debug(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline void info(spdlog::format_string_t<Args...> fmt, Args &&...args)
    {
        spdlog::default_logger()->info(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline void warn(spdlog::format_string_t<Args...> fmt, Args &&...args)
    {
        spdlog::default_logger()->warn(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline void error(spdlog::format_string_t<Args...> fmt, Args &&...args)
    {
        spdlog::default_logger()->error(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline void critical(spdlog::format_string_t<Args...> fmt, Args &&...args)
    {
        spdlog::default_logger()->critical(fmt, std::forward<Args>(args)...);
    }

    void init_rotated_logger(std::string app_name);
};
