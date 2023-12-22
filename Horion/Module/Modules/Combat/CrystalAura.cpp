#include "CrystalAura.h"

#include <unordered_set>
bool mobs = false;
CrystalAura::CrystalAura() : IModule(0, Category::COMBAT, "p100 java CA by JohnTWD // FatMC#1209") {
	registerBoolSetting("Mobs", &mobs, mobs);
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
	if (!Target::isValidTarget(curEnt, mobs, false)) return false;

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
		float epsilon = 0.001f;

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
	Vec3 delta = (end.floor().sub(start.floor())).div(200.f);
	lineResults rtn = {};

	for (float t = 0.0f; t <= 1.0f; t += 1.0f / 200.0f) {
		Vec3 point = start.add(delta.mul(200.f * t));

		if (!Game.getRegion()->getBlock(point.floor())->toLegacy()->canBeBuiltOver(*Game.getRegion(), point.floor())) {
			rtn.blockCount += 1.0f / 200.0f;
			rtn.lastSolidBlock = point;
		}
	}

	return rtn;
}

using getSeenPercent_t = float(__fastcall*)(BlockSource*, Vec3 const&, AABB const&);
static getSeenPercent_t getSeenPercent = reinterpret_cast<getSeenPercent_t>(FindSignature("40 53 55 41 56 48 81 ec ? ? ? ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 84 24"));

float CrystalAura::computeExplosionDamage(Vec3 crystalPos, Entity* target, BlockSource* reg) {
	constexpr float explosionRadius = 12.f;

	Vec3 pos = target->getPos()->sub(0.f, 1.6f, 0.f);
	float dist = pos.dist(crystalPos) / explosionRadius;
	float impact = std::max(0.f, 1.f - dist) * getSeenPercent(reg, crystalPos.add(0.5f, 0.f, 0.5f), target->aabbShape->aabb);

	float damage = (impact * (impact * 3.5f + 0.5f * 7.f)) * explosionRadius;

	int armorPoints = 0, epf = 0;
	for (int i = 0; i < 4; i++) {
		ItemStack* armor = target->getArmor(i);
		if (armor->item != nullptr) {
			armorPoints += (*armor->item)->getArmorValue();
			epf += static_cast<int>(getBlastDamageEnchantReduction(armor));
		}
	}

	float armorReduction = std::min(armorPoints * 0.04f, 0.8f);
	float enchantReduction = std::min(epf, 25) * 0.04f;

	return std::max(0.f, damage * (1.f - armorReduction) * (1.f - enchantReduction));
}

float CrystalAura::getBlastDamageEnchantReduction(ItemStack* armor) {
	float epf = (armor->getEnchantValue(0) != 4) ? armor->getEnchantValue(0) + 5 : 5 + armor->getEnchantValue(3) * 2.f;
	return epf;
}

