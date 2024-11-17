#include <map>
#include <ctime>

#include "schedule.h"

std::map<std::string, int64_t> COUNTDOWNS;

void set_countdown(const std::string phone_number, int64_t countdown)
{
    COUNTDOWNS[phone_number] = countdown;
}

bool countdown(const std::string phone_number, bool &exec)
{
    auto it = COUNTDOWNS.find(phone_number);
    if (it != COUNTDOWNS.end())
    {
        std::time_t now = std::time(0);
        exec = now > it->second;
        return true;
    }
    return false;
}
