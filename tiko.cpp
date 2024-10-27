//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <memory>

#include <td/telegram/td_json_client.h>
#include <nlohmann/json.hpp>

#include "tg/client.h"

// Basic example of TDLib JSON interface usage.
// Native interface should be preferred instead in C++, so here is only an example of
// the main event cycle, which should be essentially the same for all languages.
using json = nlohmann::json;
int main()
{
    // disable TDLib logging
    td_execute("{\"@type\":\"setLogVerbosityLevel\", \"new_verbosity_level\":0}");
    std::map<int, std::shared_ptr<tiko>> clients;
    for (int i = 0; i < 1; i++)
    {
        auto client = std::make_shared<tiko>();
        clients.emplace(client->get_client_id(), std::move(client));
    }

    const double WAIT_TIMEOUT = 10.0; // seconds
    while (true)
    {
        const char *result = td_receive(WAIT_TIMEOUT);
        if (result != nullptr)
        {
            json rsp = json::parse(result);
            std::cout << result << std::endl;
            if (rsp.contains("@client_id"))
            {
                clients[rsp["@client_id"]]->receive(result);
            }
        }
    }
}