int64_t getKey(const Vec3& pos) {
	return (static_cast<int64_t>(pos.x) << 20) | (static_cast<int64_t>(pos.y) << 10) | static_cast<int64_t>(pos.z);
}
CrystalPlacements CrystalAura::CrystalAuraJTWD(Entity* target) {
	std::vector<CrystalPlacements> on3d;
	int maxVOff = (facePlaceType.GetSelectedEntry().GetValue() == 1) ? 1 : 3;

	auto isValidLocation = [&](const Vec3& search) {
		auto region = Game.getRegion();
		auto playerPos = Game.getLocalPlayer()->getPos()->sub(0.f, 1.6f, 0.f);
		auto atkObjPos = target->getPos()->sub(0.f, 1.6f, 0.f).floor();
		float distToLoc = search.dist(playerPos);

		return !(region->getBlock(search)->toLegacy()->blockId != 0 ||
				 (!noCheckUpper && region->getBlock(search.add(0, 1, 0))->toLegacy()->blockId != 0) ||
				 (!noCheckUpper && checkCornerHitboxCollision(search.add(0, 1, 0), target)) ||
				 checkCornerHitboxCollision(search, target) ||
				 (region->getBlock(search.sub(0, 1, 0))->toLegacy()->blockId != 49 &&
				  region->getBlock(search.sub(0, 1, 0))->toLegacy()->blockId != 7) ||
				 distToLoc > placeRange ||
				 Game.getLocalPlayer()->getHealth() < minHealth ||
				 computeExplosionDamage(search, Game.getLocalPlayer(), region) > maxSelfDmg);
	};

	for (int vOffset = -5; vOffset < maxVOff; vOffset++) {
		Vec3 targetPos = target->getPos()->sub(0.f, 1.6f, 0.f).floor().add(0.f, static_cast<float>(vOffset), 0.f);
		std::unordered_map<int64_t, CrystalPlacements> bunchashit;

		for (int x = -maxProximity; x < maxProximity; x++) {
			for (int z = -maxProximity; z < maxProximity; z++) {
				Vec3 search = targetPos.add(x, 0, z);
				auto posKey = getKey(search);

				if (search.dist(target->getPos()->sub(0.f, 1.6f, 0.f)) < maxProximity &&
					target->getPos()->sub(0.f, 1.6f, 0.f).floor() != search &&
					isValidLocation(search) &&
					!(getSeenPercent(Game.getRegion(), search.add(0.5f, 0.f, 0.5f), target->aabbShape->aabb) == 0.f &&
					  thruWallsR != 10 &&
					  (thruWallsR != 0 ? countBloksAlongLine(Game.getLocalPlayer()->getPos()->sub(0.f, 1.6f, 0.f), search.add(0.5f)).blockCount >= thruWallsR ||
											 countBloksAlongLine(Game.getLocalPlayer()->getPos()->sub(0.f, 1.6f, 0.f), search.add(0.5f)).lastSolidBlock.dist(search) > postWallsR
									   : false))) {
					auto atkObjPos = target->getPos()->sub(0.f, 1.6f, 0.f).floor();
					float enemyDmg = computeExplosionDamage(search, target, Game.getRegion());

					if (!(enemyDmg < minEnemDmg && atkObjPos.y >= search.y) &&
						!((atkObjPos.y + 1) <= search.y && facePlaceType.GetSelectedEntry().GetValue() == 0 &&
						  ((fpThresh < target->getHealth() && enemyDmg < dmgAtThresh) || enemyDmg < minEnemDmg)) &&
						(!((atkObjPos.y + 1) <= search.y && facePlaceType.GetSelectedEntry().GetValue() == 1) &&
						 !std::any_of(entLocator120.begin(), entLocator120.end(), [&](Entity* i) {
							 return i != target && i->getPos()->sub(0.f, 1.6f, 0.f).floor() == search;
						 }) &&
						 Game.getLocalPlayer()->getPos()->sub(0.f, 1.6f, 0.f).floor() != search)) {
						bunchashit[posKey] = {search, enemyDmg, computeExplosionDamage(search, Game.getLocalPlayer(), Game.getRegion())};
					}
				}
			}
		}

		if (!bunchashit.empty()) {
			auto maxElement = std::max_element(bunchashit.begin(), bunchashit.end(), [](const auto& a, const auto& b) {
				return a.second.enemyDmg < b.second.enemyDmg;
			});

			on3d.push_back(maxElement->second);
		}
	}

	if (!on3d.empty()) {
		std::sort(on3d.begin(), on3d.end(), [](const auto& a, const auto& b) {
			return a.enemyDmg > b.enemyDmg;
		});

		return on3d[0];
	}

	return {{}, -42069};
}

