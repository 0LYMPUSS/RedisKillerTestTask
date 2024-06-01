#include "command.h"

#include <fstream>

std::string put_command::execute(const std::vector<std::string> &data)
{
    if (data.size() < 2)
    {
        spdlog::warn("Not enough arguments provided to command PUT. Recieved " 
            + std::to_string(data.size()) + ". Expected 2.");
        return "Not enough arguements to coplete command. No value was added.";
    } 
    else if (data.size() > 2) 
    {
        spdlog::warn("Too many arguments provided to command PUT. Recieved " 
            + std::to_string(data.size()) + ". Expected 2.");
    }
    spdlog::debug("PUT command called with key <" + data[0] + "> and value <" 
        + data[1] + ">. Current element count is " 
        + std::to_string(storage_command::storage_.count()) + ".");
    auto old_val = storage_.put(data[0], data[1]);

    return "OK" + (old_val.has_value() ? " " + old_val.value() : "") ;
}

std::string count_command::execute(const std::vector<std::string>& data)
    {
        spdlog::debug("COUNT command called. Current element count is " 
            + std::to_string(storage_command::storage_.count()) + ".");
        if (data.size() > 0)
        {
            spdlog::warn("Too many arguments provided to command COUNT. Recieved " 
                + std::to_string(data.size()) + ". Expected 0.");
        }
        return "OK " + std::to_string(storage_command::storage_.count());
    }

std::string get_command::execute(const std::vector<std::string> &data)
{
    if (data.size() < 1)
    {
        spdlog::warn("Not enough arguments provided to command GET. Recieved " 
            + std::to_string(data.size()) + ". Expected 1.");
        return "Not enough arguements to coplete command.";
    } 
    else if (data.size() > 1) 
    {
        spdlog::warn("Too many arguments provided to command GET. Recieved " 
            + std::to_string(data.size()) + ". Expected 1.");
    }
    
    spdlog::debug("GET command called with key <" + data[0] + ">.");

    auto val = storage_.get(data[0]);

    return (val.has_value() ? "OK " + val.value() : "NE");
}

std::string del_command::execute(const std::vector<std::string> &data)
{
    if (data.size() < 1)
    {
        spdlog::warn("Not enough arguments provided to command DEL. Recieved " 
            + std::to_string(data.size()) + ". Expected 1.");
        return "Not enough arguements to coplete command.";
    } 
    else if (data.size() > 1) 
    {
        spdlog::warn("Too many arguments provided to command DEL. Recieved " 
            + std::to_string(data.size()) + ". Expected 1.");
    }
    
    spdlog::debug("DEL command called with key <" + data[0]
        + ">. Current element count is " 
        + std::to_string(storage_command::storage_.count()) + ".");

    auto val = storage_.del(data[0]);

    return (val.has_value() ? "OK " + val.value() : "NE");
}

std::string dump_command::execute(const std::vector<std::string> &data)
{
    //data[0] - filename
    
    //store json to provided file
    std::ofstream f;
    f.open(data[0]);
    f << storage_.serialize().dump(4);
    f.flush();
    f.close();

    return "OK";
}
