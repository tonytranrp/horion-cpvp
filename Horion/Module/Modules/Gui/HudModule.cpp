#include "HudModule.h"

#include "../../../DrawUtils.h"

HudModule::HudModule() : IModule(0, Category::GUI, "Displays HUD Elements") {
	font.addEntry(EnumEntry("Smooth", 0));
	font.addEntry(EnumEntry("Mojangles", 1));
	//font.addEntry(EnumEntry("Unicode", 2)); weird
	//font.addEntry(EnumEntry("Galactic", 3)); weird
	font.addEntry(EnumEntry("Ten", 2));
	registerEnumSetting("Font", &font, 0);
	registerBoolSetting("Show FPS", &this->fps, this->fps);
	registerBoolSetting("Show CPS", &this->cps, this->cps);
	registerBoolSetting("Show Time", &this->MinecraftTime, this->MinecraftTime);
	registerBoolSetting("Item Durability", &this->currentItemDurability, this->currentItemDurability);
	registerBoolSetting("Coordinates", &this->coordinates, this->coordinates);
	registerBoolSetting("Show Server IP", &this->serverInfo, this->serverInfo);
	registerBoolSetting("Always Show", &this->alwaysShow, this->alwaysShow);
	registerFloatSetting("Scale", &this->scale, this->scale, 0.5f, 1.5f);
}

HudModule::~HudModule() {
}

const char* HudModule::getModuleName() {
	return ("HUD");
}

