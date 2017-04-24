#pragma once
#include<iostream>
#include<list>

typedef struct
{
	double x;
	double y;
}node;

class task
{
public:
	int taskNum;//任务编号
	node taskStart;//任务起点
	node taskEnd;//任务终点（针对移库任务）,如果是出入库任务，起点和终点相等,方便计算
	bool isMoveLocation;//是否是移库任务,默认为0出入库任务
	bool isOut;//是否是出库任务，0表示入库，1表示出库
	task();
	task(node start,node end, bool isMove);
	~task();
private:
	
	;
};
