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

    static pointer create(asio::io_context& io_context, storage_map<std::string, std::string>& storage_)
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

    ~tcp_connection()
    {
        for (auto& cmd : commands)
        {
            delete(cmd.second);
        }
    }

private:

    tcp_connection(asio::io_context& io_context, storage_map<std::string, std::string>& strg) :
        socket_(io_context), storage_(strg)
        {
            register_commands();
        }

    storage_map<std::string, std::string>& storage_;

    void handle_write(const asio::error_code&, size_t) 
    {
        //we will read a msg from client process it and sent it back
        const char delim = '\n';
        //read the msg
        asio::async_read_until(socket_, message_, delim,
            std::bind(&tcp_connection::handle_read, shared_from_this(),
            asio::placeholders::error, asio::placeholders::bytes_transferred));
    }

    void handle_read(const asio::error_code& error, size_t len)  
    {
        if (error) {
            spdlog::error("Client disconnected!");
            return;
        }
        message_.commit(len);

        std::istream is(&message_);
        std::vector<char> arr(len);
        is.getline(&arr[0], len);
        std::string init(arr.data(), len - 1);
        std::istringstream iss(init);

        std::string cmd;
        std::vector<std::string> args;

        iss >> cmd;
        
        std::string tmp;
        while(iss >> tmp) {
            args.push_back(tmp);
        }

        response_msg = process_message(cmd, args); 

        asio::async_write(socket_, asio::buffer(response_msg), 
            std::bind(&tcp_connection::handle_write, shared_from_this(),
            asio::placeholders::error,
            asio::placeholders::bytes_transferred));

        message_.consume(len);
    }

    asio::ip::tcp::socket socket_;
    asio::streambuf message_;
    std::string response_msg;

    std::string process_message(const std::string& cmd, const std::vector<std::string>& args)
    {
        // handle incorrect commands
        // incorrect argument list will be handled by commands
        if (!commands.contains(cmd)) {
            return "Given command <" + cmd + "> doesn't exist! Commands are case sensitive!";
        }
        return commands.at(cmd)->execute(args);
    }

    // COMMANDS \\
    //command map
    std::unordered_map<std::string, command*> commands;

    void register_commands() {
        commands.insert({"COUNT", new count_command(storage_)});
        commands.insert({"PUT", new put_command(storage_)});
        commands.insert({"GET", new get_command(storage_)});
        commands.insert({"DEL", new del_command(storage_)});
    }

};