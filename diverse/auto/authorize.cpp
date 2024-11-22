#include <regex>
#include <string>
#include <sstream>
#include <memory>

#include <httplib.h>

#include "utils/ada.h"
#include "utils/assert.h"
#include "utils/defer.h"
#include "utils/schedule.h"
#include "utils/variable.h"
#include "diverse/authorize.h"

using namespace httplib;

class TgRegister
{
public:
    TgRegister(std::string phone, std::string url) : phone_(phone)
    {
        ada::result<ada::url_aggregator> res = ada::parse<ada::url_aggregator>(url);
        if (!res)
        {
            PANIC("parse url failed");
        }

        host_ = res->get_host();
        uri_ = res->get_pathname();
    }

    bool fetch_code()
    {
        httplib::Client cli(host_);
        auto res = cli.Get(uri_);
        if (!res || res->status != 200)
        {
            return false;
        }
        std::string body = res->body;
        std::regex dead("号码已死");
        if (std::regex_search(body, dead))
        {
            PANIC("account is dead.");
        }

        std::regex nocode("无三十分钟内的登录消息");
        if (std::regex_search(body, nocode))
        {
            return false;
        }

        std::regex unreceive("未获取到");
        if (std::regex_search(body, unreceive))
        {
            return false;
        }

        std::regex code_field("<td id=\"code\">[0-9]*</td>");
        std::smatch matches;
        if (std::regex_search(body, matches, code_field))
        {
            if (matches.size() != 1)
            {
                PANIC("code field is more than expected.");
            }
            std::string code_str = matches[0];
            std::regex code("[0-9]+");
            if (std::regex_search(code_str, matches, code))
            {
                if (matches.size() != 1)
                {
                    PANIC("parse code failed.");
                }
                code_ = matches[0];
            }
            else
            {
                PANIC("parse code from code field failed.");
            }
        }
        else
        {
            PANIC("parse code from html failed.");
        }

        std::regex password_field("<td id=\"password\">[0-9a-zA-Z]*</td>");
        if (std::regex_search(body, matches, password_field))
        {
            if (matches.size() != 1)
            {
                PANIC("password field is more than expected.");
            }
            std::string password_str = matches[0];
            std::regex password(">.+<");
            if (std::regex_search(password_str, matches, password))
            {
                if (matches.size() != 1)
                {
                    PANIC("parse password failed.");
                }
                std::string pre = matches[0];
                pre.erase(0, 1);
                pre.erase(pre.size() - 1, 1);
                password_ = pre;
            }
            else
            {
                PANIC("parse password from password field failed.");
            }
        }
        else
        {
            PANIC("parse password from html failed.");
        }

        std::regex time_field("<th>登 录 时 间 : </th>\n                <td>[0-9-:+ ]+</td>");
        if (std::regex_search(body, matches, time_field))
        {
            if (matches.size() != 1)
            {
                PANIC("time field is more than expected.");
            }
            std::string time_str = matches[0];
            std::regex time("[0-9]+-[0-9]+-[0-9]+ [0-9]+:[0-9]+:[0-9]+");
            if (std::regex_search(time_str, matches, time))
            {
                if (matches.size() != 1)
                {
                    PANIC("parse time failed.");
                }
                std::string time_str = matches[0];
                // 2024-11-06 15:44:14 utc时间
                std::istringstream ss(time_str);
                std::tm message_time;
                ss >> std::get_time(&message_time, "%Y-%m-%d %H:%M:%S");
                if (ss.fail())
                {
                    PANIC("parse time: " + time_str + " failed");
                }
                std::time_t time = mktime(&message_time) + 8 * 60 * 60 + 10 * 60;
                if (time == -1)
                {
                    PANIC("convert to server time fail");
                }
                time_ = time;
            }
            else
            {
                PANIC("parse time from time field failed.");
            }
        }
        else
        {
            PANIC("parse time from html failed.");
        }

        return true;
    }

    std::string get_password()
    {
        return password_;
    }

    bool is_validate(std::time_t now)
    {
        return now <= time_;
    }

    std::string get_code()
    {
        return code_;
    }

private:
    std::string phone_;
    // 获取code的url
    std::string host_;
    std::string uri_;
    std::string password_;
    std::time_t time_;
    std::string code_;
};

std::map<std::string, std::shared_ptr<TgRegister>> ACCOUNTS;

void get_password(const std::string phone_number, std::string &password)
{
    if (ACCOUNTS.find(phone_number) == ACCOUNTS.end())
    {
        PANIC("cant find " + phone_number + "'s account info");
    }

    password = ACCOUNTS[phone_number]->get_password();
}

bool get_authorize_code(const std::string phone_number, std::time_t tp, std::string &code)
{
    if (ACCOUNTS.find(phone_number) == ACCOUNTS.end())
    {
        std::string url;
        if (!get_8hao_account_config(phone_number, url))
        {
            PANIC("cant find " + phone_number + "'s url config");
        }
        ACCOUNTS[phone_number] = std::make_shared<TgRegister>(phone_number, url);
    }
    for (int i = 0; i <= 5; i++)
    {
        if (i == 5)
        {
            return false;
        }
        std::cout << "try to fetch code for " << i + 1 << " times." << std::endl;
        bool success = false;
        DEFER({
            if (!success)
            {
                std::cout << "no code was found." << std::endl;
            }
            else
            {
                std::cout << "read code success." << std::endl;
            }
        });
        if (!ACCOUNTS[phone_number]->fetch_code())
        {
            sleep(10);
            continue;
        }
        if (!ACCOUNTS[phone_number]->is_validate(tp))
        {
            sleep(10);
            continue;
        }
        success = true;
        break;
    }

    code = ACCOUNTS[phone_number]->get_code();
    return true;
}

void on_login(const std::string phone_number)
{
    std::cout << phone_number << " login success.." << std::endl;
    std::time_t fut = std::time(0) + 60;
    set_countdown(phone_number, fut);
}