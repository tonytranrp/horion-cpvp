#include "SeedCommand.h"

SeedCommand::SeedCommand() : IMCCommand("seed", "Show the seed of the world you are in", "") {}

bool SeedCommand::execute(std::vector<std::string>* args) {
	auto lp = Game.getLocalPlayer();
	this->assertTrue(lp);

	int64_t result = Game.getLevel()->getLevelData()->get64BitSeed();
	this->clientMessageF("%sSeed: %s %lli", GREEN, GRAY, result);
	return true;
}