#include "MoveInputHandler.h"
#include "../Utils/Utils.h"

void MoveInputHandler::clearMovementState() {
	return Utils::CallVFunc<5, void>(this);
}
