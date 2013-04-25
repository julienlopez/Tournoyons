#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include "connection.hpp"
#include "connectionmanager.hpp"
#include "requesthandler.hpp"

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/system/error_code.hpp>

#include <boost/asio.hpp>

namespace http {

/// The top-level class of the HTTP server.
class Server : private boost::noncopyable
{
public:
	/// Construct the server to listen on the specified TCP address and port
	explicit Server(const std::string& address, const std::string& port);

	/// Run the server's io_service loop.
	void run();

	/// Stop the server.
	void stop();

private:
	/// Handle completion of an asynchronous accept operation.
	void handle_accept(const boost::system::error_code& e);

	/// Handle a request to stop the server.
	void handle_stop();

	/// The io_service used to perform asynchronous operations.
	boost::asio::io_service m_io_service;

	/// Acceptor used to listen for incoming connections.
	boost::asio::ip::tcp::acceptor m_acceptor;

	/// The connection manager which owns all live connections.
	ConnectionManager m_connection_manager;

	/// The next connection to be accepted.
	connection_ptr m_new_connection;

	/// The handler for all incoming requests.
	RequestHandler m_request_handler;
};

}

#endif // __SERVER_HPP__
