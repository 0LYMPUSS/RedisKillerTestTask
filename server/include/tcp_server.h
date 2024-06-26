#pragma once
#include <asio.hpp>

#include "tcp_connection.h"
#include "storage.h"

class tcp_server {
private:
    asio::io_context& io_context_;
    asio::ip::tcp::acceptor acceptor_;
    
public:
    tcp_server(asio::io_context& io_context, const asio::ip::port_type port, const int num_clients)
        : io_context_(io_context)
        , acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
        , max_clients(num_clients)
        {   
            spdlog::info("Server is up");
            start_accept();
        }

    tcp_server(asio::io_context& io_context, const asio::ip::port_type port, const int num_clients, const json& data)
        : io_context_(io_context)
        , acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
        , max_clients(num_clients)
        {   
            storage_.deserialize(data);
            spdlog::info("Server is up");
            start_accept();
        }

private:

    void start_accept();

    void handle_accept(tcp_connection::pointer new_connection,
        const asio::error_code& error);

    storage_map<std::string, std::string> storage_;
    int max_clients;
    int clients = 0;
};