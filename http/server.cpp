#include "server.hpp"

#include <boost/bind.hpp>

using namespace std;

namespace http {

Server::Server(const std::string& address, const std::string& port):
	m_io_service(), m_acceptor(m_io_service), m_connection_manager(),
    m_new_connection(new Connection(m_io_service, m_connection_manager, m_request_handler)),
    m_request_handler()
{
	// Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
	boost::asio::ip::tcp::resolver resolver(m_io_service);
	boost::asio::ip::tcp::resolver::query query(address, port);
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);

	m_acceptor.open(endpoint.protocol());
	m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	m_acceptor.bind(endpoint);
	m_acceptor.listen();
	m_acceptor.async_accept(m_new_connection->socket(), boost::bind(&Server::handle_accept, this, boost::asio::placeholders::error));
}

void Server::run()
{
	// The io_service::run() call will block until all asynchronous operations
	// have finished. While the server is running, there is always at least one
	// asynchronous operation outstanding: the asynchronous accept call waiting
	// for new incoming connections.
	m_io_service.run();
}

void Server::stop()
{
	// Post a call to the stop function so that server::stop() is safe to call
	// from any thread.
	m_io_service.post(boost::bind(&Server::handle_stop, this));
}

void Server::handle_accept(const boost::system::error_code& e)
{
	if(!e)
	{
		m_connection_manager.start(m_new_connection);
		m_new_connection.reset(new Connection(m_io_service, m_connection_manager, m_request_handler));
		m_acceptor.async_accept(m_new_connection->socket(), boost::bind(&Server::handle_accept, this, boost::asio::placeholders::error));
	}
}

void Server::handle_stop()
{
	// The server is stopped by cancelling all outstanding asynchronous
	// operations. Once all operations have finished the io_service::run() call
	// will exit.
	m_acceptor.close();
	m_connection_manager.stop_all();
}


}
