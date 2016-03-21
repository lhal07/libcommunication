#include "SocketConnection.h"

namespace Communication
{
	SocketClient::SocketClient(QObject *parent, QString configfile) : QThread(parent)
	{
		this->socketConnected = false;
		config = new Config(this,configfile);
		socket = new QTcpSocket(this);
		socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
		QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(printError()));
		QObject::connect(socket, SIGNAL(connected()), this, SLOT(onConnected()));
		QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(tryToReconnect()));
		QObject::connect(this, SIGNAL(failedToConnect()), this, SLOT(tryToReconnect()));
		QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
	}

	bool SocketClient::isConnected()
	{
		return this->socketConnected;
	}

	bool SocketClient::connectToHost()
	{
		bool result = false;
		config->log()->info(config->getServerIP().append(":").append(QString::number(config->getServerPort())).toStdString());
		QHostAddress addr(config->getServerIP());
		socket->connectToHost(addr, (qint16)config->getServerPort());
		if (socket->waitForConnected())
		{
			result = true;
		}
		else
		{
			emit failedToConnect();
			result = false;
		}
		//config->log()->info(std::string("Trying to connect to Host: ")+(result?"Connected!":" Could not connect..."));
		return result;
	}

	void SocketClient::onConnected()
	{
		config->log()->info("Connected!");
		emit connected();
		this->socketConnected = true;
	}

	void SocketClient::tryToReconnect()
	{
		this->socketConnected = false;
		sleep(1);
		emit disconnected();
		connectToHost();
	}

	bool SocketClient::writeData(QByteArray data)
	{
		bool ok = false;
		if(socket->state() == QAbstractSocket::ConnectedState)
		{
			socket->write(data);
			ok = socket->waitForBytesWritten();
		}
		socket->flush();
		return ok;
	}

	QByteArray SocketClient::readData()
	{
		QByteArray data = socket->readLine();
		emit dataReceived(data);
		return data;
	}

	void SocketClient::printError()
	{
		config->log()->error(socket->errorString().toStdString());
	}

	SocketServer::SocketServer(QObject *parent, QString configfile) : QThread(parent)
	{
		this->config = new Config(this,configfile);
		server = new QTcpServer(this);
		connect(server, SIGNAL(newConnection()), SLOT(onNewConnection()));

		bool listening = server->listen(QHostAddress::Any, (qint16)config->getServerPort());
		config->log()->info(std::string(listening?"Listening port ":"Failed to listen port ")+std::to_string(config->getServerPort()));
	}

	void SocketServer::onNewConnection()
	{
		config->log()->info("Got a new Connection!");
		while (server->hasPendingConnections())
		{
			QTcpSocket *socket = server->nextPendingConnection();
			socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
			connect(socket, SIGNAL(disconnected()), SLOT(disconnected()));
			connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
			connect(this, SIGNAL(write(QTcpSocket*,QByteArray)), this, SLOT(sendData(QTcpSocket*,QByteArray)));
			emit newSocketConnected();
			QtConcurrent::run(this,&SocketServer::runNewConnection,socket);
		}
	}

	void SocketServer::runNewConnection(QTcpSocket* client)
	{
		QMutex mutex;
		while(client->isValid())
		{
			if(client->state() == QAbstractSocket::ConnectedState)
			{
				mutex.lock();
				if (condition.wait(&mutex))
				{
					if (!dataToWrite.isEmpty())
						emit write(client,dataToWrite);
				}
				mutex.unlock();
			}
		}
	}

	void SocketServer::sendData(QTcpSocket* client, QByteArray data)
	{
		client->write(data);
	}

	void SocketServer::disconnected()
	{
		QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
		disconnect(socket, SIGNAL(disconnected()));
		disconnect(socket, SIGNAL(readyRead()));
		socket->deleteLater();
	}

	void SocketServer::writeData(QByteArray data)
	{
		dataToWrite = data;
		condition.wakeAll();
	}

	QByteArray SocketServer::readData()
	{
		QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
		if (!socket->isValid()) return "";
		QByteArray data = socket->readLine();
		emit dataReceived(data);
		return data;
	}

}
