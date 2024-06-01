#pragma once

#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

class i_serializable
{
    virtual json serialize() = 0;
    virtual void deserialize(json j) = 0;
};