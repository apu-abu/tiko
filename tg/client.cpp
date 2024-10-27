#include <td/telegram/td_json_client.h>
#include <nlohmann/json.hpp>

#include "client.h"
#include "utils/assert.h"

using json = nlohmann::json;

tiko::tiko()
{
    client_id_ = td_create_client_id();

    // command register
    convention_["updateAuthorizationState"] = onUpdateAuthorizationState(shared_from_this());

    // start the client by sending request to it
    td_send(client_id_, R"({"@type":"getOption", "name":"version"})");
}

void tiko::set_authorized(bool authorized)
{
    are_authorized_ = authorized;
}

void tiko::set_logout(bool logout)
{
    are_logout_ = logout;
}

int tiko::get_client_id()
{
    return client_id_;
}

bool tiko::are_authorized()
{
    return are_authorized_;
}

void tiko::send(const char *cmd)
{
    td_send(client_id_, cmd);
}

void tiko::receive(const char *msg_cstr)
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

std::int64_t tiko::set_respond(std::unique_ptr<tiko::respond> rspd)
{
    static std::int64_t request_id;
    request_id++;
    callbacks_[request_id] = std::move(rspd);
    return request_id;
}

std::unique_ptr<tiko::respond> tiko::get_respond(std::int64_t reqid)
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

void tiko::update(const char *msg_cstr)
{
    json msg = json::parse(msg_cstr);
    auto it = convention_.find(msg["@type"]);
    if (it != convention_.end())
    {
        it->second(msg_cstr);
    }
}

void tiko::response(const char *msg_cstr)
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
        std::cout << log.dump() << std::endl;
        return;
    }
    (*rspd)(msg_cstr);
}
