#pragma once
#include <string>

#include "storage.h"

/// @brief a stateless object which executes a given command
class command
{
public:
    virtual std::string execute() = 0;
    virtual ~command() = default;

};

/// @brief command objects which require access to storage 
template<class k, class v>
class storage_command
    : public command
{
public:
    storage_command(storage<k, v>& strg)
    : storage_(strg)
    {
    }
protected:
    storage<k, v>& storage_;
};

template<class k, class v>
/// @brief Used to define the count command
class count_command : 
    public storage_command<k, v>
{
public:
    
    count_command(storage<k, v>& strg)
        : storage_command<k, v>(strg)
    {
    }

    virtual std::string execute() override
    {
        spdlog::debug("COUNT command called. Current element count is " 
            + std::to_string(storage_command<k, v>::storage_.count()) + ".");
        return "OK" + std::to_string(storage_command<k, v>::storage_.count());
    }

};