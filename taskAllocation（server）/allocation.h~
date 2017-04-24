#pragma once
#include<list>
#include"task.h"
#include"robot.h"
/*
任务分配类
进行任务分配计算
1.初始化机器人数量和任务数量
2.
*/
class allocation
{
public:
	int robotSize;//机器人数量 
	int IOtaskSize;//待分配出入库任务数量
	int MVtaskSize;//待分配移库任务数量
	list<robot> robots;//机器人链表
	node inNode;//仓库入口 
	node outNode;//仓库出口
	list<task> unAllocTasks;//待分配的任务链表
	allocation();
	allocation(int robotsize,int Iotasksize, int Mvtasksize);//初始化任务和机器人链表
	~allocation();
	void allocTask();//任务分配函数
	void displayResult();//显示任务分配结果
	
private:
	;
};
