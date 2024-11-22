#include <string>

//                                         Table "public.users"
//    Column   |            Type             | Collation | Nullable |              Default
// ------------+-----------------------------+-----------+----------+-----------------------------------
//  id         | integer                     |           | not null | nextval('users_id_seq'::regclass)
//  phone      | character(15)               |           |          |
//  uid        | bigint                      |           |          |
//  pet        | bigint                      |           |          |
//  first_name | character varying(32)       |           |          |
//  last_name  | character varying(32)       |           |          |
//  username   | character varying(64)       |           |          |
//  created_at | timestamp without time zone |           |          | CURRENT_TIMESTAMP
//  updated_at | timestamp without time zone |           |          | CURRENT_TIMESTAMP
// Indexes:
//     "users_pkey" PRIMARY KEY, btree (id)
//     "users_uid_pet" btree (uid, pet)
// Triggers:
//     users_updated_at BEFORE UPDATE ON users FOR EACH ROW EXECUTE FUNCTION update_modified_column()
class User
{
public:
    User(std::int64_t uid, std::int64_t pet) : uid_(uid), pet_(pet) {};
    User(std::int64_t uid,
         std::int64_t pet,
         std::string phone,
         std::string first_name,
         std::string last_name,
         std::string username,
         std::string content) : uid_(uid), pet_(pet), phone_(phone), first_name_(first_name),
                                last_name_(last_name), content_(content) {};
    bool insert();
    bool update();
    bool get_user(bool &exist);

private:
    std::string phone_;
    std::int64_t uid_;
    std::int64_t pet_;
    std::string first_name_;
    std::string last_name_;
    std::string username_;
    std::string content_;
};
