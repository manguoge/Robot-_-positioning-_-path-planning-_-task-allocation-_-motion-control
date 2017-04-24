#include<iostream>
#include<stdlib.h>
#include<list>
#undef alloca
#include"task.h"
#include"robot.h"
#include"allocation.h"
#include<time.h>
using namespace std;
/*
main program
1.构建地图，
2.
*/

int main(int argc,char* argv[])
{
		
	//耗时计算
	time_t startTime,finishTime;
	double duration;
	startTime=time(NULL);
	int robotsize,Iotasksize,Mvtasksize;
	if(argc==4)
	{
		robotsize=atoi(argv[1]),Iotasksize=atoi(argv[2]), Mvtasksize=atoi(argv[3]);
		cout<<"the number of robots:"<<robotsize<<endl;
		cout<<"the number of IOtasks:"<<Iotasksize<<endl;
		cout<<"the number of MVtasks:"<<Mvtasksize<<endl;
	}
	else
	{
		cout<<"the input of arguements is error!!"<<endl;
	}
//test class task
	cout<<"taskAlloction Algorithm:Auction"<<endl;
	node start={1,1};
	node end={2,2};
	task unAllocTask(start,end,0);//unfinshed tasks	
	cout<<"unAllocTask.taskStart.x:"<<unAllocTask.taskStart.x<<endl;
//test class robot
	robot robot1(2,start);
	cout<<"robot1.robotNum:"<<robot1.robotNum<<endl;
//unAllocTask
	list<task> unAllocTasks;
	
//allocation
	allocation alloca(robotsize,Iotasksize,Mvtasksize);
	alloca.allocTask();
	alloca.displayResult();
	finishTime=time(NULL);
	duration=(double)(finishTime-startTime);
	cout<<"the run time is:"<<duration<<"s!"<<endl;
	return 0;
}


















