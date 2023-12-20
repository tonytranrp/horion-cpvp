#include "CrystalAura.h"
#include <unordered_set>
CrystalAura::CrystalAura() : IModule(0, Category::COMBAT, "p100 java CA by JohnTWD // FatMC#1209") {
	registerIntSetting("Delay (ticks)", &this->delay, this->delay, 0, 25);

	registerIntSetting("Max Proximity", &this->maxProximity, this->maxProximity, 1, 8);
	registerFloatSetting("Enemy Range", &this->range, this->range, 5, 15);
	registerFloatSetting("Place Range", &this->placeRange, this->placeRange, 0.f, 12.f);
	priority = SettingEnum(this)
				   .addEntry(EnumEntry("Distance", 0))
				   .addEntry(EnumEntry("Health", 1));
	registerEnumSetting("Priority", &this->priority, 0);
	registerFloatSetting("Thru Walls", &this->thruWallsR, this->thruWallsR, 0, 10);
	registerFloatSetting("Post Walls", &this->postWallsR, this->postWallsR, 0.f, 10.f);

	registerBoolSetting("Safety First!", &this->safetyFirst, this->safetyFirst);
	registerFloatSetting("Minimum Health", &this->minHealth, this->minHealth, 0.f, 20.f);
	registerFloatSetting("Max Self Damage", &this->maxSelfDmg, this->maxSelfDmg, 0.f, 20.f);
	registerFloatSetting("Min Enem Damage", &this->minEnemDmg, this->minEnemDmg, 0.f, 20.f);

	facePlaceType = SettingEnum(this)
						.addEntry(EnumEntry("Smart", 0))
						.addEntry(EnumEntry("None", 1));
	registerEnumSetting("Faceplace Type", &this->facePlaceType, 0);
	registerFloatSetting("FP Threshold", &this->fpThresh, this->fpThresh, 0.f, 20.f);
	registerFloatSetting("FP MinimumDmg", &this->dmgAtThresh, this->dmgAtThresh, 0.f, 20.f);

	registerBoolSetting("Render", &this->renderPlacing, this->renderPlacing);
	registerBoolSetting("1.13+ ", &this->noCheckUpper, this->noCheckUpper);
	registerBoolSetting("Multi-Attack", &this->attackMulti, this->attackMulti);

	switchType = SettingEnum(this)
					 .addEntry(EnumEntry("none", 0))
					 .addEntry(EnumEntry("regular", 1))
					 .addEntry(EnumEntry("switchBack", 2))
					 .addEntry(EnumEntry("APVPSpoof", 3));
	registerEnumSetting("Switch Type", &this->switchType, 0);
}

CrystalAura::~CrystalAura(){};

const char* CrystalAura::getModuleName() {
	return ("CrystalPlaceJTWD");
}
static std::vector<Entity*> tgtList;
bool locateEntCrPl(Entity* curEnt) {
	static auto CrystalAuraMod = moduleMgr->getModule<CrystalAura>("CrystalAura");
	if (curEnt == nullptr) return false;
	if (curEnt == Game.getLocalPlayer()) return false;
	if (!curEnt->isAlive()) return false;
	if (!Game.getLocalPlayer()->isAlive()) return false;
	if (curEnt->getEntityTypeId() == 71) return false;
	if (curEnt->getEntityTypeId() == 66) return false;
	if (curEnt->getEntityTypeId() == 64) return false;
	if (curEnt->getEntityTypeId() == 69) return false;
	if (curEnt->aabbShape->width <= 0.01f || curEnt->aabbShape->height <= 0.01f) return false;
	if (!Target::isValidTarget(curEnt, true, false)) return false;

	float dist = (curEnt->getPos()->sub(0.f, 1.6f, 0.f)).dist(Game.getLocalPlayer()->getPos()->sub(0.f, 1.6f, 0.f));
	if (dist <= 6) {
		tgtList.push_back(curEnt);
		return true;
	}
	return false;
}

