#include "FollowPathModule.h"

#include "../../../../Utils/Logger.h"

FollowPathModule::FollowPathModule() : IModule(0, Category::MOVEMENT, "Follows joe paths") {
	this->registerBoolSetting("Assume Jesus", &this->assumeWaterWalk, assumeWaterWalk);
}

const char *FollowPathModule::getModuleName() {
	return "FollowPath";
}

void FollowPathModule::startSearch(Vec3i startNode, BlockSource *region, float searchTimeout, std::function<void(bool, JoePath)> callback) {
	if (this->pathFinder) {
		logF("Already searching!");
		return;
	}
	this->pathFinder = std::make_shared<JoePathFinder>(startNode, region, this->goal);
	this->pathFinder->pathSearchTimeout = searchTimeout;
	//std::thread([this, callback](){
	auto ref = this->pathFinder;  // so it won't get deleted when followpathmodule is disabled
	auto tempPath = this->pathFinder->findPath();
	this->pathFinder.reset();
	if (tempPath.getNumSegments() == 0 || !this->isEnabled()) {
		callback(false, tempPath);
		return;
	}
	callback(true, tempPath);
	//}).detach();
}

bool shouldStartSearch = false;
void FollowPathModule::onEnable() {
	if (!Game.isInGame() || !Game.getLocalPlayer()->isAlive()) {
		setEnabled(false);
		return;
	}

	if (!this->goal) {
		logF("goal not set");
		setEnabled(false);
		return;
	}
	shouldStartSearch = true;
}

void FollowPathModule::onDisable() {
	shouldStartSearch = false;
	if (this->pathFinder)
		this->pathFinder->terminateSearch = true;
	this->engageDelay = -1;

	this->pathFinder.reset();
	this->path.reset();
	this->nextPath.reset();
	this->movementController.reset();
	this->goal.reset();
}

void FollowPathModule::onTick(GameMode *mode) {
	if (!shouldStartSearch)
		return;
	shouldStartSearch = false;

	auto player = Game.getLocalPlayer();
	auto pPos = player->getEyePos();
	Vec3i startNode((int)floorf(pPos.x), (int)roundf(pPos.y - 1.62f), (int)floorf(pPos.z));

	this->startSearch(startNode, Game.getRegion(), 0.5f, [&](bool succeeded, JoePath tempPath) {
		if (!succeeded) {
			this->clientMessageF("%sCould not find a path!", RED);
			this->path.reset();
			this->movementController.reset();
			this->setEnabled(false);
			this->engageDelay = -1;
			return;
		}

		this->clientMessageF("%sFound %s path!", tempPath.isIncomplete1() ? YELLOW : GREEN, tempPath.isIncomplete1() ? "incomplete" : "complete");

		if (tempPath.isIncomplete1()) {
			tempPath.cutoff(0.9f);
		}
		this->engageDelay = 10;

		this->path = std::make_shared<JoePath>(tempPath.getAllSegments(), tempPath.isIncomplete1());
		this->movementController = std::make_unique<JoeMovementController>(path);
	});
}

