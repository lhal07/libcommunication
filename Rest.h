#ifndef REST_H
#define REST_H

#include <QtCore>
#include <string>
#include <exception>
#include "Config.h"

namespace Communication
{
	class Config;
	class Rest : public QObject
	{
		Q_OBJECT //Enable slots and signals
		public:
			Rest(QObject* parent=0, Config* cfg=0);
			~Rest(){};
			std::string POST(std::string url, std::string postargs);
			std::string GET(std::string url);
		private:
			QObject* parent;
			Config* config;
	};
}
#endif
