#pragma once
#include <functional>
#include <memory>
#include <string>

#include "tg/client.h"

typedef std::function<void(std::shared_ptr<Tiko>, const char *)> onMsg;
class Tiko;

class OnMsg
{
public:
    virtual void operator()(std::shared_ptr<Tiko> client, const char *) = 0;
};

std::shared_ptr<OnMsg> get_routine(const std::string);
void set_routine(const std::string, std::shared_ptr<OnMsg>);
