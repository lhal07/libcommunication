#include "Command.h"

namespace Communication
{
	Command::Command(QObject* parent, QString configfile)
	{
		this->parent = parent;
		this->config = new Config(this,configfile);
		this->server = new Connection(this,config);

		QObject::connect(this,SIGNAL(startMoveEvaluation(QString)),parent,SLOT(startMonitoring(QString)));
		QObject::connect(this,SIGNAL(finalizeMoveEvaluation(QString)),parent,SLOT(finalizeMonitoring(QString)));
	}

	void Command::Reconnect()
	{
		config->reload();
		server->setIP(server->getServerIP());
		server->setPort(server->getServerPort());
		server->subscribeToServer();
	}

	bool Command::isSubscribed()
	{
		return this->subscribed;
	}

	void Command::receiveCommands()
	{
		QtConcurrent::run(this,&Command::waitForCommand);
	}

	void Command::answerCommand(QString json)
	{
		std::string answer = std::string(EVAL_FLAG) + "started@v#" + json.toStdString();
		try
		{
			if (this->subscribed)
			{
				(*(server)) << answer;
			}
		}
		catch(std::exception &e)
		{
			config->log()->error("Command - Error on sending Command answer!");
		}
	}

	void Command::waitForCommand()
	{
		std::string reply;
		bool connected = false;
		while(true)
		{
			try
			{
				connected = server->isConnected();
				if (connected == true) config->log()->info("Command - Connected!");
				else throw std::runtime_error("Not connected!");

				config->log()->info("Command - Waiting communication from server...");
				reply = "";
				//if (server->read(reply)) continue;
				(*(server)) >> reply;
				if (reply.compare("subscribed@v#true") == 0)
				{
					config->log()->info("Command - Subscribed!");
					this->subscribed = true;
					continue;
				}
				if(!this->subscribed) continue;
				if (reply.find(EVAL_FLAG,0) != std::string::npos){
					config->log()->info("Command - Command received!");
					if (reply.find(START_FLAG,sizeof(EVAL_FLAG)-1) != std::string::npos)
					{
						int offset = sizeof(START_FLAG)-1+sizeof(EVAL_FLAG)-1;
						emit startMoveEvaluation(QString::fromStdString(reply.substr(offset,reply.length()-offset)));
						continue;
					}
					if (reply.find(STOP_FLAG,sizeof(EVAL_FLAG)-1) != std::string::npos)
					{
						int offset = sizeof(STOP_FLAG)-1+sizeof(EVAL_FLAG)-1;
						emit finalizeMoveEvaluation(QString::fromStdString(reply.substr(offset,reply.length()-offset)));
						continue;
					}
				}
			}
			catch(std::exception &e)
			{
				if (!server->isConnected())
				{
					config->log()->warning("Command - Not connected");
					this->subscribed = false;
					sleep(5);
					Reconnect();
				}
			}
		}
	}
}
