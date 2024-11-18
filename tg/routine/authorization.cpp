#include <iostream>

#include <nlohmann/json.hpp>
#include <td/telegram/td_json_client.h>

#include "diverse/authorize.h"
#include "tg/client.h"
#include "tg/routine.h"
#include "utils/assert.h"
#include "utils/logger.h"

using json = nlohmann::json;

Tiko::respond on_authorication_failed(std::shared_ptr<Tiko> client)
{
    return [&](const char *msg_cstr)
    {
        json msg = json::parse(msg_cstr);
        if (msg["@type"] == "error")
        {
            logger::warn("phone\3{}\2key\3{}\2msg\3{}",
                         client->get_phone_number(), "authorication_failed", msg_cstr);
            json data = {"@type", "getAuthorizationState"};
            // to panic Error code: 400 message: "PHONE_NUMBER_BANNED"
            if (msg["code"] == 400 && msg["message"] == "PHONE_NUMBER_BANNED")
            {
                data = {
                    {"@type", "getLocalizationTargetInfo"},
                    {"only_local", true},
                };
                td_send(client->get_client_id(), data.dump().c_str());
                logger::warn("phone\3{}\2key\3{}\2msg\3{}",
                             client->get_phone_number(), "account_is_banned", msg_cstr);
                client->set_logout(true);
                return;
            }
            td_send(client->get_client_id(), data.dump().c_str());
        }
    };
}

