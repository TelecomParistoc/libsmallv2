#include <robotdriver/motioncontroller.h>
#include <robotdriver/speedcontroller.h>
#include <robotdriver/headingcontroller.h>
#include <robotdriver/motordriver.h>
#include <robotdriver/toolboxdriver.h>
#include <stdio.h>
#include <stdlib.h>
#include "fisharm.h"

// Ax-12 that controls the arm

#define AXFISHARM 121
#define UP        790
#define MID       600
#define DOWN      480
#define ARMSPEED        200

// Ax-12 that controls the magnet

#define AXMAGNETCONTROL 148
#define ON              532
#define OFF             300
#define MAGNETSPEED     100

static void (*captureCallback)(void) = NULL;
static void (*releaseCallback)(void) = NULL;
static void (*magnetOnCallback)(void) = NULL;
static void (*magnetOffCallback)(void) = NULL;
static void (*armUpCallback)(void) = NULL;
static void (*armMidCallback)(void) = NULL;
static void (*armDownCallback)(void) = NULL;

int hasFish() {
	return getSensor(2);
}

void fishSensorManager() {
	if(hasFish()) {
		if(captureCallback != NULL)
			captureCallback();
	} else {
		if(releaseCallback != NULL)
			releaseCallback();
	}
}

void onFishCapture(void (*callback)(void)) {
	enableSensorCallback(2);
	captureCallback = callback;
}

void onFishRelease(void (*callback)(void)) {
	enableSensorCallback(2);
	releaseCallback = callback;
}

void onMagnetOn(void (*callback)(void)) {
	magnetOnCallback = callback;
}

void onMagnetOff(void (*callback)(void)) {
	magnetOffCallback = callback;
}

void onArmUp(void (*callback)(void)) {
	armUpCallback = callback;
}

void onArmMid(void (*callback)(void)) {
	armMidCallback = callback;
}

void onArmDown(void (*callback)(void)) {
	armDownCallback = callback;
}

void waitForFish() {
	onFishCapture(armMid);
	printf("Waiting for a fish\n");
}

void initFishAx12() {
	axSetTorqueSpeed(AXMAGNETCONTROL, -1, MAGNETSPEED, 0);
	axSetTorqueSpeed(AXFISHARM      , -1, ARMSPEED   , 0);
}

void magnetOn() {
	axMove(AXMAGNETCONTROL, ON, magnetOnCallback);
}

void magnetOff() {
	axMove(AXMAGNETCONTROL, OFF, magnetOffCallback);
}

void armUp() {
	axMove(AXFISHARM, UP, armUpCallback);
}

void armMid() {
	onFishCapture(NULL);
	axMove(AXFISHARM, MID, armMidCallback);
}

void armDown() {
	axMove(AXFISHARM, DOWN, armDownCallback);
}
