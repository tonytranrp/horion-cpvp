#include "CommandMgr.h"

CommandMgr::CommandMgr(GameData* gm) {
	this->gameData = gm;
}

CommandMgr::~CommandMgr() {
	for (int i = 0; i < commandList.size(); i++) {
		delete commandList[i];
		commandList[i] = nullptr;
	}
}

void CommandMgr::initCommands() {
	logF("Initializing commands");

	commandList.push_back(new BindCommand());
	commandList.push_back(new BruhCommand());
	commandList.push_back(new CommandBlockExploitCommand());
	commandList.push_back(new ConfigCommand());
	commandList.push_back(new CoordsCommand());
	commandList.push_back(new DeviceIDCommand());
	commandList.push_back(new DupeCommand());
	commandList.push_back(new EjectCommand());
	commandList.push_back(new EnchantCommand());
	commandList.push_back(new FriendListCommand());
	commandList.push_back(new GameModeCommand());
	commandList.push_back(new GiveCommand());
	commandList.push_back(new GodPotionCommand());
	commandList.push_back(new HelpCommand());
	commandList.push_back(new HideCommand());
	commandList.push_back(new ItemIDCommand());
	commandList.push_back(new KitCommand());
	commandList.push_back(new LocateCommand());
	commandList.push_back(new ModulesCommand());
	commandList.push_back(new NbtCommand());
	commandList.push_back(new NbtPresetCommand());
	commandList.push_back(new PanicCommand());
	commandList.push_back(new PathCommand());
	commandList.push_back(new RelativeTeleportCommand());
	commandList.push_back(new RenameCommand());
	commandList.push_back(new RepairCommand());
	commandList.push_back(new SayCommand());
	commandList.push_back(new SeedCommand());
	commandList.push_back(new ServerCommand());
	commandList.push_back(new SetLoreCommand());
	commandList.push_back(new SetOffhandCommand());
	commandList.push_back(new SetprefixCommand());
	commandList.push_back(new SpammerCommand());
	commandList.push_back(new TeleportCommand());
	commandList.push_back(new ToggleCommand());
	commandList.push_back(new TopCommand());
	commandList.push_back(new UnbindCommand());
	commandList.push_back(new WaypointCommand());
	commandList.push_back(new XpCommand());

#ifdef HORION_DEBUG
	commandList.push_back(new TestCommand());
	commandList.push_back(new BlockESPCommand());
	commandList.push_back(new UnlockAchievementsCommand());
	commandList.push_back(new SignEditCommand());
	commandList.push_back(new EmoteCommand());
	commandList.push_back(new NameSpoofCommand());
	commandList.push_back(new DamageCommand());
#endif
}

void CommandMgr::disable() {
}

std::vector<IMCCommand*>* CommandMgr::getCommandList() {
	return &commandList;
}

void CommandMgr::execute(char* message) {
	if (message) {
		std::vector<std::string> args;
		std::string msgStr = message + 1;
		size_t pos = msgStr.find(" "), initialPos = 0;
		while (pos != std::string::npos) {
			args.push_back(msgStr.substr(initialPos, pos - initialPos));
			initialPos = pos + 1;

			pos = msgStr.find(" ", initialPos);
		}
		args.push_back(msgStr.substr(initialPos, std::min(pos, msgStr.size()) - initialPos + 1));
		
		std::transform(args[0].begin(), args[0].end(), args[0].begin(), ::tolower);
		std::string cmd = args[0];

		for (auto it = this->commandList.begin(); it != this->commandList.end(); ++it) {
			IMCCommand* c = *it;
			auto* aliases = c->getAliasList();
			for (auto it = aliases->begin(); it != aliases->end(); ++it) {
				if (*it == cmd) {
					try {
						if (!c->execute(&args))
							Game.getGuiData()->displayClientMessageF("%s%sUsage: %s%c%s %s", RED, BOLD, RESET, cmdMgr->prefix, c->getCommand(), c->getUsage(cmd.c_str()));
					} catch (...) {
						Game.getGuiData()->displayClientMessageF("%s%sUsage: %s%c%s %s", RED, BOLD, RESET, cmdMgr->prefix, c->getCommand(), c->getUsage(cmd.c_str()));
					}
					return;
				}
			}
		}

		Game.getGuiData()->displayClientMessageF("%s[%sHorion%s] - %sCommand '%s%s%s' could not be found!", GRAY, AQUA, GRAY, RESET, GRAY, cmd.c_str(), RESET);
	}
}

CommandMgr* cmdMgr = new CommandMgr(&Game);