static std::vector<Entity*> entLocator120;
bool processEntForentLocator120(Entity* curEnt) {
	static auto CrystalAuraMod = moduleMgr->getModule<CrystalAura>("CrystalAura");
	if (curEnt == nullptr) return false;
	if (!curEnt->isAlive()) return false;
	if (!Game.getLocalPlayer()->isAlive()) return false;
	if (curEnt->getEntityTypeId() == 71) return false;
	if (curEnt->getEntityTypeId() == 69) return false;
	if (curEnt->aabbShape->width <= 0.01f || curEnt->aabbShape->height <= 0.01f) return false;

	float dist = (curEnt->getPos()->sub(0.f, 1.6f, 0.f)).dist(Game.getLocalPlayer()->getPos()->sub(0.f, 1.6f, 0.f));
	if (dist <= 6) {
		entLocator120.push_back(curEnt);
		return true;
	}
	return false;
}

void CrystalAura::onEnable() {
	if (Game.getLocalPlayer() == nullptr) return;

	CJTWDPlaceArr.clear();
	tgtList.clear();
	entLocator120.clear();
	return;
}
inline bool checkCornerHitboxCollision(const Vec3& block, Entity* ent) {
	AABB entityAABB = ent->aabbShape->aabb;

	if (ent->getEntityTypeId() == 319) {
		Vec3 pp = ent->getPos()->sub(0.f, 1.6f, 0.f);
		float epsilon = 0.001f;  // Small value to ensure precision

		return (
			(pp.x - epsilon <= block.x && block.x <= pp.x + 0.3f + epsilon) &&
			(pp.z - epsilon <= block.z && block.z <= pp.z + 0.3f + epsilon));
	} else {
		Vec3 lower = entityAABB.lower.floor();
		Vec3 upper = entityAABB.upper.floor();

		return (block >= lower && block <= upper);
	}

	return false;
}

inline lineResults countBloksAlongLine(Vec3 start, Vec3 end) {
	Vec3 endf = end.floor();
	Vec3 startf = start.floor();
	lineResults rtn = {};

	if (endf == startf)
		return rtn;

	float dist = startf.dist(endf);
	Vec3 delta = (endf.sub(startf)).div(dist);

	for (float t = 0.0f; t <= 1.0f; t += dist / 200.0f) {
		Vec3 point = startf.add(delta.mul(dist * t));

		Vec3i inCoord = point.floor();

		if (!Game.getRegion()->getBlock(inCoord)->toLegacy()->canBeBuiltOver(*Game.getRegion(), inCoord)) {
			rtn.blockCount += dist / 200.0f;
			rtn.lastSolidBlock = point;
		}
	}

	return rtn;
}
using getSeenPercent_t = float(__fastcall*)(BlockSource*, Vec3 const&, AABB const&);
static getSeenPercent_t getSeenPercent = reinterpret_cast<getSeenPercent_t>(FindSignature("40 53 55 41 56 48 81 ec ? ? ? ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 84 24"));

bool CrystalAura::isPlaceValid(Vec3 location, Entity* atkObj) {
	auto region = Game.getRegion();
	auto playerPos = Game.getLocalPlayer()->getPos()->sub(0.f, 1.6f, 0.f);
	auto atkObjPos = atkObj->getPos()->sub(0.f, 1.6f, 0.f).floor();

	float distToLoc = location.dist(playerPos);

	if (getSeenPercent(region, location.add(0.5f, 0.f, 0.5f), Game.getLocalPlayer()->aabbShape->aabb) == 0.f &&
		thruWallsR != 10 && (thruWallsR != 0 ? countBloksAlongLine(playerPos, location.add(0.5f)).blockCount >= thruWallsR || countBloksAlongLine(playerPos, location.add(0.5f)).lastSolidBlock.dist(location) > postWallsR : false)) {
		return false;
	}

	if (region->getBlock(location)->toLegacy()->blockId != 0 ||
		(!noCheckUpper && region->getBlock(location.add(0, 1, 0))->toLegacy()->blockId != 0) ||
		(!noCheckUpper && checkCornerHitboxCollision(location.add(0, 1, 0), atkObj)) ||
		checkCornerHitboxCollision(location, atkObj) ||
		(region->getBlock(location.sub(0, 1, 0))->toLegacy()->blockId != 49 &&
		 region->getBlock(location.sub(0, 1, 0))->toLegacy()->blockId != 7) ||
		distToLoc > placeRange ||
		Game.getLocalPlayer()->getHealth() < minHealth ||
		computeExplosionDamage(location, Game.getLocalPlayer(), region) > maxSelfDmg) {
		return false;
	}

	float enemyDmg = computeExplosionDamage(location, atkObj, region);

	if (enemyDmg < minEnemDmg && atkObjPos.y >= location.y)
		return false;

	if ((atkObjPos.y + 1) <= location.y && facePlaceType.GetSelectedEntry().GetValue() == 0 &&
		((fpThresh < atkObj->getHealth() && enemyDmg < dmgAtThresh) || enemyDmg < minEnemDmg))
		return false;

	return !((atkObjPos.y + 1) <= location.y && facePlaceType.GetSelectedEntry().GetValue() == 1) && !std::any_of(entLocator120.begin(), entLocator120.end(), [&](Entity* i) { return i != atkObj && i->getPos()->sub(0.f, 1.6f, 0.f).floor() == location; }) &&
		   Game.getLocalPlayer()->getPos()->sub(0.f, 1.6f, 0.f).floor() != location;
}


