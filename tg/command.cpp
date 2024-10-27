#include <iostream>

#include <nlohmann/json.hpp>
#include <td/telegram/td_json_client.h>

#include "command.h"
#include "client.h"
#include "utils/assert.h"

using json = nlohmann::json;

tiko::respond on_authorication_failed(std::int64_t client_id)
{
    return [id = client_id](const char *msg_cstr)
    {
        json msg = json::parse(msg_cstr);
        if (msg["@type"] == "error")
        {
            std::cout << "Error code: " << msg["code"] << " message: " << msg["message"] << std::endl;
            json data = {"@type", "getAuthorizationState"};
            td_send(id, data.dump().c_str());
        }
    };
}

void onUpdateAuthorizationState::operator()(const char *msg_cstr) const
{
    json msg = json::parse(msg_cstr);
    ASSERT(msg["@type"] == "updateAuthorizationState", "msg type should be updateAuthorizationState");
    auto authorization_state = msg["authorization_state"];

    if (authorization_state["@type"] == "authorizationStateWaitTdlibParameters")
    {
        json data = {
            {"@type", "setTdlibParameters"},
            {"database_directory", "storage"},
            {"use_message_database", true},
            {"use_secret_chats", true},
            {"api_id", 94575},
            {"api_hash", "a3406de8d171bb422bb6ddf3bbd800e2"},
            {"system_language_code", "en"},
            {"device_model", "linux"},
            {"application_version", "1.0"},
        };
        td_send(client_->get_client_id(), data.dump().c_str());
    }
    else if (authorization_state["@type"] == "authorizationStateWaitPhoneNumber")
    {
        std::cout << "wait your phone number..." << std::endl;
        std::string phone_number;
        std::cin >> phone_number;
        json data = {
            {"@type", "setAuthenticationPhoneNumber"},
            {"phone_number", phone_number},
            {"@extra",
             client_->set_respond(
                 std::make_unique<tiko::respond>(on_authorication_failed(client_->get_client_id())))},
        };
        td_send(client_->get_client_id(), data.dump().c_str());
    }
    else if (authorization_state["@type"] == "authorizationStateWaitEmailAddress")
    {
        std::cout << "wait your email address..." << std::endl;
        std::string email_address;
        std::cin >> email_address;
        json data = {
            {"@type", "setAuthenticationEmailAddress"},
            {"email_address", email_address},
            {"@extra",
             client_->set_respond(
                 std::make_unique<tiko::respond>(on_authorication_failed(client_->get_client_id())))},
        };
        td_send(client_->get_client_id(), data.dump().c_str());
    }
    else if (authorization_state["@type"] == "authorizationStateWaitEmailCode")
    {
        std::cout << "Enter email code..." << std::endl;
        std::string code;
        std::cin >> code;
        json data = {
            {"@type", "checkAuthenticationEmailCode"},
            {
                "code",
                {
                    {"@type", "emailAddressAuthenticationCode"},
                    {"code", code},
                },
            },
            {"@extra",
             client_->set_respond(
                 std::make_unique<tiko::respond>(on_authorication_failed(client_->get_client_id())))},
        };
        td_send(client_->get_client_id(), data.dump().c_str());
    }
    else if (authorization_state["@type"] == "authorizationStateWaitCode")
    {
        std::cout << "Enter (SMS) code..." << std::endl;
        std::string code;
        std::cin >> code;
        json data = {
            {"@type", "checkAuthenticationEmailCode"},
            {"code", code},
            {"@extra",
             client_->set_respond(
                 std::make_unique<tiko::respond>(on_authorication_failed(client_->get_client_id())))},
        };
        td_send(client_->get_client_id(), data.dump().c_str());
    }
    else if (authorization_state["@type"] == "authorizationStateWaitCode")
    {
        std::cout << "Register new account,\n enter first name..." << std::endl;
        std::string first_name;
        std::cin >> first_name;
        std::cout << "last name..." << std::endl;
        std::string last_name;
        std::cin >> last_name;
        json data = {
            {"@type", "registerUser"},
            {"first_name", first_name},
            {"last_name", last_name},
            {"@extra",
             client_->set_respond(
                 std::make_unique<tiko::respond>(on_authorication_failed(client_->get_client_id())))},
        };
        td_send(client_->get_client_id(), data.dump().c_str());
    }
    else if (authorization_state["@type"] == "authorizationStateWaitPassword")
    {
        std::cout << "Enter password..." << std::endl;
        std::string password;
        std::cin >> password;
        json data = {
            {"@type", "checkAuthenticationPassword"},
            {"password", password},
            {"@extra",
             client_->set_respond(
                 std::make_unique<tiko::respond>(on_authorication_failed(client_->get_client_id())))},
        };
        td_send(client_->get_client_id(), data.dump().c_str());
    }
    else if (authorization_state["@type"] == "authorizationStateReady")
    {
        client_->set_authorized(true);
    }
    else if (authorization_state["@type"] == "authorizationStateLoggingOut")
    {
        client_->set_authorized(false);
        client_->set_logout(true);
    }
    else if (authorization_state["@type"] == "authorizationStateClosing")
    {
        client_->set_authorized(false);
        client_->set_logout(true);
    }
    else if (authorization_state["@type"] == "authorizationStateClosed")
    {
        client_->set_authorized(false);
        client_->set_logout(true);
    }
    else
    {
        std::cout << "Unexpected authorization state" << std::endl;
    }
}