void HudModule::onPostRender(MinecraftUIRenderContext* renderCtx) {
	Vec2 windowSize = Game.getGuiData()->windowSize;
	LocalPlayer* player = Game.getLocalPlayer();
	float f = 10.f * this->scale;
	float startY = windowSize.y - (f * 4);

	if (!Game.getLocalPlayer()) return;

	{  // Coordinates
		if (coordinates) {
			Vec3* pos = Game.getLocalPlayer()->getPos();
			int curDim = player->getDimensionId();
			Vec2 textPos = Vec2(1.f, startY + 28.f * scale + 1.f);
			std::string coordsX;
			std::string coordsY;
			std::string coordsZ;
			std::string NcoordsX;
			std::string NcoordsZ;

			switch (curDim) {
			default:
				break;
			case 0:
			case 2: {  //overworld and end // /8
				coordsX = std::to_string((int)floorf(pos->x)) + ",";
				coordsY = std::to_string((int)floorf(pos->y - 1.f)) + ",";
				coordsZ = std::to_string((int)floorf(pos->z));
				NcoordsX = std::to_string((int)floorf(pos->x + 0.001f) / 8) + ",";
				NcoordsZ = std::to_string((int)floorf(pos->z + 0.001f) / 8);

				std::string coordsText = "Position: " + coordsX + " " + coordsY + " " + coordsZ;
				std::string NcoordsText = "Nether: " + NcoordsX + " " + coordsY + " " + NcoordsZ;
				DrawUtils::drawText(textPos, &NcoordsText, MC_Color(255, 255, 255), scale, 1.f, DesiredUi::HUD);
				textPos.y -= f;
				startY -= f;
				DrawUtils::drawText(textPos, &coordsText, MC_Color(255, 255, 255), scale, 1.f, DesiredUi::HUD);
				startY -= f;
			} break;
			case 1: {  //nether // *8
				coordsX = std::to_string((int)floorf(pos->x + 0.001f) * 8) + ",";
				coordsY = std::to_string((int)floorf(pos->y - 1.f)) + ",";
				coordsZ = std::to_string((int)floorf(pos->z + 0.001f) * 8);
				NcoordsX = std::to_string((int)floorf(pos->x)) + ",";
				NcoordsZ = std::to_string((int)floorf(pos->z));

				std::string NcoordsText = "Position: " + NcoordsX + " " + coordsY + " " + NcoordsZ;
				std::string coordsText = "Overworld: " + coordsX + " " + coordsY + " " + coordsZ;
				DrawUtils::drawText(textPos, &coordsText, MC_Color(255, 255, 255), scale, 1.f, DesiredUi::HUD);
				textPos.y -= f;
				startY -= f;
				DrawUtils::drawText(textPos, &NcoordsText, MC_Color(255, 255, 255), scale, 1.f, DesiredUi::HUD);
				startY -= f;
			} break;
			}
		}
	}
	{  //ServerInfo
		if (serverInfo) {
			if (!(!Game.getLocalPlayer() || !GameData::canUseMoveKeys())) {
				if (Game.getLocalPlayer() || Game.getRakNetConnector()->isOnAServer()) {
					auto Server = Game.getRakNetConnector();
					if (!Server) return;

					std::string IP = Server->serverIp.c_str();
					std::string Port = std::to_string(Server->serverPort).c_str();

					if (Server->serverIp.size() < 1) {
						IP = "Local World";
					}

					std::string PortText = "Port: " + Port;
					DrawUtils::drawText(Vec2(1.f, startY + 28.f * scale + 1.f), &PortText, MC_Color(255, 255, 255), scale, 1.f, DesiredUi::HUD);
					startY -= f;

					std::string IPtext = "IP: " + IP;
					DrawUtils::drawText(Vec2(1.f, startY + 28.f * scale + 1.f), &IPtext, MC_Color(255, 255, 255), scale, 1.f, DesiredUi::HUD);
					startY -= f;
				}
			}
		}
	}
	{  //item durability
		if (currentItemDurability) {
			LocalPlayer* plr = Game.getLocalPlayer();
			PlayerInventoryProxy* supplies = Game.getSupplies();
			Inventory* inv = Game.getInventory();
			ItemStack* stack = inv->getItemStack(supplies->selectedHotbarSlot);
			std::string RealDurabilityThing;
			if (stack->item)
				RealDurabilityThing = "Durability: " + std::to_string(stack->getDurability()) + "/" + std::to_string(stack->getMaxDurability());
			else
				RealDurabilityThing = "Durability: 0/0";
			DrawUtils::drawText(Vec2(1.f, startY + 28.f * scale + 1.f), &RealDurabilityThing, MC_Color(255, 255, 255), scale, 1.f, DesiredUi::HUD);
			startY -= f;
		}
	}
	{  //Minecraft Time
		if (MinecraftTime) {
			std::string CurrentGameTime = "";
			int ThingWeNeed = (int)(timeOfDay * 24);
			if (ThingWeNeed > 12)
				ThingWeNeed -= 12;
			else
				ThingWeNeed += 12;

			if (ThingWeNeed < 10)
				CurrentGameTime = "Time: 0" + std::to_string(ThingWeNeed) + ":00";
			else
				CurrentGameTime = "Time: " + std::to_string(ThingWeNeed) + ":00";
			DrawUtils::drawText(Vec2(1.f, startY + 28.f * scale + 1.f), &CurrentGameTime, MC_Color(255, 255, 255), scale, 1.f, DesiredUi::HUD);
			startY -= f;
		}
	}
	{  // CPS
		if (cps) {
			std::string cpsText = "CPS: " + std::to_string(Game.getLeftCPS()) + " - " + std::to_string(Game.getRightCPS());;
			DrawUtils::drawText(Vec2(1.f, startY + 28.f * scale + 1.f), &cpsText, MC_Color(255, 255, 255), scale, 1.f, DesiredUi::HUD);

			startY -= f;
		}
	}
	{  // FPS
		if (fps) {
			std::string fpsText = "FPS: " + std::to_string(Game.getFPS());
			DrawUtils::drawText(Vec2(1.f, startY + 28.f * scale + 1.f), &fpsText, MC_Color(255, 255, 255), scale, 1.f, DesiredUi::HUD);

			startY -= f;
		}
	}
}