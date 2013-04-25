#include "connection.hpp"
#include "connectionmanager.hpp"
#include "requesthandler.hpp"

#include <boost/bind.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>

namespace http {

Connection::Connection(boost::asio::io_service& io_service, ConnectionManager& manager, RequestHandler& handler):
	m_socket(io_service), m_connection_manager(manager), m_request_handler(handler)
{}

boost::asio::ip::tcp::socket& Connection::socket()
{
  return m_socket;
}

void Connection::start()
{
	m_socket.async_read_some(boost::asio::buffer(m_buffer), boost::bind(&Connection::handle_read, shared_from_this(),
        boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Connection::stop()
{
	m_socket.close();
}

void Connection::handle_read(const boost::system::error_code& e, std::size_t bytes_transferred)
{
	if(!e)
	{
		boost::tribool result;
		boost::tie(result, boost::tuples::ignore) = m_request_parser.parse(m_request, m_buffer.data(), m_buffer.data() + bytes_transferred);

		if(result)
		{
			m_request_handler.handle_request(m_request, m_reply);
			boost::asio::async_write(m_socket, m_reply.to_buffers(), boost::bind(&Connection::handle_write, shared_from_this(), boost::asio::placeholders::error));
		}
		else if(!result)
		{
			m_reply = Reply::stock_reply(Reply::bad_request);
			boost::asio::async_write(m_socket, m_reply.to_buffers(), boost::bind(&Connection::handle_write, shared_from_this(), boost::asio::placeholders::error));
		}
		else
		{
			m_socket.async_read_some(boost::asio::buffer(m_buffer),
				boost::bind(&Connection::handle_read, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
		}
	}
	else if (e != boost::asio::error::operation_aborted)
	{
		m_connection_manager.stop(shared_from_this());
	}
}

void Connection::handle_write(const boost::system::error_code& e)
{
	if(!e)
	{
		// Initiate graceful connection closure.
		boost::system::error_code ignored_ec;
		m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
	}

	if (e != boost::asio::error::operation_aborted)
	{
		m_connection_manager.stop(shared_from_this());
	}
}


}
