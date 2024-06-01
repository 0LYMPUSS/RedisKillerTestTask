#pragma once
#include <memory>
#include <asio.hpp>
#include <string>
#include <iostream>
#include <spdlog/spdlog.h>

#include "storage.h"
#include "command.h"

class tcp_connection 
    : public std::enable_shared_from_this<tcp_connection>
{
public:
    typedef std::shared_ptr<tcp_connection> pointer;

    static pointer create(asio::io_context& io_context, storage<std::string, std::string>& storage_)
    {   
        return pointer(new tcp_connection(io_context, storage_));
    }

    asio::ip::tcp::socket& socket()
    {
        return socket_;
    }

    void start()
    {
        spdlog::info("A new client has been connected!");
        //we will read a msg from client process it and sent it back
        const char delim = '\n';
        //read the msg
        asio::async_read_until(socket_, message_, delim,
            std::bind(&tcp_connection::handle_read, shared_from_this(),
            asio::placeholders::error, asio::placeholders::bytes_transferred));
        
    }

private:

    tcp_connection(asio::io_context& io_context, storage<std::string, std::string>& strg) :
        socket_(io_context), storage_(strg), COUNT(strg) {}

    storage<std::string, std::string>& storage_;

    void handle_write(const asio::error_code&, size_t) 
    {
        //we will read a msg from client process it and sent it back
        const char delim = '\n';
        //read the msg
        asio::async_read_until(socket_, message_, delim,
            std::bind(&tcp_connection::handle_read, shared_from_this(),
            asio::placeholders::error, asio::placeholders::bytes_transferred));
    }

    void handle_read(const asio::error_code&, size_t len)  
    {
        message_.commit(len);

        std::istream is(&message_);
        std::string str;
        std::getline(is, str);

        std::cout << "msg recieved " << str << '\n';

        response_msg = process_message(str); 

        asio::async_write(socket_, asio::buffer(response_msg), 
            std::bind(&tcp_connection::handle_write, shared_from_this(),
            asio::placeholders::error,
            asio::placeholders::bytes_transferred));

        message_.consume(len);
    }

    asio::ip::tcp::socket socket_;
    asio::streambuf message_;
    std::string response_msg;

    std::string process_message(const std::string& msg)
    {
        return "client said: " + msg;
    }

    //commands

    count_command<std::string, std::string> COUNT;

};