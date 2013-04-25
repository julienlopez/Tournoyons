#ifndef __DISPATCHER_HPP__
#define __DISPATCHER_HPP__

#include <utils/singleton.hpp>

#include <string>
#include <map>

class Dispatcher : public Singleton<Dispatcher>
{
	friend class Singleton<Dispatcher>;

	public:
		static void dispatch(const std::string moteur, const std::map<std::string, std::string>& arguments);

	private:
		Dispatcher();
};

#endif // __DISPATCHER_HPP__
