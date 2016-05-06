#include <robotdriver/speedcontroller.h>
#include <robotdriver/motioncontroller.h>
#include <robotdriver/toolboxdriver.h>
#include <robotdriver/motordriver.h>
#include <librobot/robot.h>
#include <iostream>
#include <chrono>


#include "WaysAndActions.hpp"


void pauseActionOrWay()
{
	if(!PathFollower::isPaused())
	{
		blocked = true;
		pausePath = false;
		if(isMovingToAction)
		{
			pausePath = true;
			PathFollower::pause();
		}
		else
			actions[way].pauseAction();
	}
}

int main()
{
	initRobot();
	initWaysAndActions();

	onGameStop(&allume);

	onCollisionDetect(&checkCollisionAndReact);

	setMoveStartCallback(&PathFollower::updateAngleStartingMove);
	setMoveEndCallback(&PathFollower::updatePositionEndingMove);
	PathFollower::resetPosition(curPos);

	clk_start = Clock::now();

	while(getTime()<seconds*1000||(start&&!started))
	{
		if(!start)
			clk_start = Clock::now();

		if(start&&!started)
		{
			printf("Start %f\n",getTime());
			started = true;
			clk_start = Clock::now();
			printf("Get time after : %f\n",getTime());
		}
		else if(started)
		{
			if(!isMovingToAction)
				if(actions[way].isFinished())
				{
					actions[way].stopAction();
					endAction();
				}

			if(seeBlocked)
			{
				if(PathFollower::isSpeedPositive())
				{
					if(!isRobotFront())
						seeBlocked = false;
					else
						pauseActionOrWay();
				}
				else
				{
					if(!isRobotBehind())
						seeBlocked = false;
					else
						pauseActionOrWay();
				}
			}

			if(!seeBlocked)
				if(blocked)
				{
					blocked = false;
					if(pausePath)
						PathFollower::continueMoving();
					else
						actions[way].continueAction();
				}

			curPos = PathFollower::getCurrentPos();
			/*std::cout<<"---------------"<<std::endl;
			std::cout<<"Cur action : "<<way<<" => "<<actions[way].isFinished()<<std::endl;*/
			//std::cout<<curPos.first<<" "<<curPos.second<<std::endl;
			//std::cout<<"is blocked ? "<<blocked<<" is paused moving ? "<<pausePath<<" see blocked ? "<<seeBlocked<<std::endl;
		}

		waitFor(50);
	}

	endCallback();

	return 0;
}