float CrystalAura::computeExplosionDamage(Vec3 crystalPos, Entity* target, BlockSource* reg) {
	constexpr float explosionRadius = 12.f;

	Vec3 pos = target->getPos()->sub(0.f, 1.6f, 0.f);
	float dist = pos.dist(crystalPos) / explosionRadius;

	if (dist > 1.f) {
		return 0.f;
	}

	float exposure = getSeenPercent(reg, crystalPos.add(0.5f, 0.f, 0.5f), target->aabbShape->aabb);
	float impact = std::max(0.f, (1.f - dist) * exposure);

	int damage = static_cast<int>((impact * impact * 3.5f + impact * 0.5f * 7.f) * explosionRadius + 1.f);

	int armorPoints = 0, epf = 0;
	for (int i = 0; i < 4; i++) {
		ItemStack* armor = target->getArmor(i);
		if (armor->item != nullptr) {
			armorPoints += (*armor->item)->getArmorValue();
			epf += static_cast<int>(getBlastDamageEnchantReduction(armor));
		}
	}

	float armorReduction = std::min(armorPoints * 0.04f, 0.8f);
	float finalDamage = std::max(0.f, damage * (1.f - armorReduction));

	float enchantReduction = std::min(epf, 25) * 0.04f;
	return std::max(0.f, finalDamage * (1.f - enchantReduction));
}

float CrystalAura::getBlastDamageEnchantReduction(ItemStack* armor) {
	float epf = 0.f;
	if (armor->getEnchantValue(0) != 4) {
		epf += armor->getEnchantValue(0);
	} else
		epf += 5;
	epf += armor->getEnchantValue(3) * 2.f;

	return epf;
}

bool cmpAgain(CrystalPlacements E1, CrystalPlacements E2) {
	return E1.enemyDmg > E2.enemyDmg;
}

int64_t getKey(const Vec3& pos) {
	return static_cast<int64_t>(pos.x) << 20 | static_cast<int64_t>(pos.y) << 10 | static_cast<int64_t>(pos.z);
}
CrystalPlacements CrystalAura::CrystalAuraJTWD(Entity* target) {
	std::vector<CrystalPlacements> on3d;

	int maxVOff = (facePlaceType.GetSelectedEntry().GetValue() == 1) ? 1 : 3;

	for (int vOffset = -5; vOffset < maxVOff; vOffset++) {
		Vec3 targetPos = target->getPos()->sub(0.f, 1.6f, 0.f).floor().add(0.f, static_cast<float>(vOffset), 0.f);

		std::unordered_set<int64_t> visitedPositions;
		std::map<float, CrystalPlacements> bunchashit;

		for (int x = -maxProximity; x < maxProximity; x++) {
			for (int z = -maxProximity; z < maxProximity; z++) {
				Vec3 search = targetPos.add(x, 0, z);
				auto posKey = getKey(search);
				if (search.dist(target->getPos()->sub(0.f, 1.6f, 0.f)) < maxProximity &&
					target->getPos()->sub(0.f, 1.6f, 0.f).floor() != search &&
					visitedPositions.emplace(posKey).second &&
					isPlaceValid(search, target)) {
					bunchashit[computeExplosionDamage(search, target, Game.getRegion())] = {search, computeExplosionDamage(search, target, Game.getRegion()), computeExplosionDamage(search, Game.getLocalPlayer(), Game.getRegion())};
				}
			}
		}

		if (!bunchashit.empty()) {
			auto& maxElement = bunchashit.rbegin()->second;
			on3d.push_back(maxElement);
		}
	}

	if (on3d.empty()) {
		return {{}, -42069};
	}

	std::sort(on3d.begin(), on3d.end(), [](const auto& a, const auto& b) {
		return a.enemyDmg > b.enemyDmg;
	});

	return on3d[0];
}


