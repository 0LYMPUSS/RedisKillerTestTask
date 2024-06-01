#include <asio.hpp>
#include <iostream>

using asio::ip::tcp;

int main(int argc, char* argv[]){
    
    try
    {
        if (argc != 3) 
        {
            std::cerr << "Usage: client <host> <port>" << std::endl;
            return 1;
        }
        asio::io_context io_context;

        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve(asio::ip::tcp::v4(), argv[1], argv[2]);

        tcp::socket socket(io_context);
        asio::connect(socket, endpoints);

        for(;;) {
            std::array<char, 128> buf;
            asio::error_code error;
            std::string command;

            //read a command from user
            std::getline(std::cin, command);

            //send message to server
            socket.send(asio::buffer(command + '\n'));

            // recieve msg from client
            size_t len = socket.read_some(asio::buffer(buf), error);

            if (error == asio::error::eof)
            {
                ;
                break;
            }
            else if (error)
            {
                throw asio::system_error(error);
            }
            
            // print response from client
            std::cout.write(buf.data(), len);
            std::cout << '\n';
        }

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

    return 0;
}