#include <asio.hpp>
#include <iostream>
#include <fstream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "tcp_server.h"

int main(int argc, char* argv[]){

    //setting up spdlog to log to a file
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::warn);

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/server_log.txt", true);
    file_sink->set_level(spdlog::level::info);

#ifdef _DEBUG
    console_sink->set_level(spdlog::level::debug);
    file_sink->set_level(spdlog::level::trace);
#endif

    auto logger = spdlog::logger("server", {console_sink, file_sink});

    spdlog::set_default_logger(std::make_shared<spdlog::logger>(logger));
    spdlog::flush_on(spdlog::level::trace);

    // usage server -p <port> -n <num_clients> -f <file-name>

    int port = 13;
    int num_clients = 2;
    std::string filename;
    json data;


    std::unordered_map<std::string, std::string> args{};

    for (int i = 1; i < argc; ++i)
    {
        if(argv[i][0] == '-')
        {
            std::string param = std::string(argv[i]);
            std::string arg = std::string(argv[i + 1]);
            args.insert({param, arg});
        }
    }

    //validate input args
    for (auto& arg : args)
    {
        //port should be a number
        if (arg.first == "-p")
        {
            try {
                port = std::stoi(arg.second);
            } catch (...) {
                std::cout << "Incorrect input format. Port should be a number.\n";
                std::cout << "Usage: server -p <port> -n <num_clients> -f <file-name>\n";
                spdlog::critical("Incorrect input format. Shutting down.");
                return -1;
            }
        }
        //num_of clients should be a number
        if (arg.first == "-n")
        {
            try {
                num_clients = std::stoi(arg.second);
            } catch (...) {
                std::cout << "Incorrect input format. Number of clients should be a number.\n";
                std::cout << "Usage: server -p <port> -n <num_clients> -f <file-name>\n";
                spdlog::critical("Incorrect input format. Shutting down.");
                return -1;
            }
        }
        //file should exist
        if (arg.first == "-f")
        {
            filename = arg.second;
            try {
                std::ifstream f(filename);
                data = json::parse(f);
            }
            catch (...) 
            {
                spdlog::critical("Provided startup file <" + filename + "> doesn't exist. Please provide an existing file.");
            }
        }
    }

    if (!args.contains("-p"))
    {
        spdlog::warn("No port was provided, using default port " + std::to_string(port));
    }

    if (!args.contains("-n"))
    {
        spdlog::warn("No number of clients limit was provided, using default value of " + std::to_string(num_clients));
    }

    tcp_server* server;
    try
    {
        asio::io_context io_context;
        if (!filename.empty())
        {
            server = new tcp_server(io_context, port, num_clients, data);
        } else {
            server = new tcp_server(io_context, port, num_clients);
        }
        io_context.run();

        delete(server);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        delete(server);
    }
    
    return 0;
}