Entity* p;
bool compareTargListVec(Entity* E1, Entity* E2) {
	return Game.getLocalPlayer()->getPos()->sub(0.f, 1.6f, 0.f).dist(E1->getPos()->sub(0.f, 1.6f, 0.f)) < Game.getLocalPlayer()->getPos()->sub(0.f, 1.6f, 0.f).dist(E2->getPos()->sub(0.f, 1.6f, 0.f));
}
Vec2 angle;
void CrystalAura::onTick(GameMode* gm) {
	if (Game.getLocalPlayer() == nullptr || !Game.canUseMoveKeys() || (switchType.GetSelectedEntry().GetValue() == 3))
		return;

	placeArrEmpty = CJTWDPlaceArr.empty();

	entLocator120.clear();
	tgtList.clear();
	CJTWDPlaceArr.clear();

	Game.forEachEntity(locateEntCrPl);

	if (tgtList.empty()) return;

	if (Game.getLocalPlayer()->getSelectedItemId() == 300 || Game.getLocalPlayer()->getSelectedItemId() == 316 || Game.getLocalPlayer()->getSelectedItemId() == 318 || Game.getLocalPlayer()->getSelectedItemId() == 319 || Game.getLocalPlayer()->getSelectedItemId() == 604 || Game.getLocalPlayer()->getSelectedItemId() == 607 || Game.getLocalPlayer()->getSelectedItemId() == 606)
		return;

	std::sort(tgtList.begin(), tgtList.end(), compareTargListVec);

	if (!attackMulti) {
		while (tgtList.size() > 0) {
			CrystalPlacements placeInfo = CrystalAuraJTWD(tgtList[0]);
			if (placeInfo.enemyDmg != -42069) {
				CJTWDPlaceArr.push_back({placeInfo, tgtList[0]});
				break;
			}
			tgtList.erase(tgtList.begin());
		}
	} else {
		for (auto& enemy : tgtList) {
			CrystalPlacements placeInfo = CrystalAuraJTWD(enemy);
			if (placeInfo.enemyDmg != -42069) {
				CJTWDPlaceArr.push_back({placeInfo, enemy});
			}
		}
	}

	ctr++;

	if (ctr >= delay) {
		angle = Game.getLocalPlayer()->getPos()->CalcAngle(CJTWDPlaceArr[0].CPlacements.toPlace);
		for (CrystalInfo& Place : CJTWDPlaceArr) {
			Vec3 placeMe = Place.CPlacements.toPlace;
			
			Game.getGameMode()->buildBlock(Vec3i(placeMe).sub(0, 1, 0), 1);
		}

		Game.forEachEntity([](Entity* ent) {
			int id = ent->getEntityTypeId();
			if (id == 71) {
				p = ent;
				Game.getGameMode()->attack(ent);
			}
		});

		ctr = 0;
		return;
	}

	return;
}

void CrystalAura::onLevelRender() {
	if (!Game.getLocalPlayer() || !Game.getGameMode()) return;
	auto plr = Game.getLocalPlayer();
	
	
	

	plr->viewAnglesPtr->viewAngles.x = angle.x;
}
void CrystalAura::onDisable() {
	if (switchType.GetSelectedEntry().GetValue() == 1) {
		auto supplies = Game.getLocalPlayer()->getSupplies();
		supplies->selectedHotbarSlot = origSlot;
		return;
	}
}

