#include <iostream>

#include "authorize.h"

#include "utils/assert.h"

void get_password(const std::string phone_number, std::string &password)
{
    PANIC("unreachable: get_password");
}

bool get_authorize_code(const std::string phone_number, std::time_t tp, std::string &code)
{
    PANIC("unreachable: get_authorize_code");
    return false;
}

void on_login(const std::string phone_number)
{
    std::cout << phone_number << " login success.." << std::endl;
}