#include <filesystem>
#include <iostream>
#include <memory>

#include <td/telegram/td_json_client.h>
#include <nlohmann/json.hpp>

#include "tg/client.h"
#include "tg/routine.h"
#include "utils/logger.h"
#include "utils/database.h"

// Basic example of TDLib JSON interface usage.
// Native interface should be preferred instead in C++, so here is only an example of
// the main event cycle, which should be essentially the same for all languages.
using json = nlohmann::json;
int main(int argc, char *argv[])
{
    // set logger for tiko
    logger::init_rotated_logger("tiko");
    logger::set_log_level(logger::level_enum::trace_level);
    logger::flush_every(std::chrono::seconds(1));

    // if you want to use db, please set db connection first.
    set_db_connection();

    std::string path = "storage";
    std::map<int, std::shared_ptr<Tiko>> clients;
    int client_nums = 0;
    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        auto telephone = entry.path().filename().string();
        auto client = std::make_shared<Tiko>(telephone);
        clients.emplace(client->get_client_id(), std::move(client));
        client_nums++;
    }

    if (client_nums == 0)
    {
        logger::error("\2key\3{}\2msg\3{}", "storage_is_empty", "");
        std::exit(-1);
    }

    // disable TDLib logging
    td_execute("{\"@type\":\"setLogVerbosityLevel\", \"new_verbosity_level\":0}");
    const double WAIT_TIMEOUT = 10.0; // seconds
    while (true)
    {
        const char *result = td_receive(WAIT_TIMEOUT);
        if (result != nullptr)
        {
            json rsp = json::parse(result);
            logger::info("\2key\3{}\2msg\3{}", "rcv_msg", result);
            if (rsp.contains("@client_id"))
            {
                clients[rsp["@client_id"]]->receive(result);
            }
            else
            {
                logger::info("\2key\3{}\2msg\3{}", "msg_with_no_client", result);
            }
        }
    }
}