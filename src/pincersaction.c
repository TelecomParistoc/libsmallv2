#include "robot.h"
#include "pincers.h"
#include "pincersaction.h"
#include <robotdriver/motioncontroller.h>
#include <robotdriver/speedcontroller.h>
#include <robotdriver/headingcontroller.h>
#include <robotdriver/imudriver.h>
#include <stdlib.h>
#include <stdio.h>

static int first = 1;
static int finished = 0;

static double (*getTimeCallback)(void) = NULL;

int pincersHasFinished(){
	return finished;
}

void pincersStop(){
	finished = 0;
}

static void finish(){
	finished = 1;
}

static void deliver(){
	setPosInCorner(getHeading());
	if(first){
		first = 0;
		onOpenPincers(faceShell);
		finish();
	} else {
		onOpenPincers(closePincers);
		finish();
	}
}

static void back(){
	setRobotDistance(0);
	queueSpeedChange(-0.15, NULL);
	queueStopAt(-70, deliver);
}

static void stopAndCatch(){
	setBlockingCallback(NULL);
	fastSpeedChange(0);
	enableHeadingControl(1);
	tryCapture();
}

static void getShell(){
	if(!first) {
		if(getTeam() == GREEN_TEAM)
			setActiveDetectors(right);
		else
			setActiveDetectors(left);
	}
	setBlockingCallback(stopAndCatch);
	enableHeadingControl(0);
	queueSpeedChange(0.15, NULL);
}

void faceShell(){
	onTryCapture(back);
	onOpenPincers(getShell);
	setActiveDetectors(none);
	if(getTableConfig() != 4){
		if(first)
			setTargetHeading(200, openPincers);
		else
			setTargetHeading(240, openPincers);
	}
}

void setGetTimeCallback(double (* callback)(void)){
	getTimeCallback = callback;
}
