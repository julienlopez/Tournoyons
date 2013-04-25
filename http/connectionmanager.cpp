#include "connectionmanager.hpp"

#include <boost/bind.hpp>

namespace http {

void ConnectionManager::start(connection_ptr c)
{
	m_connections.insert(c);
	c->start();
}

void ConnectionManager::stop(connection_ptr c)
{
	m_connections.erase(c);
	c->stop();
}

void ConnectionManager::stop_all()
{
	std::for_each(m_connections.begin(), m_connections.end(),
    boost::bind(&Connection::stop, _1));
	m_connections.clear();
}

}
