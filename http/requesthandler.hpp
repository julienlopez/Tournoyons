#ifndef __REQUESTHANDLER_HPP__
#define __REQUESTHANDLER_HPP__

#include <map>
#include <string>
#include <boost/noncopyable.hpp>

namespace http {

struct Reply;
struct Request;

class RequestHandler : private boost::noncopyable
{
public:
	explicit RequestHandler();

	/// Handle a request and produce a reply.
	void handle_request(const Request& req, Reply& rep);

private:
	/// Perform URL-decoding on a string. Returns false if the encoding was
	/// invalid.
	static bool url_decode(const std::string& in, std::string& out);

	static std::map<std::string, std::string> splitArguments(const std::string& arguments);
};

}

#endif // __REQUESTHANDLER_HPP__