class OnUpdateAuthorizationState : public OnMsg
{
public:
    OnUpdateAuthorizationState() = default;
    void operator()(std::shared_ptr<Tiko> client, const char *msg_cstr)
    {
        json msg = json::parse(msg_cstr);
        ASSERT(msg["@type"] == "updateAuthorizationState", "msg type should be updateAuthorizationState");
        auto authorization_state = msg["authorization_state"];

        if (authorization_state["@type"] == "authorizationStateWaitTdlibParameters")
        {
            json data = {
                {"@type", "setTdlibParameters"},
                {"database_directory", "storage/" + client->get_phone_number()},
                {"use_message_database", true},
                {"use_secret_chats", true},
                {"api_id", 25578803},
                {"api_hash", "c21f19559919c7a65702295187f8fe83"},
                {"system_language_code", "en"},
                {"device_model", "linux"},
                {"application_version", "1.0"},
            };
            td_send(client->get_client_id(), data.dump().c_str());
        }
        else if (authorization_state["@type"] == "authorizationStateWaitPhoneNumber")
        {
            json data = {
                {"@type", "setAuthenticationPhoneNumber"},
                {"phone_number", client->get_phone_number()},
                {"@extra",
                 client->set_respond(
                     std::make_unique<Tiko::respond>(on_authorication_failed(client)))},
            };
            td_send(client->get_client_id(), data.dump().c_str());
        }
        else if (authorization_state["@type"] == "authorizationStateWaitEmailAddress")
        {
            logger::warn("phone\3{}\2key\3{}\2msg\3{}",
                         client->get_phone_number(), "wait_email_address", msg_cstr);
            PANIC("account shouldn't use email address");
            std::string email_address;
            std::cin >> email_address;
            json data = {
                {"@type", "setAuthenticationEmailAddress"},
                {"email_address", email_address},
                {"@extra",
                 client->set_respond(
                     std::make_unique<Tiko::respond>(on_authorication_failed(client)))},
            };
            td_send(client->get_client_id(), data.dump().c_str());
        }
        else if (authorization_state["@type"] == "authorizationStateWaitEmailCode")
        {
            logger::warn("phone\3{}\2key\3{}\2msg\3{}",
                         client->get_phone_number(), "wait_email_code", msg_cstr);
            PANIC("account shouldn't use email code");
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
                 client->set_respond(
                     std::make_unique<Tiko::respond>(on_authorication_failed(client)))},
            };
            td_send(client->get_client_id(), data.dump().c_str());
        }
        else if (authorization_state["@type"] == "authorizationStateWaitCode")
        {
            std::string code;
            if (!get_authorize_code(client->get_phone_number(), std::time(0), code))
            {
                json data = {
                    {"@type", "getLocalizationTargetInfo"},
                    {"only_local", true},
                };
                td_send(client->get_client_id(), data.dump().c_str());
                logger::warn("phone\3{}\2key\3{}\2msg\3{}",
                             client->get_phone_number(), "fetch_sms_code_failed", msg_cstr);
                client->set_logout(true);
                return;
            };
            json data = {
                {"@type", "checkAuthenticationCode"},
                {"code", code},
                {"@extra",
                 client->set_respond(
                     std::make_unique<Tiko::respond>(on_authorication_failed(client)))},
            };
            td_send(client->get_client_id(), data.dump().c_str());
            logger::info("phone\3{}\2key\3{}\2msg\3{}",
                         client->get_phone_number(), "send_sms_code", "");
        }
        else if (authorization_state["@type"] == "authorizationStateWaitRegistration")
        {
            logger::warn("phone\3{}\2key\3{}\2msg\3{}",
                         client->get_phone_number(), "register_accout", msg_cstr);
            PANIC("the account can't register in process.");
            std::cout << "first name.." << std::endl;
            std::string first_name;
            std::cin >> first_name;
            std::cout << "last name.." << std::endl;
            std::string last_name;
            std::cin >> last_name;
            json data = {
                {"@type", "registerUser"},
                {"first_name", first_name},
                {"last_name", last_name},
                {"@extra",
                 client->set_respond(
                     std::make_unique<Tiko::respond>(on_authorication_failed(client)))},
            };
            td_send(client->get_client_id(), data.dump().c_str());
        }
        else if (authorization_state["@type"] == "authorizationStateWaitPassword")
        {
            std::string password;
            get_password(client->get_phone_number(), password);
            json data = {
                {"@type", "checkAuthenticationPassword"},
                {"password", password},
                {"@extra",
                 client->set_respond(
                     std::make_unique<Tiko::respond>(on_authorication_failed(client)))},
            };
            td_send(client->get_client_id(), data.dump().c_str());
            logger::info("phone\3{}\2key\3{}\2msg\3{}",
                         client->get_phone_number(), "send_password", "");
        }
        else if (authorization_state["@type"] == "authorizationStateReady")
        {
            logger::info("phone\3{}\2key\3{}\2msg\3{}",
                         client->get_phone_number(), "login_success", "");
            client->set_authorized(true);
            on_login(client->get_phone_number());
        }
        else if (authorization_state["@type"] == "authorizationStateLoggingOut")
        {
            client->set_authorized(false);
            logger::warn("phone\3{}\2key\3{}\2msg\3{}",
                         client->get_phone_number(), "rcv_logout_msg", msg_cstr);
            client->set_logout(true);
        }
        else if (authorization_state["@type"] == "authorizationStateClosing")
        {
            client->set_authorized(false);
            logger::warn("phone\3{}\2key\3{}\2msg\3{}",
                         client->get_phone_number(), "rcv_closing_msg", msg_cstr);
            client->set_logout(true);
        }
        else if (authorization_state["@type"] == "authorizationStateClosed")
        {
            client->set_authorized(false);
            logger::warn("phone\3{}\2key\3{}\2msg\3{}",
                         client->get_phone_number(), "rcv_closed_msg", msg_cstr);
            client->set_logout(true);
        }
        else
        {
            logger::warn("phone\3{}\2key\3{}\2msg\3{}",
                         client->get_phone_number(), "rcv_upexpected_authorization_msg", msg_cstr);
        }
    }
};

void init() __attribute__((constructor));

void init()
{
    auto routine = std::make_shared<OnUpdateAuthorizationState>();
    set_routine("updateAuthorizationState", std::move(routine));
}
