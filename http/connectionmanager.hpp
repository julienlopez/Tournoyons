#ifndef __CONNECTIONMANAGER_HPP__
#define __CONNECTIONMANAGER_HPP__

#include <set>
#include <boost/noncopyable.hpp>
#include "connection.hpp"

namespace http {

class ConnectionManager : private boost::noncopyable
{
public:
	/// Add the specified connection to the manager and start it.
	void start(connection_ptr c);

	/// Stop the specified connection.
	void stop(connection_ptr c);

	/// Stop all connections.
	void stop_all();

private:
	/// The managed connections.
	std::set<connection_ptr> m_connections;
};

}

#endif // __CONNECTIONMANAGER_HPP__
