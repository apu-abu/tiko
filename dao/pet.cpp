#include <format>
#include <pqxx/pqxx>

#include "pet.h"
#include "utils/database.h"
#include "utils/logger.h"

bool Pet::insert()
{
    if (uid_ <= 0)
    {
        return false;
    }

    if (first_name_.length() < 2)
    {
        return false;
    }

    if (phone_.length() < 5)
    {
        return false;
    }

    // sql = "INSERT INTO pets (phone, uid, first_name, last_name, heartbeat, username, status)"
    //    "VALUES ('" +
    //    phone + "', " + std::to_string(uid) + ", '" + first_name + "', '" + last_name + "', " + std::to_string(t) + ", '" + username + "', 1 );";
    auto sql = std::format("INSERT INTO pets (phone, uid, first_name, last_name, username, heartbeat, status, age) VALUES ('{}', {}, '{}', '{}', '{}', {}, {}, {})", phone_, uid_, first_name_, last_name_, username_, heartbeat_, status_, age_);

    try
    {
        pqxx::connection pg_conn(get_db_connection());
        pqxx::work tx(pg_conn);
        sql = std::format("INSERT INTO pets (phone, uid, first_name, last_name, username, heartbeat, status, age) VALUES ('{}', {}, '{}', '{}', '{}', {}, {}, {})", phone_, uid_, tx.esc(first_name_), tx.esc(last_name_), tx.esc(username_), heartbeat_, status_, age_);
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

bool Pet::update_heartbeat(std::int64_t uid, std::int64_t heartbeat)
{
    if (uid <= 0)
    {
        return false;
    }

    // sql = "update pets set status = 1, heartbeat =" +
    //    std::to_string(t) + " where uid = " + std::to_string(uid);
    std::time_t now = std::time(0);
    auto sql = std::format("update pets set heartbeat = {}, status = 1 where uid= {}", now, uid);
    try
    {
        pqxx::connection pg_conn(get_db_connection());
        pqxx::work tx(pg_conn);
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

bool Pet::update_status(std::string phone, std::int16_t status)
{
    // sql = "update pets set status = 1, heartbeat =" +
    //    std::to_string(t) + " where uid = " + std::to_string(uid);
    std::time_t now = std::time(0);
    auto sql = std::format("update pets set status = {}, heartbeat = {} where phone='{}'", status, now, phone);
    try
    {
        pqxx::connection pg_conn(get_db_connection());
        pqxx::work tx(pg_conn);
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

bool Pet::update_status(std::int64_t uid, std::int16_t status)
{
    if (uid <= 0)
    {
        return false;
    }

    // sql = "update pets set status = 1, heartbeat =" +
    //    std::to_string(t) + " where uid = " + std::to_string(uid);
    std::time_t now = std::time(0);
    auto sql = std::format("update pets set status = {}, heartbeat = {} where uid={}", status, now, uid);
    try
    {
        pqxx::connection pg_conn(get_db_connection());
        pqxx::work tx(pg_conn);
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

bool Pet::get_by_phone(std::string phone, bool &exist)
{
    exist = false;
    if (phone.length() < 4)
    {
        return false;
    }
    auto sql = std::format("select uid, status, heartbeat, first_name, last_name, username, age from pets where phone='{}'", phone);
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

    if (res.size() != 1)
    {
        logger::warn("\2db_fail\3{}\2msg\3no such pet: phone{}", sql, phone);
        return false;
    }
    for (auto row : res)
    {
        uid_ = row[0].as<std::int64_t>();
        phone_ = phone;
        status_ = row[1].as<std::int16_t>();
        heartbeat_ = row[2].as<std::int64_t>();
        first_name_ = row[3].as<std::string>();
        last_name_ = row[4].as<std::string>();
        username_ = row[5].as<std::string>();
        age_ = row[6].as<std::int16_t>();
    }

    exist = true;
    return true;
}

bool Pet::get_by_uid(std::int64_t uid, bool &exist)
{
    exist = false;
    if (uid <= 0)
    {
        return false;
    }
    auto sql = std::format("select phone, status, heartbeat, first_name, last_name, username, age from pets where uid='{}'", uid);
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
        logger::warn("db_fail[{}] msg[{}]", sql, e.what());
        return false;
    }

    if (res.size() != 1)
    {
        logger::warn("db_fail[{}] msg[no such pet: uid{}]", sql, uid);
        return false;
    }

    for (auto row : res)
    {
        uid_ = uid;
        phone_ = row[0].as<std::string>();
        status_ = row[1].as<std::int16_t>();
        heartbeat_ = row[2].as<std::int64_t>();
        first_name_ = row[3].as<std::string>();
        last_name_ = row[4].as<std::string>();
        username_ = row[5].as<std::string>();
        age_ = row[6].as<std::int16_t>();
    }

    exist = true;
    return true;
}
