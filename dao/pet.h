#include <string>

//                                         Table "public.pets"
//    Column   |            Type             | Collation | Nullable |             Default
// ------------+-----------------------------+-----------+----------+----------------------------------
//  id         | integer                     |           | not null | nextval('pets_id_seq'::regclass)
//  phone      | character(15)               |           |          |
//  uid        | bigint                      |           |          |
//  first_name | character varying(32)       |           |          |
//  last_name  | character varying(32)       |           |          |
//  username   | character varying(64)       |           |          |
//  heartbeat  | integer                     |           |          |
//  status     | integer                     |           |          |
//  created_at | timestamp without time zone |           |          | CURRENT_TIMESTAMP
//  updated_at | timestamp without time zone |           |          | CURRENT_TIMESTAMP
// Indexes:
//     "pets_pkey" PRIMARY KEY, btree (id)
//     "pets_phone" btree (phone)
//     "pets_status_heartbeat" btree (status, heartbeat)
//     "pets_uid" btree (uid)
// Triggers:
//     pets_updated_at BEFORE UPDATE ON pets FOR EACH ROW EXECUTE FUNCTION update_modified_column()
// pet 的status 流转
// 0: 未定义 预留
// 1: pet 正常, 可以执行命令
// 2: pet 下线, 比如logout状态, 有恢复成 1的可能, 也有可能会死在这个状态
// 3: pet死亡, 被封禁了. 这个状态下的pet最终会进入 100.
// 100: pet 已经清理. 最终状态.
class Pet
{
public:
    Pet(std::string phone) : phone_(phone) {};
    Pet(std::int64_t uid) : uid_(uid) {};
    Pet(std::string phone, std::int64_t uid, std::string first_name, std::string last_name, std::string username, std::int64_t heartbeat, std::int16_t status, std::int16_t age) : phone_(phone), uid_(uid), first_name_(first_name), last_name_(last_name), username_(username), heartbeat_(heartbeat), status_(status), age_(age) {};
    bool insert();
    bool update_heartbeat(std::int64_t uid, std::int64_t heartbeat);
    bool update_status(std::int64_t uid, std::int16_t status);
    bool update_status(std::string phone, std::int16_t status);
    bool get_by_phone(std::string phone, bool &exist);
    bool get_by_uid(std::int64_t uid, bool &exist);

private:
    std::string phone_;
    std::int64_t uid_;
    std::string first_name_;
    std::string last_name_;
    std::string username_;
    std::int64_t heartbeat_;
    std::int16_t status_;
    std::int16_t age_;
};
