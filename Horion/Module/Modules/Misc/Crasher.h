#pragma once

#include "../Module.h"

class Crasher : public IModule {
public:
	enum class CrashType {
		MOVEMENT_PACKET = 0,
		PURCHASE_RECEIPT_OVERFLOW = 1,
		BLOATED_SUBCHUNK_REQUEST = 2,
		PERSONA_PIECE_OVERFLOW = 3,
		FREEZE = 4,
		TEST = 100,
	};

	Crasher();

	SettingEnum selectedCrashType;
	static constexpr inline float MAGIC_MOVEMENT_PKT_NUM = static_cast<float>(0xFFFFFFFF);

	virtual const char* getModuleName() override;
	virtual void onSendPacket(Packet* packet) override;
	virtual void onTick(GameMode* gm) override;

	inline CrashType getCrashType() {
		return static_cast<CrashType>(this->selectedCrashType.GetSelectedEntry().GetValue());
	}
};
