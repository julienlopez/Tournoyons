#include "requesthandler.hpp"
#include "request.hpp"
#include "reply.hpp"

#include <utils/logger.hpp>
#include <dispatcher.hpp>

#include <fstream>

#include <boost/lexical_cast.hpp>

namespace http {

RequestHandler::RequestHandler()
{}

void RequestHandler::handle_request(const Request& req, Reply& rep)
{
	// Decode url to path.
	std::string request_path;
	if (!url_decode(req.uri, request_path))
	{
		rep = Reply::stock_reply(Reply::bad_request);
		return;
	}

	// Request path must be absolute and not contain "..".
	if (request_path.empty() || request_path[0] != '/' || request_path.find("..") != std::string::npos)
	{
		rep = Reply::stock_reply(Reply::bad_request);
		return;
	}

	if(request_path.front() == '/')
		request_path.erase(0, 1);

	std::size_t pos = request_path.find('?');
	std::string moteur = request_path.substr(0, pos);
	std::string arguments = request_path.substr(pos+1);

	Logger::out() << moteur << " : " << arguments;

	std::map<std::string, std::string> args = splitArguments(arguments);
	try
	{
		Dispatcher::dispatch(moteur, args);
	}
	catch(std::exception& ex)
	{
		Logger::err() << ex.what();
	}
}

bool RequestHandler::url_decode(const std::string& in, std::string& out)
{
	out.clear();
	out.reserve(in.size());
	for (std::size_t i = 0; i < in.size(); ++i)
	{
		if (in[i] == '%')
		{
			if (i + 3 <= in.size())
			{
				int value = 0;
				std::istringstream is(in.substr(i + 1, 2));
				if (is >> std::hex >> value)
				{
					out += static_cast<char>(value);
					i += 2;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else if (in[i] == '+')
		{
			out += ' ';
		}
		else
		{
			out += in[i];
		}
	}
	return true;
}

std::map<std::string, std::string> RequestHandler::splitArguments(const std::string& arguments)
{
	std::map<std::string, std::string> res;
	//TODO RequestHandler::splitArguments()
	return res;
}

}
