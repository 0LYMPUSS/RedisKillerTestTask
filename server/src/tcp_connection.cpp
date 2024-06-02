#include "tcp_connection.h"

void tcp_connection::start()
{
    ++clients_;
    if (clients_ > max_clients_) 
    {
        // terminate the connection
        socket_.close();
        
        return;
    }
    spdlog::info("A new client has been connected!");
    // tell the client that connection has been established
    const std::string welcome_msg = "Connection has been established";
    response_msg = welcome_msg;
    //send the msg
    asio::async_write(socket_, asio::buffer(response_msg), 
        std::bind(&tcp_connection::handle_write, shared_from_this(),
        asio::placeholders::error, asio::placeholders::bytes_transferred));
}


void tcp_connection::handle_write(const asio::error_code&, size_t)
{
    // read a msg from client, 
    // then process it and send it back in the handle_read callback
    const char delim = '\n';
    //read the msg
    asio::async_read_until(socket_, message_, delim,
        std::bind(&tcp_connection::handle_read, shared_from_this(),
        asio::placeholders::error, asio::placeholders::bytes_transferred));
}

void tcp_connection::handle_read(const asio::error_code& error, size_t len)
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

std::string tcp_connection::process_message(const std::string &cmd, const std::vector<std::string> &args)
{
    // handle incorrect commands
    // incorrect argument list will be handled by commands
    if (!commands.contains(cmd)) {
        return "Given command <" + cmd + "> doesn't exist! Commands are case sensitive!";
    }
    return commands.at(cmd)->execute(args);
}

void tcp_connection::register_commands()
{
    commands.insert({"COUNT", new count_command(storage_)});
    commands.insert({"PUT", new put_command(storage_)});
    commands.insert({"GET", new get_command(storage_)});
    commands.insert({"DEL", new del_command(storage_)});
    commands.insert({"DUMP", new dump_command(storage_)});
}
