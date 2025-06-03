#include "Server.hpp"
#include <cstdlib>
#include <signal.h>

Server* g_server = NULL;
volatile sig_atomic_t g_stop = 0;

void handleSigint(int sig) {
	(void)sig;
	g_stop = 1;
}

int main(int argc, char* argv[])
{
	int port;
	std::string password;
	struct sigaction sa;

	sa.sa_handler = handleSigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
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