Entity* p;
bool compareTargListVec(Entity* E1, Entity* E2) {
	return Game.getLocalPlayer()->getPos()->sub(0.f, 1.6f, 0.f).dist(E1->getPos()->sub(0.f, 1.6f, 0.f)) < Game.getLocalPlayer()->getPos()->sub(0.f, 1.6f, 0.f).dist(E2->getPos()->sub(0.f, 1.6f, 0.f));
}
Vec2 angle;
void attack(Entity* target) {
	auto lp = Game.getLocalPlayer();
	ItemUseOnActorInventoryTransaction complxTransac(
		ItemUseOnActorInventoryTransaction::ActionType::Attack,
		target->getRuntimeId(),
		lp->getSupplies()->selectedHotbarSlot,
		lp->getSelectedItem(),
		lp->stateVector->pos,
		target->stateVector->pos);

	InventoryTransactionPacket transPkt{&complxTransac, true};

	Game.getLoopbackPacketSender()->sendToServer(&transPkt);
}
void CrystalAura::onTick(GameMode* gm) {
	if (!Game.getLocalPlayer() || !Game.canUseMoveKeys() || (switchType.GetSelectedEntry().GetValue() == 3))
		return;

	entLocator120.clear();
	tgtList.clear();
	CJTWDPlaceArr.clear();

	Game.forEachEntity(locateEntCrPl);

	if (tgtList.empty())
		return;

	int selectedItem = Game.getLocalPlayer()->getSelectedItemId();
	if (std::unordered_set<int>{300, 316, 318, 319, 604, 607, 606}.count(selectedItem))
		return;

	std::sort(tgtList.begin(), tgtList.end(), compareTargListVec);

	for (auto& enemy : tgtList) {
		CrystalPlacements placeInfo = CrystalAuraJTWD(enemy);
		if (placeInfo.enemyDmg != -42069) {
			CJTWDPlaceArr.push_back({placeInfo, enemy});
			if (!attackMulti)
				break;
		}
	}

	ctr++;
	if (ctr >= delay) {
		Vec3 placeMe = CJTWDPlaceArr.empty() ? Vec3{} : CJTWDPlaceArr[0].CPlacements.toPlace;
		if (!placeMe.iszero()) {
			angle = Game.getLocalPlayer()->stateVector->pos.CalcAngle(placeMe);
			Game.getGameMode()->buildBlock(Vec3i(placeMe).sub(0, 1, 0), 1);
			Game.forEachEntity([](Entity* ent) {
				if (ent->getEntityTypeId() == 71) {
					p = ent;
					attack(ent);
				}
			});
		}

		ctr = 0;
	}
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
	if (!Game.getLocalPlayer() || !Game.getLocalPlayer()->isAlive() || !Game.canUseMoveKeys() || !Game.isInGame() || tgtList.empty() || tgtList[0] == nullptr) {
		tgtList.clear();
		return;
	}

	GuiData* dat = Game.getClientInstance()->getGuiData();
	Vec2 windowSize = dat->windowSize;

	if (CJTWDPlaceArr.empty() || !renderPlacing)
		return;

	for (CrystalInfo& indivCrystal : CJTWDPlaceArr) {
		if (tgtList.empty())
			return;

		Vec3 placeLoc = indivCrystal.CPlacements.toPlace.lerp(indivCrystal.ent->getPos()->sub(0.f, 1.6f, 0.f).floor(), 0.1f);
		float enemyDmg = indivCrystal.CPlacements.enemyDmg;
		float selfDmg = indivCrystal.CPlacements.selfDmg;

		DrawUtils::SetColor(.1f, .75f, 1.f, 1.f);
		DrawUtils::drawBox(placeLoc.sub(0, 1, 0), placeLoc.add(1, 0, 1), .4f, false);

		std::string self_dmg = chopOffDigits(std::to_string(roundoff(selfDmg, 2)), 2);
		DrawUtils::drawTextInWorld(&self_dmg, placeLoc.sub(0, 1.4f, 0), 0.6f, Vec3i(66, 238, 238), Vec3i(12, 12, 12));

		Vec3 posplace = (indivCrystal.ent->getPos()->sub(0.f, 1.6f, 0.f)).floor();

		std::string enem_dmg = chopOffDigits(std::to_string(roundoff(enemyDmg, 2)), 2);
		DrawUtils::drawTextInWorld(&enem_dmg, placeLoc.sub(0, 1.2f, 0), 0.6f, Vec3i(255, 0, 0), Vec3i(12, 12, 12));
	}
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
		movePacket->actorRuntimeId = Game.getLocalPlayer()->getRuntimeId();
	}
	if (packet->getId() == PacketID::PlayerAuthInput) {
		auto* authPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
		authPacket->rot = Vec2(angle.x, angle.y);
		authPacket->headYaw = angle.y;
		authPacket->clientTick = Game.getLocalPlayer()->getRuntimeId();
	}
}