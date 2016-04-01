#include <QtCore>
#include <QtNetwork>
#include <QNetworkProxy>
//#include <QtConcurrent/QtConcurrent> //for Qt5
#include "Config.h"

namespace Communication
{
	class SocketClient : public QThread
	{
		Q_OBJECT
		public:
			explicit SocketClient(QObject *parent = 0, QString configfile=0);
		signals:
			void failedToConnect();
			void connected();
			void disconnected();
			void dataReceived(QByteArray);
		public slots:
			bool connectToHost();
			void onConnected();
			void tryToReconnect();
			bool writeData(QByteArray data);
			QByteArray readData();
			bool isConnected();
			void printError();
		private:
			QTcpSocket* socket;
			Config* config;
			bool socketConnected;
	};

	class SocketServer : public QThread
	{
		Q_OBJECT
		public:
			explicit SocketServer(QObject *parent = 0, QString configfile=0);

		public slots:
			void writeData(QTcpSocket*, QByteArray data);
			void broadcast(QByteArray);
			QByteArray readData();

		signals:
			void dataReceived(QTcpSocket*, QByteArray);
			void newSocketConnected(QTcpSocket*);
			void write(QTcpSocket*, QByteArray);

		private slots:
			void onNewConnection();
			void disconnected();

		private:
			QTcpServer* server;
			Config* config;
			QByteArray dataToWrite;
			QList<QTcpSocket *> clientConnections;
			QWaitCondition condition;
	};
}