void FollowPathModule::onMove(MoveInputHandler *handler) {
	if (this->movementController) {
		this->movementController->step(Game.getLocalPlayer(), Game.getMoveInputHandler());
		if (this->engageDelay > 0)
			this->engageDelay--;

		if (this->movementController->isDone()) {
			if (this->movementController->getCurrentPath()->isIncomplete1()) {
				// Replace with next path if it exists
				if (this->nextPath && !pathFinder) {
					this->clientMessageF("%sContinuing on next path...", GREEN);

					this->path = this->nextPath;
					this->nextPath.reset();
					this->movementController = std::make_unique<JoeMovementController>(path);
				} else if (!pathFinder) {
					this->setEnabled(false);
				} else if (Game.getLocalPlayer()->isInWater()) {
					Game.getLocalPlayer()->jumpFromGround();
				}
			} else {
				this->clientMessageF("%sDone!", GREEN);
				this->setEnabled(false);
				return;
			}
		} else if (!this->pathFinder && this->engageDelay == 0 && this->path && this->path->isIncomplete1() && !this->nextPath) {
			this->engageDelay = 10;

			// Estimate time to completion
			auto curPath = this->movementController->getCurrentPath();
			float timeSpent = 0;
			if (curPath->getNumSegments() == 0) {
				this->setEnabled(false);
				return;
			}
			for (size_t i = curPath->getNumSegments() - 1; i > this->movementController->getCurrentPathSegment(); i--) {
				auto cur = curPath->getSegment(i);
				timeSpent += cur.getCost();
				if (timeSpent > 11)
					break;
			}

			if (timeSpent > 11)
				return;

			this->clientMessageF("%sCalculating next path...", YELLOW);

			float timeForSearch = std::clamp(timeSpent - 0.5f, 1.f, 3.f);
			auto lastSeg = curPath->getSegment(curPath->getNumSegments() - 1);
			this->nextPath.reset();
			this->startSearch(lastSeg.getEnd(), Game.getRegion(), timeForSearch, [&](bool succeeded, JoePath tempPath) {
				if (!succeeded) {
					this->clientMessageF("%sCould not find subsequent path!", RED);

					this->engageDelay = -1;
					return;
				}

				this->clientMessageF("%sFound subsequent %s path!", tempPath.isIncomplete1() ? YELLOW : GREEN, tempPath.isIncomplete1() ? "incomplete" : "complete");

				if (tempPath.isIncomplete1()) {
					tempPath.cutoff(0.9f);
				}

				this->nextPath = std::make_shared<JoePath>(tempPath.getAllSegments(), tempPath.isIncomplete1());
			});
		}
	}
}

float smoothingTest = 0.f;
void FollowPathModule::onLevelRender() {
	if (!Game.isInGame()) {
		this->setEnabled(false);
		return;
	}

	if (this->movementController && this->path) {
		this->path->draw(this->movementController->getCurrentPathSegment());
	} /* else if (this->pathFinder) {
		JoePath localPath = this->pathFinder->getCurrentPath();
		localPath.draw(-1); // copy so we avoid drawing while its being updated by the pathfinder
	}*/
	if (Game.getLocalPlayer()) {
		Vec3 vel = Game.getLocalPlayer()->stateVector->velocity;
		vel = vel.normalize();  // Only use values from 0 - 1
		float velocityxz = Game.getLocalPlayer()->stateVector->velocity.magnitudexz();
		Vec3 blockBelow = Game.getLocalPlayer()->getEyePos();  // Block 1 block below the player
		blockBelow.y -= Game.getLocalPlayer()->aabbShape->height;
		blockBelow.y -= 0.5;

		blockBelow.z -= vel.z * 0.1f;
		blockBelow.x -= vel.x * 0.1f;
		blockBelow.z += vel.z;
		blockBelow.x += vel.x;

		Vec2 down = Game.getLocalPlayer()->getPos()->CalcAngle(blockBelow);
		if (smoothingTest > down.y)
			smoothingTest -= ((smoothingTest - down.y) / 2.f);
		else if (smoothingTest < down.y)
			smoothingTest += ((down.y - smoothingTest) / 2.f);

		Vec2 newAngle = {smoothingTest, down.y};

		if (velocityxz > 0.08f) {
			Vec2 appl = down.sub(Game.getLocalPlayer()->viewAnglesPtr->viewAngles).normAngles();
			appl.x = -appl.x;
			if ((appl.x < 180 && appl.x > -180) && (appl.y < 180 && appl.y > -180)) {
				appl.x /= (100.f - 90);
				appl.y /= (100.f - 90);
				if (appl.x >= 1 || appl.x <= -1) appl.div(abs(appl.x));
				if (appl.y >= 1 || appl.y <= -1) appl.div(abs(appl.y));
				Game.getLocalPlayer()->applyTurnDelta(&Vec2{appl.x, appl.y});
				Game.getLocalPlayer()->applyTurnDelta(&Vec2{-appl.x, appl.y});
			}
		}
	}
}