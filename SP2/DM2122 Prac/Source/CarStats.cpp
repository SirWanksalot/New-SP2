#include "CarStats.h"
CarStats::CarStats() //set default to 1
{
	for (int i = 0; i < 6; ++i)
	{
		StatLevel[i] = 1.f;
		StatTRS[i].Scale = Vector3(0.f, 1.f, 1.f);
	}
	cost = 500;
	cost_upgrade = 250;
	lock = true;
}

CarStats::CarStats(float speed, float turbo, float fuel, float speedupgrade, float turboupgrade, float fuelupgrade)
{
	StatLevel[0] = speed; StatLevel[1] = turbo; StatLevel[2] = fuel;
	StatLevel[3] = speedupgrade; StatLevel[4] = turboupgrade; StatLevel[5] = fuelupgrade;

	for (int i = 0; i < 6; ++i) {
		StatTRS[i].Scale = Vector3(0.f, 1.f, 1.f);
	}
	cost = 500;
	cost_upgrade = 250;
	lock = true;
}