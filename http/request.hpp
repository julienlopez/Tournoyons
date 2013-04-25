#ifndef __REQUEST_HPP__
#define __REQUEST_HPP__

#include "header.hpp"

#include <vector>

namespace http {

struct Request
{
	std::string method;
	std::string uri;
	int http_version_major;
	int http_version_minor;
	std::vector<header> headers;
};

}

#endif // __REQUEST_HPP__
