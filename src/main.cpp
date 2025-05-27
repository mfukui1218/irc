#include "Server.hpp"
#include <cstdlib>
#include <signal.h>

Server* g_server = NULL;

void handleSigint(int sig) {
	(void)sig;
    std::cout << "\n[Server] SIGINT caught. Shutting down..." << std::endl;
	if (g_server) {
		std::vector<Client*> clients = g_server->getClients();
    	for (size_t i = 0; i < clients.size(); ++i) {
        	clients[i]->sendMessage("ERROR :Server is shutting down\r\n");
        	close(clients[i]->getFd());
       		delete clients[i];
    	}
		std::vector<Channel*> channels = g_server->getChannels();
    	for (size_t i = 0; i < channels.size(); ++i) {
        	delete channels[i];
    	}
		close(g_server->getServerFd());
	}
    exit(0);
}

int main(int argc, char* argv[])
{
	int port;
	std::string password;

	signal(SIGINT, handleSigint);
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
		g_server = &server;
		server.start();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	return 0;
}
