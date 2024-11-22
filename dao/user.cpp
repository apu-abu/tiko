#include <format>
#include <pqxx/pqxx>

#include "user.h"
#include "utils/database.h"
#include "utils/logger.h"

bool User::insert()
{
    if (uid_ <= 0)
    {
        return false;
    }

    auto sql = std::format("INSERT INTO users (uid, pet, phone, first_name, last_name, username, content) VALUES ({}, '{}', '{}', '{}', '{}', '{}', '{}')", uid_, pet_, phone_, first_name_, last_name_, username_, content_);

    try
    {
        pqxx::connection pg_conn(get_db_connection());
        pqxx::work tx(pg_conn);
        sql = std::format("INSERT INTO users (uid, pet, phone, first_name, last_name, username, content) VALUES ({}, '{}', '{}', '{}', '{}', '{}', '{}')", uid_, pet_, phone_, tx.esc(first_name_), tx.esc(last_name_), tx.esc(username_), tx.esc(content_));
        tx.exec(sql);
        tx.commit();
    }
    catch (std::exception &e)
    {
        logger::warn("\2db_fail\3{}\2msg\3{}", sql, e.what());
        return false;
    }

    return true;
}

bool User::update()
{
    if (uid_ <= 0)
    {
        return false;
    }

    auto sql = std::format("update users set first_name = '{}', last_name = '{}', phone = '{}', username = '{}', content = '{}' where uid = {} and pet = {}", first_name_, last_name_, phone_, username_, uid_, pet_, content_);
    try
    {
        pqxx::connection pg_conn(get_db_connection());
        pqxx::work tx(pg_conn);
        sql = std::format("update users set first_name = '{}', last_name = '{}', phone = '{}', username = '{}', content = '{}' where uid = {} and pet = {}", tx.esc(first_name_), tx.esc(last_name_), phone_, tx.esc(username_), tx.esc(content_), uid_, pet_);
        tx.exec(sql);
        tx.commit();
    }
    catch (std::exception &e)
    {
        logger::warn("\2db_fail\3{}\2msg\3{}", sql, e.what());
        return false;
    }
    return true;
}

bool User::get_user(bool &exist)
{
    if (uid_ <= 0)
    {
        return false;
    }
    exist = false;
    auto sql = std::format("select first_name, last_name, username, phone, content from users where uid = {} and pet = {}", uid_, pet_);
    pqxx::result res;
    try
    {
        pqxx::connection pg_conn(get_db_connection());
        pqxx::work tx(pg_conn);
        res = tx.exec(sql);
        tx.commit();
    }
    catch (std::exception &e)
    {
        logger::warn("\2db_fail\3{}\2msg\3{}", sql, e.what());
        return false;
    }

    for (auto row : res)
    {
        first_name_ = row[0].as<std::string>();
        last_name_ = row[1].as<std::string>();
        username_ = row[2].as<std::string>();
        phone_ = row[3].as<std::string>();
        content_ = row[4].as<std::string>();
    }

    exist = true;
    return true;
}