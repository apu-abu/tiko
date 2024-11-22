# tiko

tiko is named by my wechat name ticco.

## how to compile?

sh build.sh

## how to use?

## pgsql

```pgsql

# local:
postgres://user_tiko:F7JuZvnxx6tfpxgAgmvD@localhost:5432/tiko
# remote:
postgres://user_tiko:F7JuZvnxx6tfpxgAgmvD@pgm-0iwi734y3l4o9tzpvo.pgsql.japan.rds.aliyuncs.com:5432/tiko

# 创建用户
CREATE USER user_tiko WITH PASSWORD 'F7JuZvnxx6tfpxgAgmvD';
GRANT CONNECT ON DATABASE tiko TO user_tiko;
GRANT USAGE ON SCHEMA public TO user_tiko;
# 创建 trigger of update_modified_column
CREATE OR REPLACE FUNCTION update_modified_column()
RETURNS TRIGGER AS $$
BEGIN
    NEW.updated_at = now();
    RETURN NEW;
END;
$$ language 'plpgsql';

# create table pets
CREATE TABLE pets (
    id SERIAL PRIMARY KEY,
    phone char(15),
    uid bigint,
    first_name varchar(32),
    last_name varchar(32),
    username varchar(64),
    heartbeat integer,
    status integer,
    age integer,
    created_at timestamp DEFAULT current_timestamp,
    updated_at timestamp DEFAULT current_timestamp
);

CREATE TRIGGER pets_updated_at BEFORE UPDATE ON pets FOR EACH ROW EXECUTE PROCEDURE update_modified_column();

CREATE INDEX pets_uid ON pets(uid);
CREATE INDEX pets_phone ON pets(phone);
CREATE INDEX pets_status_heartbeat ON pets(status, heartbeat);

CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    phone char(15),
    uid bigint,
    pet bigint,
    first_name varchar(32),
    last_name varchar(32),
    username varchar(64),
    content text,
    created_at timestamp DEFAULT current_timestamp,
    updated_at timestamp DEFAULT current_timestamp
);
CREATE INDEX users_uid_pet ON users(uid, pet);
CREATE TRIGGER users_updated_at BEFORE UPDATE ON users FOR EACH ROW EXECUTE PROCEDURE update_modified_column();
```
