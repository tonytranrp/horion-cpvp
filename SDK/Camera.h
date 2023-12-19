#pragma once

#include "../Utils/HMath.h"

class Camera {
private:
	uint8_t pad_0000[0x30]{};  // 0x0000
public:
	Vec4 lookingAt;            // 0x0030
	Vec3 cameraPos;            // 0x0040
	float uiScale;             // 0x004C
	float fieldOfView;         // 0x0050
	float nearClippingPlane;   // 0x0054
	float farClippingPlane;    // 0x0058
	glm::mat4 nauseaModifier;  // 0x005C
private:
	uint8_t pad_006C[0x34]{};
public:
	Vec4 someRotationThing;    // 0x00D0


	void getForwardVector(Vec3*);
	void getEulerAngles(Vec3*);
	void getPlayerRotation(Vec2*);
	void setOrientation(float pitch, float yaw, float roll);
	void setOrientationDeg(float pitch, float yaw, float roll) {
		setOrientation((-yaw - 180) * RAD_DEG, -pitch * RAD_DEG, roll * RAD_DEG);
	}
};

class CameraManager {
public:
	Camera* getCameraOrDebugCamera();
};