#include "SwingSounds.h"

SwingSounds::SwingSounds() : IModule(0, Category::WORLD, "Changes your attack swing sounds") {

	this->registerEnumSetting("Swings", &this->selectedSwingSound, (int32_t)SoundType::EVOKER_WOLOLO);

	this->selectedSwingSound.addEntry(EnumEntry("Evoker Wololo", (int32_t)SoundType::EVOKER_WOLOLO));
	this->selectedSwingSound.addEntry(EnumEntry("Ender Dragon Death", (int32_t)SoundType::ENDER_DRAGON_DEATH));
	this->selectedSwingSound.addEntry(EnumEntry("Fireball Shoot", (int32_t)SoundType::FIREBALL_SHOOT));
	this->selectedSwingSound.addEntry(EnumEntry("Crossbow Shoot", (int32_t)SoundType::CROSSBOW_SHOOT));
	this->selectedSwingSound.addEntry(EnumEntry("Ghast Scream", (int32_t)SoundType::GHAST_SCREAM));
	this->selectedSwingSound.addEntry(EnumEntry("Village Bell Ring", (int32_t)SoundType::VILLAGE_BELL_RING));
	this->selectedSwingSound.addEntry(EnumEntry("Jukebox Record (Ward)", (int32_t)SoundType::JUKEBOX_WARD));
	this->selectedSwingSound.addEntry(EnumEntry("Jukebox Record (Mellohi)", (int32_t)SoundType::JUKEBOX_MELLOHI));
	this->selectedSwingSound.addEntry(EnumEntry("Jukebox Record (Pigstep)", (int32_t)SoundType::JUKEBOX_PIGSTEP));
	this->selectedSwingSound.addEntry(EnumEntry("Goat Scream", (int32_t)SoundType::GOAT_SCREAM));
	this->selectedSwingSound.addEntry(EnumEntry("Warden Sonic Boom", (int32_t)SoundType::WARDEN_SONIC_BOOM));
	this->selectedSwingSound.addEntry(EnumEntry("Village Raid Horn", (int32_t)SoundType::VILLAGE_RAID_HORN));
	this->selectedSwingSound.addEntry(EnumEntry("Conduit Activate", (int32_t)SoundType::CONDUIT_ACTIVATE));
	this->selectedSwingSound.addEntry(EnumEntry("Nether Portal Travel", (int32_t)SoundType::NETHER_PORTAL_TRAVEL));
	this->selectedSwingSound.addEntry(EnumEntry("Thunder", (int32_t)SoundType::THUNDER));
	this->selectedSwingSound.addEntry(EnumEntry("TNT Explode", (int32_t)SoundType::TNT_EXPLODE));
	this->selectedSwingSound.addEntry(EnumEntry("Evocation Fang", (int32_t)SoundType::EVOCATION_FANG));
	this->selectedSwingSound.addEntry(EnumEntry("Eating Burp", (int32_t)SoundType::EATING_BURP));
	this->selectedSwingSound.addEntry(EnumEntry("None", (int32_t)SoundType::NONE));

	this->registerBoolSetting("Global Sound", &this->ShouldBeGlobal, this->ShouldBeGlobal);
}

const char* SwingSounds::getModuleName() {
	return "SwingSounds";
}

