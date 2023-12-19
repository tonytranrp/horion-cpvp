#include "ServerCommand.h"

ServerCommand::ServerCommand() : IMCCommand("server", "Show information about the server you're playing on", "") {
	registerAlias("ip");
}

ServerCommand::~ServerCommand() {
}

bool ServerCommand::execute(std::vector<std::string>* args) {
	assertTrue(Game.getLocalPlayer());

	if (Game.getRakNetConnector()->isOnAServer())
		clientMessageF("You're currently playing on:\nIP: %s\nPort: %s", Game.getRakNetConnector()->serverIp.c_str(), std::to_string(Game.getRakNetConnector()->serverPort).c_str());
	else
		clientMessageF("%sYou're not playing on a server.", RED);
	return true;
}