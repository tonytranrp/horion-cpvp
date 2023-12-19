#pragma once

#include "../../Memory/GameData.h"
#include "Commands/BindCommand.h"
#include "Commands/BruhCommand.h"
#include "Commands/CommandBlockExploitCommand.h"
#include "Commands/ConfigCommand.h"
#include "Commands/CoordsCommand.h"
#include "Commands/DeviceIDCommand.h"
#include "Commands/DupeCommand.h"
#include "Commands/EjectCommand.h"
#include "Commands/EnchantCommand.h"
#include "Commands/FriendListCommand.h"
#include "Commands/GameModeCommand.h"
#include "Commands/GiveCommand.h"
#include "Commands/GodPotionCommand.h"
#include "Commands/HelpCommand.h"
#include "Commands/HideCommand.h"
#include "Commands/ICommand.h"
#include "Commands/ItemIDCommand.h"
#include "Commands/KitCommand.h"
#include "Commands/LocateCommand.h"
#include "Commands/ModulesCommand.h"
#include "Commands/NbtCommand.h"
#include "Commands/NbtPresetCommand.h"
#include "Commands/PanicCommand.h"
#include "Commands/PathCommand.h"
#include "Commands/PlayerTeleportCommand.h"
#include "Commands/RelativeTeleportCommand.h"
#include "Commands/RenameCommand.h"
#include"Commands/RepairCommand.h"
#include "Commands/SayCommand.h"
#include"Commands/SeedCommand.h"
#include "Commands/ServerCommand.h"
#include "Commands/SetLoreCommand.h"
#include "Commands/SetOffhandCommand.h"
#include "Commands/SetprefixCommand.h"
#include "Commands/SignEditCommand.h"
#include "Commands/SpammerCommand.h"
#include "Commands/TeleportCommand.h"
#include "Commands/ToggleCommand.h"
#include "Commands/TopCommand.h"
#include "Commands/UnbindCommand.h"
#include "Commands/WaypointCommand.h"
#include"Commands/XpCommand.h"

#ifdef HORION_DEBUG
#include "Commands/BlockESPCommand.h"
#include "Commands/TestCommand.h"
#include "Commands/UnlockAchievementsCommand.h"
#include "Commands/EmoteCommand.h"
#include "Commands/NameSpoofCommand.h"
#include "Commands/DamageCommand.h"
#endif

class CommandMgr {
private:
	GameData* gameData;
	std::vector<IMCCommand*> commandList;

public:
	CommandMgr(GameData* gm);
	~CommandMgr();

	char prefix = '.';

	void initCommands();
	void disable();
	std::vector<IMCCommand*>* getCommandList();

	void execute(char* message);
};

extern CommandMgr* cmdMgr;
