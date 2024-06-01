#pragma once
#include <string>

#include "storage.h"

/// @brief a stateless object which executes a given command
class command
{
public:
    virtual std::string execute(const std::vector<std::string>& data) = 0;
    virtual ~command() = default;

};

/// @brief command objects which require access to storage 
class storage_command
    : public command
{
public:
    storage_command(storage_map<std::string, std::string>& strg)
    : storage_(strg)
    {
    }
protected:
    storage_map<std::string, std::string>& storage_;
};

/// @brief Used to define the count command
class count_command : 
    public storage_command
{
public:
    
    count_command(storage_map<std::string, std::string>& strg)
        : storage_command(strg) {}

    virtual std::string execute(const std::vector<std::string>& data) override;

};

/// @brief Defines PUT command
class put_command
    : public storage_command
{
public:
    put_command(storage_map<std::string, std::string>& strg)
        : storage_command(strg) {}

    virtual std::string execute(const std::vector<std::string>& data) override;
    
};