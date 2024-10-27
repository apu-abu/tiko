#pragma once
#include <functional>
#include <string>

typedef std::function<void(const char *)> onMsg;
class tiko;

class onUpdateAuthorizationState
{
public:
    onUpdateAuthorizationState(std::shared_ptr<tiko> client) : client_(client) {};
    void operator()(const char *msg) const;

private:
    std::shared_ptr<tiko> client_;
};