float roundoff(float value, unsigned char prec) {
	return round(value * pow(10.0f, static_cast<float>(prec))) / pow(10.0f, static_cast<float>(prec));
}

std::string chopOffDigits(const std::string& STUFF, int digits) {
	std::string toOutput;
	bool afterDecimalPt = false;

	for (char ch : STUFF) {
		toOutput.push_back(ch);

		if (ch == '.') {
			afterDecimalPt = true;
		}

		if (afterDecimalPt && --digits <= 0) {
			break;
		}
	}

	return toOutput;
}

void CrystalAura::onPreRender(MinecraftUIRenderContext* renderCtx) {
	if (Game.getLocalPlayer() == nullptr) return;
	if (!Game.getLocalPlayer()->isAlive()) {
		return;
	}

	if (!Game.canUseMoveKeys() || tgtList.empty() || !Game.isInGame() || tgtList[0] == NULL || tgtList.size() == 0 || tgtList[0] == nullptr || !Game.getLocalPlayer()->isAlive()) {
		tgtList.clear();
		return;
	}
	GuiData* dat = Game.getClientInstance()->getGuiData();
	Vec2 windowSize = dat->windowSize;
	std::string text = tgtList[0]->getNameTag().c_str();
	Vec3 pp = tgtList[0]->getPos()->sub(0.f, 1.6f, 0.f);
	if (CJTWDPlaceArr.empty()) return;

	if (renderPlacing) {
		for (CrystalInfo& indivCrystal : CJTWDPlaceArr) {
			if (CJTWDPlaceArr.size() == 0 || tgtList.size() == 0) return;
			Vec3 placeLoc = indivCrystal.CPlacements.toPlace;
			float enemyDmg = indivCrystal.CPlacements.enemyDmg;
			float selfDmg = indivCrystal.CPlacements.selfDmg;

			DrawUtils::SetColor(.1f, .75f, 1.f, 1.f);
			DrawUtils::drawBox(placeLoc.sub(0, 1, 0), placeLoc.add(1, 0, 1), .4f, false);

			std::string self_dmg = std::to_string(roundoff(selfDmg, 2));
			self_dmg = chopOffDigits(self_dmg, 2);

			DrawUtils::drawTextInWorld(&self_dmg, placeLoc.sub(0, 1.4f, 0), 0.6f, Vec3i(66, 238, 238), Vec3i(12, 12, 12));

			Vec3 posplace = (indivCrystal.ent->getPos()->sub(0.f, 1.6f, 0.f)).floor();

			std::string enem_dmg = std::to_string(roundoff(enemyDmg, 2));
			enem_dmg = chopOffDigits(enem_dmg, 2);

			DrawUtils::drawTextInWorld(&enem_dmg, placeLoc.sub(0, 1.2f, 0), 0.6f, Vec3i(255, 0, 0), Vec3i(12, 12, 12));
		}
	}
	return;
}
void CrystalAura::onSendPacket(Packet* packet) {
	if (!Game.getLocalPlayer()) return;

	if (packet->getId() == PacketID::InventoryTransaction) {
		InventoryTransactionPacket* itp = reinterpret_cast<InventoryTransactionPacket*>(packet);
		if (itp->transaction->type == ComplexInventoryTransaction::Type::ItemUseOnActor) {
			ComplexInventoryTransaction* invComplex = itp->transaction;
			*(int*)((__int64)(invComplex) + 104) = p->getRuntimeId();
		}
	}
	if (packet->getId() == PacketID::MovePlayer) {
		auto* movePacket = reinterpret_cast<MovePlayerPacket*>(packet);
		movePacket->pitch = angle.x;
		movePacket->headYaw = angle.y;
		movePacket->yaw = angle.y;
	}
	if (packet->getId() == PacketID::PlayerAuthInput) {
		auto* authPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
		authPacket->rot = Vec2(angle.x, angle.y);
		authPacket->headYaw = angle.y;
	}
}