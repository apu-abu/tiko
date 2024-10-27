#pragma once
#include <functional>
#include <iostream>
#include <memory>
#include <map>

#include "command.h"

class tiko : public std::enable_shared_from_this<tiko>
{
public:
    typedef std::function<void(const char *rsp)> respond;
    tiko();
    int get_client_id();
    bool are_authorized();
    void set_authorized(bool authorized);
    void set_logout(bool logout);
    void send(const char *cmd);
    void receive(const char *update);
    std::int64_t set_respond(std::unique_ptr<respond> rspd);

private:
    void update(const char *msg);
    void response(const char *msg);
    std::unique_ptr<respond> get_respond(std::int64_t);
    int client_id_;
    bool are_authorized_{false};
    bool are_logout_{false};
    std::map<std::int64_t, std::unique_ptr<respond>> callbacks_;
    std::map<std::string, onMsg> convention_;
};
