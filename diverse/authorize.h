#include <ctime>
#include <string>

void get_password(const std::string phone_number, std::string &password);

bool get_authorize_code(const std::string phone_number, std::time_t tp, std::string &code);

void on_login(const std::string phone_number);
