#pragma once

#include "../Module.h"
#include <SDK/LevelSoundEvent.h>

class SwingSounds : public IModule {
public:

	enum class SoundType : uint8_t {
		EVOKER_WOLOLO = 0,
		ENDER_DRAGON_DEATH,
		FIREBALL_SHOOT,
		CROSSBOW_SHOOT,
		GHAST_SCREAM,
		VILLAGE_BELL_RING,
		JUKEBOX_WARD,
		JUKEBOX_MELLOHI,
		JUKEBOX_PIGSTEP,
		GOAT_SCREAM,
		WARDEN_SONIC_BOOM,
		VILLAGE_RAID_HORN,
		CONDUIT_ACTIVATE,
		NETHER_PORTAL_TRAVEL,
		THUNDER,
		TNT_EXPLODE,
		EVOCATION_FANG,
		EATING_BURP,
		NONE = 255,
	};

	SettingEnum selectedSwingSound;
	bool ShouldBeGlobal = false;

	SwingSounds();

	virtual const char* getModuleName() override;
	virtual void onSendPacket(Packet* packet) override;

	inline bool isSwingSoundEvent(LevelSoundEvent ev) {
		return ((ev >= LevelSoundEvent::ATTACK) && (ev <= LevelSoundEvent::ATTACK_STRONG));
	}
	inline SoundType getSoundType() {
		return (SoundType)(this->selectedSwingSound.GetSelectedEntry().GetValue());
	}
};