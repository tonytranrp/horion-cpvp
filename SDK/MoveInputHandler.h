#pragma once

#include <cstdint>
#include <Utils/Utils.h>

class MoveInputHandler {
	uintptr_t** vtable;
	uint8_t pad[0x80 - sizeof(MoveInputHandler::vtable)]{};

public:
	CLASS_FIELD(sideMovement, 0x8, float);
	CLASS_FIELD(forwardMovement, 0xC, float);
	CLASS_FIELD(isSneakDown, 0x50, bool);
	CLASS_FIELD(isJumping, 0x4C, bool);
	CLASS_FIELD(autoJumpInWater, 0x4D, bool);
	CLASS_FIELD(forward, 0x5A, bool);
	CLASS_FIELD(backward, 0x6B, bool);
	CLASS_FIELD(left, 0x5C, bool);
	CLASS_FIELD(right, 0x5D, bool);

	void clearMovementState();
};
