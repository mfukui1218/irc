#include "Server.hpp"
#include <cstdlib>
#include <signal.h>

volatile sig_atomic_t g_stop = 0;
int g_pipefd[2];

void signalHandler(int sig)
{
	(void)sig;
	char c = 'x';
	write(g_pipefd[1], &c, 1);
	g_stop = 1; 
}

int main(int argc, char* argv[])
{
	int port;
	std::string password;
	struct sigaction sa;

	sa.sa_handler = signalHandler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	signal(SIGHUP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
	}
	if (pipe(g_pipefd) == -1) {
	std::cerr << "Failed to create pipe" << std::endl;
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
