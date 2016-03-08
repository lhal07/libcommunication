#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>
#include <QtCore>
#include "Config.h"

#include "netcfg/netconfig.h"
#include "ss++/Socket.h"
#include "ss++/ClientSocket.h"
#include "ss++/SocketException.h"

namespace Communication
{
	class Command;
	class Config;
	class Connection : public QObject
	{
		Q_OBJECT //Enable slots and signals
		public:
			Connection(QObject* parent=0, QString configfile=0);
			~Connection(){};
			const Connection& operator << (const std::string& value) const;
			const Connection& operator >> (std::string& value) const;
		private:
			QObject* parent;
			Config* config;
			std::string ip;
			int  port;
			std::string localIP;
			std::string localMAC;
			int  started;
			bool connected;
			bool firsttime;
			ClientSocket* server;
			NetConfig::EthernetConfig *eth0;
		signals:
			void lostConnection();
		public slots:
			std::string getServerIP();
			int getServerPort();
			bool isConnected();
			std::string getIP();
			void setIP(std::string);
			int getPort();
			void setPort(int);
			ClientSocket* getServerConnection();
			void subscribeToServer();
		private slots:
			void disconnect();
	};
}
#endif
