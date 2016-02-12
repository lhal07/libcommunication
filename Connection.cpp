#include "Connection.h"

namespace Communication
{
	Connection::Connection(Command* parent, Config* cfg)
	{
		this->parent = parent;
		this->config = cfg;
		setIP(getServerIP());
		setPort(getServerPort());
		this->connected = false;
		this->firsttime = true;
		eth0 = new NetConfig::EthernetConfig((char*)"eth0");
		this->localIP  = std::string(eth0->getIPAddr());
		this->localMAC = std::string(eth0->getMAC());
		QObject::connect(this,SIGNAL(lostConnection()),this,SLOT(disconnect()));
	}

	bool Connection::isConnected()
	{
		return this->connected;
	}

	std::string Connection::getServerIP()
	{
		this->ip = config->getServerIP().toStdString();
		return(ip);
	}

	int Connection::getServerPort()
	{
		this->port = config->getServerPort();
		return(port);
	}

	std::string Connection::getIP()
	{
		return this->ip;
	}

	void Connection::setIP(std::string value)
	{
		this->ip = value;
	}

	int Connection::getPort()
	{
		return this->port;
	}

	void Connection::setPort(int value)
	{
		this->port = value;
	}

	const Connection& Connection::operator << (const std::string& value) const
	{
		try
		{
			(*server) << value;
		}
		catch(SocketException e)
		{
			config->log()->error("Connection - Could not send Data");
		}
		return *this;
	}

	const Connection& Connection::operator >> (std::string& value) const
	{
		try
		{
			(*server) >> value;
		}
		catch(SocketException e)
		{
			config->log()->error("Connection - Could not get Data");
		}
		return *this;
	}

	ClientSocket* Connection::getServerConnection()
	{
		return this->server;
	}

	void Connection::disconnect()
	{
		if (this->firsttime == false) delete server;
		this->connected = false;
	}

	void Connection::subscribeToServer()
	{
		try
		{
			config->log()->info("Connection - Trying to subscribe to "+getServerIP()+":"+std::to_string(getServerPort()));
			this->localIP  = std::string(eth0->getIPAddr());
			this->localMAC = std::string(eth0->getMAC());
			if (this->firsttime == false) delete server;
			server = new ClientSocket(ip,port);
			(*this) << "subscribe@v#" + this->localMAC + "@v#" + this->localIP;
			this->connected = true;
			this->firsttime = false;
		}
		catch(SocketException e)
		{
			config->log()->alert("Connection - Could not subscribe to Server");
			emit lostConnection();
		}
	}
}
