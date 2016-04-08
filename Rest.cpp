#include "Rest.h"
#include <cstdlib>
#include <cerrno>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

namespace Communication
{
	Rest::Rest(QObject* parent, Config* cfg)
	{
		this->parent = parent;
		this->config = cfg;
	}

	std::string Rest::POST(std::string url, std::string postargs)
	{
		std::stringstream out;

		try {
			curlpp::Cleanup cleaner;
			curlpp::Easy request;
			curlpp::options::WriteStream ws(&out);

			request.setOpt(ws);
			request.setOpt(new curlpp::options::Url(url));
			request.setOpt(new curlpp::options::Verbose(false));

			std::list<std::string> header;
			header.push_back("Accept: application/json");
			header.push_back("Content-Type: application/json");
			header.push_back("charsets: utf-8");

			request.setOpt(new curlpp::options::HttpHeader(header));

			request.setOpt(new curlpp::options::PostFields(postargs));
			request.setOpt(new curlpp::options::PostFieldSize(postargs.length()));

			request.perform();
		}
		catch ( curlpp::LogicError & e ) {
			throw std::runtime_error(e.what());
		}
		catch ( curlpp::RuntimeError & e ) {
			throw std::runtime_error(e.what());
		}

		 return out.str();
	}

	std::string Rest::GET(std::string url)
	{
		std::stringstream out;

		try {
			curlpp::Cleanup cleaner;
			curlpp::Easy request;
			curlpp::options::WriteStream ws(&out);

			request.setOpt(ws);
			request.setOpt(new curlpp::options::Url(url));
			request.setOpt(new curlpp::options::Verbose(false));

			request.perform();
		}
		catch ( curlpp::LogicError & e ) {
			throw std::runtime_error(e.what());
		}
		catch ( curlpp::RuntimeError & e ) {
			throw std::runtime_error(e.what());
		}

		return out.str();
	}

}
