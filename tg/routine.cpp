#include <map>
#include "tg/routine.h"

namespace
{
    std::map<std::string, std::shared_ptr<OnMsg>> ROUTINES;
}

std::shared_ptr<OnMsg> get_routine(const std::string type)
{
    auto it = ROUTINES.find(type);
    if (it == ROUTINES.end())
    {
        return std::shared_ptr<OnMsg>(nullptr);
    }
    return it->second;
}

void set_routine(const std::string type, std::shared_ptr<OnMsg> routine)
{
    ROUTINES[type] = routine;
}
