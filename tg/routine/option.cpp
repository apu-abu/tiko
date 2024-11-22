#include <iostream>
#include <sstream>

#include <nlohmann/json.hpp>
#include <td/telegram/td_json_client.h>

#include "dao/pet.h"
#include "tg/client.h"
#include "tg/routine.h"
#include "utils/assert.h"
#include "utils/logger.h"

using json = nlohmann::json;

class OnUpdateOption : public OnMsg
{
public:
    OnUpdateOption() = default;
    void operator()(std::shared_ptr<Tiko> client, const char *msg_cstr)
    {

        json data = json::parse(msg_cstr);
        if (data["name"] == "my_id")
        {
            std::istringstream uidss(data["value"]["value"].get<std::string>());
            std::int64_t uid;
            if (!(uidss >> uid))
            {
                PANIC(client->get_phone_number() + " parse uid failed: " + msg_cstr);
            }
            std::string phone = client->get_phone_number();
        }
    }
};

void init_option() __attribute__((constructor));

void init_option()
{
    auto option = std::make_shared<OnUpdateOption>();
    set_routine("updateOption", std::move(option));
}