#include <iostream>

#include <nlohmann/json.hpp>
#include <td/telegram/td_json_client.h>

#include "tg/client.h"
#include "tg/routine.h"
#include "utils/assert.h"
#include "utils/logger.h"

using json = nlohmann::json;

class OnUser : public OnMsg
{
public:
    OnUser() = default;
    void operator()(std::shared_ptr<Tiko> client, const char *msg_cstr)
    {
        json data = json::parse(msg_cstr);
        std::int64_t uid = data["id"];
        std::int64_t pet = client->get_uid();
    }
};

void init_user() __attribute__((constructor));

void init_user()
{
    auto user = std::make_shared<OnUser>();
    set_routine("user", std::move(user));
}