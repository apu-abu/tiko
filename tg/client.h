#pragma once
#include <functional>
#include <string>
#include <memory>
#include <map>

class Tiko : public std::enable_shared_from_this<Tiko>
{
public:
    typedef std::function<void(const char *rsp)> respond;

    Tiko(std::string phone_number);
    std::string get_phone_number() const;
    void set_phone_number(std::string phone_number);
    int get_client_id();
    bool is_authorized();
    void set_authorized(bool authorized);
    void set_logout(bool logout);
    bool is_logout();
    std::int64_t get_uid() { return uid_; };
    void set_uid(std::int64_t uid) { uid_ = uid; };
    void send(const char *cmd);
    void receive(const char *update);
    std::int64_t set_respond(std::unique_ptr<respond> rspd);

private:
    void update(const char *msg);
    void response(const char *msg);
    std::unique_ptr<respond> get_respond(std::int64_t);

    std::string phone_number_;
    std::int64_t uid_;
    int client_id_;

    bool is_authorized_{false};
    bool is_logout_{false};
    std::map<std::int64_t, std::unique_ptr<respond>> callbacks_;
};
