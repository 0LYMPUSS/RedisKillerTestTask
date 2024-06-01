#pragma once
#include <asio.hpp>

#include "tcp_connection.h"
#include "storage.h"

class tcp_server {
private:
    asio::io_context& io_context_;
    asio::ip::tcp::acceptor acceptor_;
    
public:
    tcp_server(asio::io_context& io_context)
        : io_context_(io_context)
        , acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 13))
        {   
            spdlog::info("Server is up");
            start_accept();
        }

private:
    void start_accept()
    {
        tcp_connection::pointer new_connection = tcp_connection::create(io_context_, storage_);

        acceptor_.async_accept(new_connection->socket(), std::bind(&tcp_server::handle_accept, this,
            new_connection, asio::placeholders::error));
    }

    void handle_accept(tcp_connection::pointer new_connection,
        const asio::error_code& error)
    {
        if (!error)
        {
            new_connection->start();
        }
        start_accept(); 
    }

    storage_map<std::string, std::string> storage_;
};