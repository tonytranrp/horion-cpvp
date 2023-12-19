#include "CrystalAura.h"


CrystalAura::CrystalAura() : IModule(VK_NUMPAD0, Category::COMBAT, "Automatically places and destroys crystals near other players") {
	registerFloatSetting("Range", &this->range, this->range, 1.f, 10.f);
	registerIntSetting("Place Delay", &this->placeDelay, this->placeDelay, 0, 10);
	registerIntSetting("packets", &this->packets, this->packets, 0, 10);
	registerIntSetting("Attack Delay", &this->attackDelay, this->attackDelay, 0, 10);
	registerBoolSetting("Auto Place", &this->autoPlace, this->autoPlace);
	registerBoolSetting("Multi Place", &this->multiPlace, this->multiPlace);
}

CrystalAura::~CrystalAura() {
}

const char* CrystalAura::getModuleName() {
	return ("CrystalAura");
}

std::string CrystalAura::getModeText() {
	return multiPlace ? "Multi" : "Single";
}

int Odelay = 0;//Place Delay
int Ddelay = 0;//Attack Delay
static std::vector<Entity*> targetList;
static std::vector<Entity*> CrystalList;
void FindValidEnts(Entity* currentEntity) {
	static auto CrystalAuraMod = moduleMgr->getModule<CrystalAura>("CrystalAura");

	if (!currentEntity)
		return;

	if (currentEntity == Game.getLocalPlayer())  // Skip Local player
		return;

	if (!Game.getLocalPlayer()->canAttack(currentEntity))
		return;

	if (!Game.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;
	
	float dist = (*currentEntity->getPos()).dist(*Game.getLocalPlayer()->getPos());
	if (dist < CrystalAuraMod->range) {
		if (Target::isValidTarget(currentEntity, false, false))
				targetList.push_back(currentEntity);

		if (currentEntity->getEntityTypeId() == 71)
			CrystalList.push_back(currentEntity);
	}
}

void CrystalAura::onEnable() {
	Odelay = 0;
	Ddelay = 0;
}
#include <unordered_set>

using getSeenPercent_t = float(__fastcall*)(BlockSource*, Vec3 const&, AABB const&);
static getSeenPercent_t getSeenPercent = reinterpret_cast<getSeenPercent_t>(FindSignature("40 53 55 41 56 48 81 ec ? ? ? ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 84 24"));
float getBlastDamageEnchantReduction(ItemStack* armor) {
	float epf = 0.f;

	if (armor->getEnchantValue(0) != 4) {
		epf += armor->getEnchantValue(0);  // protection
	} else
		epf += 5;                            //  yeah prot in BE is OP AF
	epf += armor->getEnchantValue(3) * 2.f;  // blast protection

	return epf;
}
#include <memory>
inline bool checkCornerHitboxCollision(Vec3* block,Entity* ent) {  // THANK SB HOLY SHIT I WAS TRYING TO MAKE THIS BY MYSELF FOR HOURS!!11!1
	std::vector<Vec3*> corners;
	corners.clear();

	corners.push_back(new Vec3(ent->aabbShape->aabb.lower.x, ent->aabbShape->aabb.lower.y, ent->aabbShape->aabb.lower.z));
	corners.push_back(new Vec3(ent->aabbShape->aabb.lower.x, ent->aabbShape->aabb.lower.y, ent->aabbShape->aabb.upper.z));
	corners.push_back(new Vec3(ent->aabbShape->aabb.upper.x, ent->aabbShape->aabb.lower.y, ent->aabbShape->aabb.upper.z));
	corners.push_back(new Vec3(ent->aabbShape->aabb.upper.x, ent->aabbShape->aabb.lower.y, ent->aabbShape->aabb.lower.z));

	if (ent->getEntityTypeId() != 319) {
		if (!corners.empty()) {
			for (auto corner : corners) {
				if ((floor(corner->x) == floor(block->x)) && (floor(corner->y) == floor(block->y)) && (floor(corner->z) == floor(block->z))) {
					return true;
				}
			}
		}
	} else {
		Vec3 pp = *ent->getPos();
		Vec3 entCorners[8] = {
			pp.add(.3f, 0, .3f),
			pp.add(-.3f, 0, .3f),
			pp.add(.3f, 0, -.3f),
			pp.add(-.3f, 0, -.3f),
			pp.add(.33541f, 0, 0),
			pp.add(-.33541f, 0, 0),
			pp.add(0, 0, .33541f),
			pp.add(0, 0, -.33541f),
		};

		for (Vec3 i : entCorners) {
			if (i.floor() == *block) {
				return true;
			}
		}
	}

	return false;
}
float DamageToEntity(Entity* Target, Vec3 PlacementPos) {
	float explosionRadius = 12.f;  // 6 * 2

	Vec3 pos = *Target->getPos();

	float dist = pos.dist(PlacementPos) / explosionRadius;

	if (dist <= 1.f) {
		float exposure = getSeenPercent(Game.getRegion(), PlacementPos.add(0.5f), Target->aabbShape->aabb);
		float impact = (1 - dist) * exposure;

		int armorPoints = 0;
		int epf = 0;

		int damage = static_cast<int>((impact * impact * 3.5f + impact * 0.5f * 7.f) * explosionRadius + 1.f);

		float finalDamage = static_cast<float>(damage);

		for (int i = 0; i < 4; i++) {
			ItemStack* armor = Target->getArmor(i);

			if (armor->item == nullptr)
				continue;

			armorPoints += (*armor->item)->getArmorValue();
			epf += static_cast<int>(getBlastDamageEnchantReduction(armor));
		}

		// Improved explosion reduction using a more precise algorithm
		finalDamage -= finalDamage * armorPoints / (armorPoints + 8.0f) * 0.04f;

		// Improved enchant reduction using a more precise algorithm
		float enchantReduction = std::max(1 - epf / 25.0f, 0.0f);
		finalDamage -= finalDamage * std::min(enchantReduction, 0.2f) * 0.04f;

		int targetY = static_cast<int>(floorf(Target->aabbShape->aabb.lower.y));
		int crystalY = static_cast<int>(floorf(PlacementPos.y));

		return finalDamage;
	} else {
		return 0.f;
	}
}
std::vector<Vec3i> getCrystalPlacementsFromTarget(Entity* ent) {
	static auto CrystalAuraMod = moduleMgr->getModule<CrystalAura>("CrystalAura");
	Vec3 entPos = ent->getPos()->floor();
	entPos.y -= 1.f;

	std::vector<Vec3i> ValidPlacements;
	std::unordered_set<Vec3i, Vec3iHash> CheckedPositions;

	float maxDamage = 0.f;  // Variable to store the maximum damage among potential placements

	for (float x = entPos.x - 5.5f; x < entPos.x + 5.5f; x += 0.5f) {
		for (float z = entPos.z - 5.5f; z < entPos.z + 5.5f; z += 0.5f) {
			for (float y = entPos.y - 4.5f; y < entPos.y + 0.5f; y += 0.5f) {
				Vec3 blockPos = Vec3(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z));

				// Check if position is already checked
				if (CheckedPositions.find(blockPos) != CheckedPositions.end()) {
					continue;
				}

				auto blockid = Game.getRegion()->getBlock(blockPos)->toLegacy()->blockId;

				if (blockid == 49 || blockid == 7) {
					// Check for two air blocks beneath
					if (Game.getRegion()->getBlock(blockPos.add(0, 1, 0))->toLegacy()->blockId != 0 ||
						Game.getRegion()->getBlock(blockPos.add(0, 2, 0))->toLegacy()->blockId != 0)
						continue;

					// Calculate damage for the current placement
					float damage = DamageToEntity(ent, blockPos);

					// Check if the damage is non-zero, indicating it can damage the target
					if (damage > 0.f && !checkCornerHitboxCollision(&blockPos, ent)) {
						// Optionally, you can add more conditions for strategic placement
						// e.g., check if the placement is closer to the target, in a blind spot, etc.

						// Check if the damage for this placement is greater than the maximum recorded damage
						if (damage > maxDamage) {
							// Clear the existing valid placements, as we found a better one
							ValidPlacements.clear();
							// Update the maximum damage
							maxDamage = damage;
						}

						// Add the current placement to the list of valid placements
						ValidPlacements.push_back(blockPos);
					}
				}

				// Mark position as checked
				CheckedPositions.insert(blockPos);
			}
		}
	}

	// Sort placements based on distance (closest placement first)
	std::sort(ValidPlacements.begin(), ValidPlacements.end(), [&](const Vec3i& lhs, const Vec3i& rhs) {
		return (*ent->getPos()).dist(lhs.toVec3t()) < (*ent->getPos()).dist(rhs.toVec3t());
	});

	return ValidPlacements;
}


