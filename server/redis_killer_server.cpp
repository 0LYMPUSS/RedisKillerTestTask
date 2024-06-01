#include <asio.hpp>
#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "tcp_server.h"

int main(int argc, char* argv[]){

    //setting up spdlog to log to a file
#ifdef _unix_
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
#endif
#ifdef WIN32
    auto console_sink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
#endif
    console_sink->set_level(spdlog::level::warn);

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/server_log.txt", true);
    file_sink->set_level(spdlog::level::info);

#ifdef _DEBUG
    console_sink->set_level(spdlog::level::debug);
    file_sink->set_level(spdlog::level::trace);
#endif

    auto logger = spdlog::logger("new_logger", {console_sink, file_sink});

    spdlog::set_default_logger(std::make_shared<spdlog::logger>(logger));
    spdlog::flush_on(spdlog::level::trace);

    // usage server -p <port> -n <num_clients> -f <file-name>

    int port = 13;
    int num_clients = 2;
    std::string filename = "";


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
                std::cout << "usage server -p <port> -n <num_clients> -f <file-name>\n";
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
                std::cout << "usage server -p <port> -n <num_clients> -f <file-name>\n";
                return -1;
            }
        }
        //file should exist
        if (arg.first == "-f")
        {
            filename = arg.second;
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

    try
    {
        asio::io_context io_context;
        class tcp_server server(io_context, port, num_clients);
        io_context.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}