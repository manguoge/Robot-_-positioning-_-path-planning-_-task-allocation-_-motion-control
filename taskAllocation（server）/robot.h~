#pragma once
#include<iostream>
#include"task.h"
#include<list>
using namespace std;

class robot
{
public:
	int robotNum;//机器人编号
	node robotCoord;
	list<task> robotTasks;//每个机器人对应的已分配到它身上的任务链表
	double tasksPriceD;//任务链表的总路程代价
	task tasking;//机器人正在执行的任务
	robot();
	robot(int robotN,node robotC);
	~robot();
	/*在任务序列的location位置插入新任务,
	机器人任务链表的第一个任务代表机器人当前正在执行的任务,所以插入的位置location从1开始		
	*/
	void inserTask(int location,task newTask);
	/*在任务序列的location位置取出任务,放到正在执行的tasking变量任务里
	当执行完链表里的头任务时，删除第一个任务	
	*/
	void eraseTask(int location);
private:

};
