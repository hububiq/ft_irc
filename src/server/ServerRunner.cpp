#include "ServerRunner.hpp"

#include "CommandHandler.hpp"
#include "Executor.hpp"
#include "JoinGatekeeper.hpp"
#include "Listener.hpp"
#include "MessageParser.hpp"
#include "ModeReporter.hpp"
#include "Multiplexer.hpp"
#include "Validator.hpp"

extern volatile sig_atomic_t g_running;

std::string ServerRunner::parse_pwd(char *arg)
{
	std::string pwd(arg ? arg : "");
	if (pwd.empty())
	{
		throw std::out_of_range("Password must contain at least 1 letter");
	}
	return pwd;
}

long ServerRunner::parse_port(char *arg)
{
	if (!arg)
		throw std::invalid_argument("Port argument missing");
	char *endptr = NULL;
	long  port = std::strtol(arg, &endptr, 10);
	if (*endptr != '\0' || port <= 1024 || port > 65535)
	{
		throw std::invalid_argument("Invalid port number");
	}
	return port;
}

ServerRunner::ServerRunner(int argc, char **argv)
{
	if (argc != 3)
	{
		throw std::invalid_argument("Please use format: ./irc <port> "
		                            "<password>");
	}

	long        port = parse_port(argv[1]);
	uint16_t    port_num = htons((uint16_t) port);
	std::string pwd = parse_pwd(argv[2]);
	uint32_t    host_ip = inet_addr("0.0.0.0");
	int         socket_fd = m_Listener.init_socket(host_ip, port_num);
	int         epoll_fd;
	try
	{
		epoll_fd = m_epollInitializer.init_epoll(socket_fd);
	}
	catch (...)
	{
		close(socket_fd);
		throw;
	}

	m_server = new ServerDao(static_cast<int>(port), port_num, host_ip, pwd, socket_fd, epoll_fd);

	EpollStateManager *stateManager = new EpollStateManager(m_server);
	m_Multiplexer = new Multiplexer(m_server, new ConnHandler(m_server), new RequestHandler(stateManager, new Executor(new CommandHandler(m_server, new Validator(), new JoinGatekeeper(), new ModeReporter(m_server))), new MessageParser()), stateManager);
}

/*
m_server = new ServerDao(static_cast<int>(port), port_num, host_ip, pwd,
socket_fd, epoll_fd);

  EpollStateManager *stateManager = new EpollStateManager(m_server);
  m_Multiplexer = new Multiplexer(
    m_server,
    new ConnHandler(m_server),
    new RequestHandler(
      stateManager,
      new Executor(
        new CommandHandler(
          m_server,
          new Validator(),
          new JoinGatekeeper(),
          new ModeReporter(m_server)
        )
      ),
      new MessageParser();
    ),
    stateManager
  );
*/

ServerRunner::~ServerRunner()
{
	if (m_Multiplexer)
		delete m_Multiplexer;

	if (m_server)
	{
		delete m_server;
		m_server = NULL;
	}
}

void ServerRunner::run()
{
	m_Multiplexer->loop_epoll();
}
