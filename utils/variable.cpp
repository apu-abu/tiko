#include <map>

#include "variable.h"

std::map<std::string, std::string> URLS_8HAO;

void register_8hao_account_config(std::string phone, std::string url)
{
    URLS_8HAO[phone] = url;
}

bool get_8hao_account_config(const std::string phone, std::string &url)
{
    if (URLS_8HAO.find(phone) != URLS_8HAO.end())
    {
        url = URLS_8HAO[phone];
        return true;
    }
    return false;
}
