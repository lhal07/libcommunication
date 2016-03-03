#ifndef COMCONFIG_H
#define COMCONFIG_H

#include <QtCore>
#include <Log.h>

#define DEFAULT_CONFIG_FILE "config.xml"

namespace Communication
{
	class Config : public QObject
	{
		public:
			Config(QObject* parent=0, QString f=0);
			~Config(){};
			Log::Log* log();
			void clean();
			void print();
			void load();
			void reload();
			QString getServerIP();
			int getServerPort();
		private:
			//in params
			QObject* parent;
			QString configfile;
			//data
			Log::Log* logging;
			QString serverip;
			int serverport;
	};
}
#endif
