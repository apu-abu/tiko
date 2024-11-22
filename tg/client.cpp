#include <td/telegram/td_json_client.h>
#include <nlohmann/json.hpp>
#include <iostream>

#include "tg/client.h"
#include "tg/routine.h"
#include "utils/assert.h"
#include "utils/logger.h"

using json = nlohmann::json;

Tiko::Tiko(std::string phone_number)
{
    if (phone_number.size() == 0)
    {
        PANIC("init tiko client with empty phone number...");
    }
    client_id_ = td_create_client_id();
    phone_number_ = phone_number;

    // start the client by sending request to it
    td_send(client_id_, R"({"@type":"getOption", "name":"version"})");
}

void Tiko::set_phone_number(std::string phone_number)
{
    phone_number_ = phone_number;
}

std::string Tiko::get_phone_number() const
{
    return phone_number_;
}

void Tiko::set_authorized(bool authorized)
{
    is_authorized_ = authorized;
}

void Tiko::set_logout(bool logout)
{
    is_logout_ = logout;
}

bool Tiko::is_logout()
{
    return is_logout_;
}

int Tiko::get_client_id()
{
    return client_id_;
}

bool Tiko::is_authorized()
{
    return is_authorized_;
}

void Tiko::send(const char *cmd)
{
    td_send(client_id_, cmd);
}

void Tiko::receive(const char *msg_cstr)
{
    json msg = json::parse(msg_cstr);
    ASSERT(msg["@client_id"] == client_id_, "receive msg's client id doesn't match client");
    if (msg.contains("@extra"))
    {
        response(msg_cstr);
    }
    else
    {
        update(msg_cstr);
    }
}

std::int64_t Tiko::set_respond(std::unique_ptr<Tiko::respond> rspd)
{
    static std::int64_t request_id;
    request_id++;
    callbacks_[request_id] = std::move(rspd);
    return request_id;
}

std::unique_ptr<Tiko::respond> Tiko::get_respond(std::int64_t reqid)
{
    auto it = callbacks_.find(reqid);
    if (it == callbacks_.end())
    {
        return nullptr;
    }
    auto rspd = std::move(it->second);
    callbacks_.erase(reqid);
    return rspd;
}

void Tiko::update(const char *msg_cstr)
{
    json msg = json::parse(msg_cstr);
    std::string type = msg["@type"];
    auto routine = get_routine(type);
    if (routine)
    {
        (*routine)(shared_from_this(), msg_cstr);
    }
}

void Tiko::response(const char *msg_cstr)
{
    json msg = json::parse(msg_cstr);
    std::int64_t reqid = msg["@extra"];
    auto rspd = get_respond(reqid);
    if (rspd == nullptr)
    {
        json log = {
            {"@client_id", client_id_},
            {"@requset_id", reqid},
            {"@msg", "no respond was found"}};
        logger::warn("\2phone\3{}\2key\3{}\2msg\3{}",
                     get_phone_number(), "no_respond", log.dump());
        return;
    }
    (*rspd)(msg_cstr);
}
