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

    static pointer create(asio::io_context& io_context, storage_map<std::string, 
        std::string>& storage_, int& clients, int max_clients)
    {   
        return pointer(new tcp_connection(io_context, storage_, clients, max_clients));
    }

    asio::ip::tcp::socket& socket() { return socket_; }

    void start();

    ~tcp_connection()
    {
        //decrease ammount of clients
        --clients_;
        for (auto& cmd : commands)
        {
            delete(cmd.second);
        }
        spdlog::warn("A client connection has been terminated.");
    }

private:

    tcp_connection(asio::io_context& io_context, storage_map<std::string, std::string>& strg,
        int& clients, int max_clients) 
        : socket_(io_context), storage_(strg), clients_(clients), max_clients_(max_clients)
        {
            register_commands();
        }

    storage_map<std::string, std::string>& storage_;

    void handle_write(const asio::error_code&, size_t);

    void handle_read(const asio::error_code& error, size_t len);

    asio::ip::tcp::socket socket_;
    asio::streambuf message_;
    std::string response_msg;

    std::string process_message(const std::string& cmd, const std::vector<std::string>& args);

    // COMMANDS \\
    //command map
    std::unordered_map<std::string, command*> commands;

    void register_commands();

    int& clients_;
    int max_clients_;
};