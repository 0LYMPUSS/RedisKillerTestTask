#pragma once 

#include <unordered_map>
#include <string>
#include <optional>
#include <spdlog/spdlog.h>

#include "i_serializable.h"

/// @brief base class for a storage wrapper
/// @tparam k key
/// @tparam v value
template<class k, class v>
class storage
{
public:
    virtual ~storage() = default;

    //returns a value corresponding to given key if key exists or null if the key doesn't exist
    virtual std::optional<v> get(const k& key) const = 0;
    //puts a value in storage, returns previous value if key already exists
    virtual std::optional<v> put(const k&key , const v& val) = 0;
    //removes a value corresponding to given key from storage, returns null if the key doesn't exist
    virtual std::optional<v> del(const k& key) = 0;
    //returns the amount of values currently stored
    virtual size_t count() const = 0;

};



template<class k, class v>
class storage_map :
    public storage<k, v>, public i_serializable
{
public:

    // Returns a value corresponding to given key if key exists or null if the key doesn't exist
    virtual std::optional<v> get(const k& key) const override;
    // Puts a value in storage, returns previous value if key already exists
    virtual std::optional<v> put(const k& key, const v& val) override;
    // Removes a value corresponding to given key from storage, returns nullopt if the key doesn't exist
    virtual std::optional<v> del(const k& key) override;
    // Returns the amount of values currently stored in the storage
    virtual size_t count() const override;

    virtual json serialize() override { return json(storage_); };
    virtual void deserialize(const json& j) override 
    {
        try
        {
            auto m = j.get<std::unordered_map<std::string, std::string>>();
            for (auto& p : m) {
                storage_.insert(p);
            }
        }
        catch(...)
        {
            spdlog::error("Could not deserialize storage from provided file. Please make sure file contents are valid.");
        }
    };

    storage_map()
    {
        storage_ = std::unordered_map<k, v>();
        spdlog::debug("Storage created.");
    }

    storage_map(json j)
    {
        storage_ = std::unordered_map<k, v>();
        deserialize(j);
    }

private:

    std::unordered_map<k, v> storage_;

};

template <class k, class v>
std::optional<v> storage_map<k, v>::get(const k &key) const
{
    if (storage_.contains(key))
    {
        spdlog::debug("Value <" + storage_.at(key) + "> corresponding to key <" + key + "> was found.");
        return std::optional<v>(storage_.at(key));
    }
    spdlog::warn("No value corresponding to given key <" + key + "> exists. No value returned.");
    return std::nullopt;
}

template <class k, class v>
std::optional<v> storage_map<k, v>::put(const k& key, const v& val)
{
    v old_val;
    bool exists = storage_.contains(key);
    if (exists) {
        //delete old value otherwise it would not be stored via insert method
        old_val = this->del(key).value();
    }
    storage_.insert({key, val});

    std::string msg = "Value <" + val + "> corresponding to key <" + "> has been added to storage.";
    msg += exists ? "Previous value  <" + old_val + "> was replaced." : "No prevoius value was found.";
    spdlog::debug(msg);

    return exists ? std::optional<v>{old_val} : std::nullopt;
}

template <class k, class v>
std::optional<v> storage_map<k, v>::del(const k &key)
{
    if (storage_.contains(key))
    {
        auto val = std::optional<v>{storage_.at(key)};
        storage_.erase(key);
        spdlog::debug("Value <" + val.value() + "> of given key <" + key 
            + "> was deleted. Current element count is " + std::to_string(this->count()) + ".");
        return val;
    }

    // if key doesn't exist return null
    spdlog::warn("Given key <" + key + "> doesn't exist. No value was deleted."); 
    return std::nullopt;
}


template <class k, class v>
size_t storage_map<k, v>::count() const
{
    spdlog::debug("Current element count is " + std::to_string(storage_.size()) + ".");
    return storage_.size();
}

