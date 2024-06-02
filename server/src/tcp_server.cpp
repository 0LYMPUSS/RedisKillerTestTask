#include "tcp_server.h"

void tcp_server::start_accept()
{
    tcp_connection::pointer new_connection = tcp_connection::create(io_context_, storage_, clients, max_clients);

    acceptor_.async_accept(new_connection->socket(), std::bind(&tcp_server::handle_accept, this,
        new_connection, asio::placeholders::error));
}

void tcp_server::handle_accept(tcp_connection::pointer new_connection,
        const asio::error_code& error)
{
    if (!error)
    {
        new_connection->start();
    }
    start_accept(); 
}

