#include "Server.hpp"

int main(int argc, char* argv[])
{
	int port;
	std::string password;

	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
	}
	port = std::atoi(argv[1]);
	password = argv[2];
	try
	{
		Server server(port, password);
		server.start();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	return 0;
}
