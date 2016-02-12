#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <QtCore>
#include "Config.h"
#include "Connection.h"

#define EVAL_FLAG "evaluation@v#"
#define START_FLAG "start@v#"
#define STOP_FLAG "stop@v#"

namespace Communication
{
	class Config;
	class Connection;
	class Command : public QObject
	{
		Q_OBJECT //Enable slots and signals
		public:
			Command(QObject* parent=0, QString configfile=0);
			~Command(){};
		private:
			QObject* parent;
			Config* config;
			Connection* server;
			QThread* tConnection;
			bool subscribed;
			void Reconnect();
			void waitForCommand();
		protected:
			void run();
			void exec();
		signals:
			void startMoveEvaluation(QString);
			void finalizeMoveEvaluation(QString);
		public slots:
			bool isSubscribed();
			void receiveCommands();
			void answerCommand(QString);
	};
}
#endif
