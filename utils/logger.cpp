#include "logger.hpp"

#include <ctime>
#include "boost/date_time/posix_time/posix_time.hpp"

Logger::Log::Log(std::ostream& o): m_ostream(o)
{
	boost::posix_time::ptime t(boost::posix_time::second_clock::local_time());
	m_ostream << std::endl << t << ": ";
}

Logger::Log::Log(const Log& log): m_ostream(log.m_ostream)
{}

Logger::Logger(): m_out("out.log", std::ofstream::app), m_err("err.log", std::ofstream::app)
{

}

Logger::Log Logger::out()
{
	return Log(instance().m_out);
}

Logger::Log Logger::err()
{
	return Log(instance().m_err);
}
