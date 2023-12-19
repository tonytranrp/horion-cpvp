#pragma once
class TimerUtils {
public:
	static bool hasTimeElapsed(float sec);
	static unsigned __int64 getCurrentMs();
	static void reset();
	static float getTime();
};
