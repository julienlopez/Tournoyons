#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include <utils/singleton.hpp>

#include <fstream>

class Logger : public Singleton<Logger>
{
	friend class Singleton<Logger>;

public:

	class Log
	{
		public:
			Log(std::ostream& o);
			Log(const Log& log);

			template<typename T> Log& operator<<(T t)
			{
				m_ostream << t;
				return *this;
			}

		private:
			std::ostream& m_ostream;
	};

	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

	static Log out();
	static Log err();

private:
	Logger();

	std::ofstream m_out;
	std::ofstream m_err;
};

#endif // __LOGGER_HPP__

