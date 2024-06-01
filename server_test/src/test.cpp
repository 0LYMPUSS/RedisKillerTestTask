#include <gtest/gtest.h>

#include "storage.h"
#include "command.h"


TEST(storage_testing, storage_count_empty) {
    storage_map<std::string, std::string> storage_ = storage_map<std::string, std::string>();
    EXPECT_EQ(storage_.count(), 0);
}

class storage_test
    : public ::testing::Test
{
protected:

    virtual void SetUp()
    {
        storage_ = new storage_map<std::string, std::string>();
        storage_->put("age", "23");
        storage_->put("name", old_name);
        storage_->put("occupation", "coder");
        storage_->put("experience", "zero");
        storage_->put("tale", "as old as time");
    }

    virtual void TearDown()
    {
        delete(storage_);
    }

    void put_existent()
    {

    }

    storage_map<std::string, std::string>* storage_;
    std::string old_name = "Nick";
    std::string non_existent_key = "fake";
    
    std::string key = "name";
};

TEST_F(storage_test, storage_count_filled) {
    EXPECT_EQ(storage_->count(), static_cast<size_t>(5));
}

//get should return null
TEST_F(storage_test, storage_get_non_existent) {
    EXPECT_FALSE(storage_->get(non_existent_key).has_value());
}

TEST_F(storage_test, storage_get_filled) {
    auto val = storage_->get(key);
    //stop the test if no value returned
    ASSERT_TRUE(val.has_value());
    std::string old_val = val.value();
    EXPECT_EQ(storage_->count(), 5);
    // check that received value equals expected value
    EXPECT_EQ(old_val, old_name);
}

// count should increase
TEST_F(storage_test, storage_put_non_existent_count) {
    std::string new_val = "Wojak";
    auto val = storage_->put(non_existent_key, new_val);
    EXPECT_EQ(storage_->count(), 6);
}

// put should return null
TEST_F(storage_test, storage_put_non_existent_return_value) {
    std::string new_val = "Wojak";
    auto val = storage_->put(non_existent_key, new_val);
    EXPECT_FALSE(val.has_value());
}

TEST_F(storage_test, storage_put_non_existent_value) {
    std::string new_val = "Wojak";
    auto val = storage_->put(non_existent_key, new_val);
    std::string str = storage_->get(non_existent_key).value();
    EXPECT_EQ(new_val, str);
}

//count should not increase
TEST_F(storage_test, storage_put_existent_count) {
    std::string new_val = "Wojak";

    auto val = storage_->put(key, new_val);
    EXPECT_EQ(storage_->count(), 5);
}

// storage should return the previous value corresponding to given key
TEST_F(storage_test, storage_put_existent_return_val) {
    std::string new_val = "Wojak";
    auto val = storage_->put(key, new_val);
    ASSERT_TRUE(val.has_value());
    std::string old_val = val.value();
    EXPECT_EQ(old_val, old_name);
}

//storage should now return new value via get command
TEST_F(storage_test, storage_put_existent_new_val) {
    std::string new_val = "Wojak";
    auto val = storage_->put(key, new_val);
    ASSERT_TRUE(val.has_value());
    EXPECT_EQ(new_val, storage_->get(key));
}

TEST_F(storage_test, del_existent_old_val) {
    auto val = storage_->del(key);
    ASSERT_TRUE(val.has_value());
    EXPECT_STREQ(val.value().c_str(), old_name.c_str());
}

TEST_F(storage_test, del_existent_count) {
    auto val = storage_->del(key);
    EXPECT_EQ(storage_->count(), 4);
}

TEST_F(storage_test, del_non_existent) {
    auto val = storage_->del(non_existent_key);
    ASSERT_FALSE(val.has_value());
}



