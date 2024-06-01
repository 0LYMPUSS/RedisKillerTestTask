#include <asio.hpp>
#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "tcp_server.h"

int main(){

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

    try
    {
        asio::io_context io_context;
        class tcp_server server(io_context);
        io_context.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}