void CrystalAura::onTick(GameMode* gm) {
	auto player = Game.getLocalPlayer();
	if (!player) return;

	targetList.clear();
	CrystalList.clear();
	Game.forEachEntity(FindValidEnts);
	if (targetList.empty()) return;

	if (autoPlace) {
		std::vector<Vec3i> placements = getCrystalPlacementsFromTarget(targetList[0]);
		if (Odelay > placeDelay && !placements.empty()) {
			Odelay = 0;
			auto supplies = Game.getLocalPlayer()->getSupplies();
			int slot = supplies->selectedHotbarSlot;
			auto SelectedSlot = supplies->inventory->getItemStack(slot);

		
			if (SelectedSlot->isValid() && strcmp(SelectedSlot->getItem()->tileName.c_str(), "item.end_crystal") == 0) {
				// Crystal id moves a lot, so we check if it's valid before placing
				for (auto& place : placements) {
					gm->buildBlock(place, 1);
					if (!multiPlace) break;
				}
			}
		}
		Odelay++;
	}

	
	if (Ddelay > placeDelay && !CrystalList.empty()) {
		Ddelay = 0;

		// Sort CrystalList based on proximity to target (lambda function)
		std::sort(CrystalList.begin(), CrystalList.end(), [&](Entity* lhs, Entity* rhs) {
			return (*lhs->getPos()).dist(*targetList[0]->getPos()) < (*rhs->getPos()).dist(*targetList[0]->getPos());
		});

		if (player->getPos()->dist(*CrystalList[0]->getPos()) <= range) {
			player->swing();
			ItemUseOnActorInventoryTransaction complxTransac(
				ItemUseOnActorInventoryTransaction::ActionType::Attack,
				CrystalList[0]->getRuntimeId(),
				player->getSupplies()->selectedHotbarSlot,
				player->getSelectedItem(),
				player->stateVector->pos);

			InventoryTransactionPacket transPkt{&complxTransac, true};
			for (int i = 0; i < packets; i++) {
				Game.getLoopbackPacketSender()->sendToServer(&transPkt);
			}
			

			//gm->attack(CrystalList[0]);
			 
			
		}
	}
	Ddelay++;
}
void CrystalAura::onSendPacket(Packet* packet) {
	if (!Game.getLocalPlayer()) return;
	if (targetList.empty()) return;
	if (packet->getId() == PacketID::InventoryTransaction) {
		
		InventoryTransactionPacket* itp = reinterpret_cast<InventoryTransactionPacket*>(packet);
		if (itp->transaction->type == ComplexInventoryTransaction::Type::ItemUseOnActor) {
			ComplexInventoryTransaction* invComplex = itp->transaction;
			*(int*)((__int64)(invComplex) + 104) = CrystalList[0]->getRuntimeId();

		}
	}
	
	
}
