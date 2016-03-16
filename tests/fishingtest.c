#include <librobot/fisharm.h>
#include <librobot/robot.h>
#include <robotdriver/toolboxdriver.h>
#include <stdio.h>

int main () {
	initRobot();
	initFishAx12();
	onArmDown(waitForFish);
	armDown();
	while(1);
	return 0;
}
