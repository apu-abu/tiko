#include <iostream>

#include "diverse/authorize.h"

void get_password(const std::string phone_number, std::string &password)
{
    std::cout << "Enter password.." << std::endl;
    std::cin >> password;
}

bool get_authorize_code(const std::string phone_number, std::time_t tp, std::string &code)
{
    std::cout << "Enter (SMS) code.." << std::endl;
    std::cin >> code;
    return true;
}

void on_login(const std::string phone_number)
{
    std::cout << phone_number << " login success.." << std::endl;
}