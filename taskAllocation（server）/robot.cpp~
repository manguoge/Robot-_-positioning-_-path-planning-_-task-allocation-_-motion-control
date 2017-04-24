#include<iostream>
#include"robot.h"
robot::robot()
{
	;
}
robot::robot(int robotN,node robotC)
{
	robotNum=robotN;
	robotCoord=robotC;
	tasksPriceD=0;	
}

robot::~robot()
{
	
}

void robot::inserTask(int location,task newTask)
{

	list<task>::iterator iter=robotTasks.begin();
	while(location--)
		iter++;
	robotTasks.insert(iter,newTask);

}
void robot::eraseTask(int location)
{

	list<task>::iterator iter=robotTasks.begin();
	while(location--)
		iter++;
	tasking=*iter;
	robotTasks.erase(iter);

}