void SwingSounds::onSendPacket(Packet* packet) {
	if (packet->getId() == PacketID::LevelSoundEvent) {

		auto& soundPkt = *reinterpret_cast<LevelSoundEventPacket*>(packet);

		if (this->isSwingSoundEvent(soundPkt.sound)) {
			soundPkt.isGlobal = ShouldBeGlobal;

			switch (this->getSoundType()) {
			case SoundType::EVOKER_WOLOLO: {
				soundPkt.sound = LevelSoundEvent::PREPARE_WOLOLO;
				soundPkt.entityType = "minecraft:evocation_illager";
				soundPkt.extraData = -1; soundPkt.isBabyMob = false;
				break;
			}
			case SoundType::ENDER_DRAGON_DEATH: {
				soundPkt.sound = LevelSoundEvent::DEATH;
				soundPkt.entityType = "minecraft:ender_dragon";
				soundPkt.extraData = -1; soundPkt.isBabyMob = false;
				break;
			}
			case SoundType::FIREBALL_SHOOT: {
				soundPkt.sound = LevelSoundEvent::SHOOT;
				soundPkt.entityType = "minecraft:ghast";
				soundPkt.extraData = -1; soundPkt.isBabyMob = false;
				break;
			}
			case SoundType::CROSSBOW_SHOOT: {
				soundPkt.sound = LevelSoundEvent::CROSSBOW_SHOOT;
				soundPkt.entityType = "";
				soundPkt.extraData = -1; soundPkt.isBabyMob = false;
				break;
			}
			case SoundType::GHAST_SCREAM: {
				soundPkt.sound = LevelSoundEvent::WARN;
				soundPkt.entityType = "minecraft:ghast";
				soundPkt.extraData = -1; soundPkt.isBabyMob = false;
				break;
			}
			case SoundType::VILLAGE_BELL_RING: {
				soundPkt.sound = LevelSoundEvent::BLOCK_BELL_HIT;
				soundPkt.entityType = "";
				soundPkt.extraData = -1; soundPkt.isBabyMob = false;
				break;
			}
			case SoundType::JUKEBOX_WARD: {
				soundPkt.sound = LevelSoundEvent::RECORD_WARD;
				soundPkt.entityType = "";
				soundPkt.extraData = -1; soundPkt.isBabyMob = false;
				break;
			}
			case SoundType::JUKEBOX_MELLOHI: {
				soundPkt.sound = LevelSoundEvent::RECORD_MELLOHI;
				soundPkt.entityType = "";
				soundPkt.extraData = -1; soundPkt.isBabyMob = false;
				break;
			}
			case SoundType::JUKEBOX_PIGSTEP: {
				soundPkt.sound = LevelSoundEvent::RECORD_PIGSTEP;
				soundPkt.entityType = "";
				soundPkt.extraData = -1; soundPkt.isBabyMob = false;
				break;
			}
			case SoundType::GOAT_SCREAM: {
				soundPkt.sound = LevelSoundEvent::PRE_RAM_SCREAMER;
				soundPkt.entityType = "minecraft:goat";
				soundPkt.extraData = -1; soundPkt.isBabyMob = false;
				break;
			}
			case SoundType::WARDEN_SONIC_BOOM: {
				soundPkt.sound = LevelSoundEvent::SONIC_BOOM;
				soundPkt.entityType = "minecraft:warden";
				soundPkt.extraData = -1; soundPkt.isBabyMob = false;
				break;
			case SoundType::VILLAGE_RAID_HORN: {
				soundPkt.sound = LevelSoundEvent::RAID_HORN;
				soundPkt.entityType = "minecraft:ravager";
				soundPkt.extraData = -1; soundPkt.isBabyMob = false;
				break;
			}
			case SoundType::CONDUIT_ACTIVATE: {
				soundPkt.sound = LevelSoundEvent::CONDUIT_ACTIVATE;
				soundPkt.entityType = "";
				soundPkt.extraData = -1; soundPkt.isBabyMob = false;
				break;
			}
			case SoundType::NETHER_PORTAL_TRAVEL: {
				soundPkt.sound = LevelSoundEvent::PORTAL_TRAVEL;
				soundPkt.entityType = "";
				soundPkt.extraData = -1; soundPkt.isBabyMob = false;
				break;
			}
			case SoundType::THUNDER: {
				soundPkt.sound = LevelSoundEvent::THUNDER;
				soundPkt.entityType = "minecraft:lightning_bolt";
				soundPkt.extraData = -1; soundPkt.isBabyMob = false;
				break;
			}
			case SoundType::TNT_EXPLODE: {
				soundPkt.sound = LevelSoundEvent::EXPLODE;
				soundPkt.entityType = "";
				soundPkt.extraData = -1; soundPkt.isBabyMob = false;
				break;
			}
			case SoundType::EVOCATION_FANG: {
				soundPkt.sound = LevelSoundEvent::FANG;
				soundPkt.entityType = "minecraft:evocation_fang";
				soundPkt.extraData = -1; soundPkt.isBabyMob = false;
				break;
			}
			case SoundType::EATING_BURP: {
				soundPkt.sound = LevelSoundEvent::BURP;
				soundPkt.entityType = "";
				soundPkt.extraData = -1; soundPkt.isBabyMob = false;
				break;
			}
			}
			case SoundType::NONE: {
				/*soundPkt.sound = LevelSoundEvent::STOP_RECORD;
				soundPkt.entityType = ""; soundPkt.extraData = -1; soundPkt.isBabyMob = false; soundPkt.isGlobal = false;
				break;*/
				return;
			}
			default:
				break;
			}
		}
	}
}