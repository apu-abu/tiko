#include <cstdlib>

#include "assert.h"
#include "database.h"

namespace
{
    std::string URL = "";
}

std::string get_db_connection()
{
    return URL;
}

void set_db_connection()
{
    URL = getenv("POSTGRESQL_CONNECTION");
    if (URL.size() == 0)
    {
        PANIC("NO POSTGRESQL_CONNECTION in enveriment");
    }
}
