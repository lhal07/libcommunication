#include "Config.h"

/* Boost C++ includes */
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
namespace pt = boost::property_tree;


namespace Communication
{
	Config::Config(QObject* parent, QString f)
	{
		this->parent = parent;
		this->configfile = f;

		if (configfile.isNull()) configfile = DEFAULT_CONFIG_FILE;
		this->logging = new Log::Log(this,configfile,"Communication");

		this->load();
	}

	Log::Log* Config::log()
	{
		return(logging);
	}

	void Config::reload()
	{
		this->load();
	}

	void Config::clean()
	{
		serverip = "0.0.0.0";
		serverport = 0;
	}

	void Config::print()
	{
		std::cout << " Connection" << std::endl;
		std::cout << "  ServerIP: ";
		std::cout << this->serverip.toStdString() << std::endl;
		std::cout << "  ServerPort: ";
		std::cout << this->serverport << std::endl;
	}

	void Config::load()
	{
		QFileInfo checkFile(configfile);
		if (!checkFile.exists() || !checkFile.isFile()) return;

		/* Removes previous rules */
		this->clean();

		/* Create empty property tree object */
		pt::ptree tree;

		/* Parse the XML into the property tree. */
		pt::ptree cfg;
		try{
			pt::read_xml(configfile.toStdString(), tree);
			cfg = tree.get_child("Config");
		}
		catch(boost::exception const&  ex)
		{
			this->log()->error("Failed to Read "+configfile.toStdString());
			return;
		}

		pt::ptree con;
		try{
			con = cfg.get_child("Connection");
		}
		catch(boost::exception const&  ex){}

		try{
			this->serverip = con.get<std::string>("ServerIP").c_str();
			this->serverport = con.get<int>("ServerPort");
		}
		catch(boost::exception const&  ex){}

		return;
	}

	QString Config::getServerIP()
	{
		return(serverip);
	}

	int Config::getServerPort()
	{
		return(serverport);
	}
}
