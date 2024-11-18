//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <memory>
#include <format>

#include <CLI/CLI.hpp>
#include <td/telegram/td_json_client.h>
#include <nlohmann/json.hpp>

#include "tg/client.h"
#include "tg/routine.h"
#include "utils/variable.h"
#include "utils/schedule.h"
#include "utils/assert.h"
#include "utils/logger.h"

// Basic example of TDLib JSON interface usage.
// Native interface should be preferred instead in C++, so here is only an example of
// the main event cycle, which should be essentially the same for all languages.
using json = nlohmann::json;
int main(int argc, char *argv[])
{
    // set logger for tiko
    logger::init_rotated_logger("tgen");
    logger::set_log_level(logger::level_enum::trace_level);
    logger::flush_every(std::chrono::seconds(1));

    CLI::App tapp("tgen client generator");
    std::string telephone;
    std::string url;
    tapp.add_option("-t,--telephone", telephone, "client's telephone number, eg: 8619999999999")->required();
    tapp.add_option("-u,--url", url, "client's code url, eg: https://what.is.code/12345")->required();
    CLI11_PARSE(tapp, argc, argv);

    // disable TDLib logging
    td_execute("{\"@type\":\"setLogVerbosityLevel\", \"new_verbosity_level\":0}");
    if (telephone.size() == 0)
    {
        std::cout << "input phone number is empty" << std::endl;
        return -1;
    }
    auto client = std::make_shared<Tiko>(telephone);
    register_8hao_account_config(telephone, url);

    const double WAIT_TIMEOUT = 10.0; // seconds
    while (true)
    {
        const char *result = td_receive(WAIT_TIMEOUT);
        if (client->is_logout())
        {
            std::cout << "Final: " << telephone << " is logout." << std::endl;
            return -1;
        }
        bool finish = false;
        if (countdown(telephone, finish) && finish)
        {
            std::cout << "Final: " << telephone << " is login." << std::endl;
            return 0;
        }
        if (result != nullptr)
        {
            // std::cout << result << std::endl;
            json rsp = json::parse(result);
            if (rsp.contains("@client_id") && rsp.contains("@client_id") == client->get_client_id())
            {
                client->receive(result);
            }
            else
            {
                std::cout << "receive no client message: " << result << std::endl;
            }
        }
    }
}