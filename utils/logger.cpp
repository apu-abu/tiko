#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>

#include "logger.h"

namespace logger
{
    void init_rotated_logger(std::string app_name)
    {
        auto logger = spdlog::daily_logger_mt(app_name, "logs/" + app_name + ".log", 1, 0);
        spdlog::set_default_logger(logger);
    }
};
