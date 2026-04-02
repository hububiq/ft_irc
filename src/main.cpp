#include "Validate.hpp"
#include "Server.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    try {
        Server server(argc, argv);
        server.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}