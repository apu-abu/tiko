#include <iostream>

#ifndef NDEBUG
#define ASSERT(condition, message)                                             \
    do                                                                         \
    {                                                                          \
        if (!(condition))                                                      \
        {                                                                      \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__   \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            std::terminate();                                                  \
        }                                                                      \
    } while (false)
#define PANIC(message)                     \
    do                                     \
    {                                      \
        std::cerr << message << std::endl; \
        std::abort();                      \
    } while (false)
#else
#define ASSERT(condition, message) \
    do                             \
    {                              \
    } while (false)
#define PANIC(message) \
    do                 \
    {                  \
    } while (false)
#endif