#ifndef __CONNECTION_HPP__
#define __CONNECTION_HPP__

#include "reply.hpp"
#include "request.hpp"
#include "requestparser.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <boost/asio.hpp>
#include <boost/array.hpp>

namespace http {

class ConnectionManager;
class RequestHandler;

class Connection : public boost::enable_shared_from_this<Connection>, private boost::noncopyable
{
public:
	/// Construct a connection with the given io_service.
	explicit Connection(boost::asio::io_service& io_service, ConnectionManager& manager, RequestHandler& handler);

	/// Get the socket associated with the connection.
	boost::asio::ip::tcp::socket& socket();

	/// Start the first asynchronous operation for the connection.
	void start();

	/// Stop all asynchronous operations associated with the connection.
	void stop();

private:
	/// Handle completion of a read operation.
	void handle_read(const boost::system::error_code& e, std::size_t bytes_transferred);

	/// Handle completion of a write operation.
	void handle_write(const boost::system::error_code& e);

	/// Socket for the connection.
	boost::asio::ip::tcp::socket m_socket;

	/// The manager for this connection.
	ConnectionManager& m_connection_manager;

	/// The handler used to process the incoming request.
	RequestHandler& m_request_handler;

	/// Buffer for incoming data.
	boost::array<char, 8192> m_buffer;

	/// The incoming request.
	Request m_request;

	/// The parser for the incoming request.
	RequestParser m_request_parser;

	/// The reply to be sent back to the client.
	Reply m_reply;
};

typedef boost::shared_ptr<Connection> connection_ptr;

}

#endif // __CONNECTION_HPP__
