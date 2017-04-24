#include"allocation.h"
#include<stdlib.h>
#include<time.h>
#include<math.h>
#define MAX_XY 1000
//路程性能a和时间性能(1-a)的倾向参数
#define a 0.5

allocation::allocation()
{
	;
}
allocation::allocation(int robotsize,int Iotasksize, int Mvtasksize)
{
	//1.参数初始化
	robotSize=robotsize; 
	IOtaskSize=Iotasksize;
	MVtaskSize=Mvtasksize;
	list<robot>::iterator iter=robots.begin();
	//2.初始化机器人链表
	srand((unsigned)time(NULL));
	for(int i=1;i<=robotSize;i++)
	{
		//仓库地图坐标限制在0--MAX_XY
                double x=rand()%MAX_XY+1;
		double y=rand()%MAX_XY+1;
		cout<<"x="<<x<<" y="<<y<<endl;
		node robotC={x,y};
		robot newRobot(i,robotC);//初始化每个机器人编号及坐标
		robots.insert(iter++,newRobot);	
	}
	//3.初始化待分配的任务链表
	int temNum=0;

	while(Iotasksize||Mvtasksize)
	{
		//
		list<task>::iterator iter=unAllocTasks.begin();
		//任务坐标
		node taskStart={rand()%MAX_XY+1,rand()%MAX_XY+1};
		node taskEnd={rand()%MAX_XY+1,rand()%MAX_XY+1};
		
		int flagMV=rand()%2;//如果是0的话，产生出入库任务，如果是1的话，产生移库任务

		//产生随机出入库和移库顺序的任务序列
		switch(flagMV)
		{
		case 0:
			{
				if(Mvtasksize>0)
				{
					//如果是出入库任务，起点和终点相等
					task newTask(taskStart,taskStart,0);
					newTask.taskNum=temNum;	
					temNum++;
					//如果是1的话，为出库任务，0为入库任务
					int flagO=rand()%2;
					if(flagO)
						newTask.isOut=1;
					else 
						newTask.isOut=0;	
					unAllocTasks.insert(iter++,newTask);
					cout<<"MvtaskNum:"<<Mvtasksize<<endl;
					Mvtasksize--;
				}
				break; 	
			}	
		case 1:
			{
				if(Iotasksize>0)
				{
					task newTask(taskStart,taskEnd,1);
					newTask.taskNum=temNum;
					temNum++;
					newTask.isOut=0;
					unAllocTasks.insert(iter++,newTask);
					cout<<"IotaskNum:"<<Iotasksize<<endl;
					Iotasksize--;
				}
				break;
			}
		default:
			;
		}
		
	}
	//4.初始化出入库位置
	inNode={0,MAX_XY/2};
	outNode={MAX_XY,MAX_XY/2};
}
allocation::~allocation()
{
	;
}
void allocation::allocTask()
{	
	
	while(!unAllocTasks.empty())
	{	
		int unallocatTaskSize=unAllocTasks.size();
		//0.初始化标值列表数组
		//0.1申请行的空间
		double **price = new double*[robotSize]; 
		//0.2每行的列申请空间
		for(int i=0; i<robotSize;i++)
		{
		price[i] = new double[unallocatTaskSize];
		}
		//1.计算出价标值列表
		//当前未分配的任务的数量
		list<robot>::iterator robotIter=robots.begin();
		for(int i=0;i<robotSize;i++)
		{	
			//1.1机器人任务链表的最后一个任务，接尾法
			list<task> robotTasks=robotIter->robotTasks;
			node taskStartNodeR,taskEndNodeR;
			bool isMoveR;
			//1.2机器人的任务链表上有任务时，用机器人的任务链表的末端任务计算
			if(!robotTasks.empty())
			{
				//cout<<"!empty"<<endl;
				list<task>::iterator lastTask=--robotTasks.end();
				taskStartNodeR=lastTask->taskStart;
				//cout<<"taskStartNodeR.x:"<<taskStartNodeR.x<<endl;
				taskEndNodeR=lastTask->taskEnd;
				isMoveR=lastTask->isMoveLocation;
			}
			//1.3当机器人的任务链表为空时，用机器人的位置坐标计算
			else
			{
				taskStartNodeR=robotIter->robotCoord;
				taskEndNodeR=robotIter->robotCoord;
				isMoveR=1;
			}	

			list<task>::iterator taskIter=unAllocTasks.begin();	
			for(int j=0;j<unallocatTaskSize;j++)
			{
				//1.3.1计算新任务出价标值
				node taskStartNode=taskIter->taskStart;
				node taskEndNode=taskIter->taskEnd;
				bool isMove=taskIter->isMoveLocation;
				//1.3.2如果新任务是移库任务，自身代价保持原值
				double selfPriceD,relatedPriceD,priceD,priceT;
				if(isMove)
				{
					selfPriceD=abs(taskStartNode.x-taskEndNode.x)+abs(taskStartNode.y-taskEndNode.y);
					relatedPriceD=abs(taskEndNodeR.x-taskStartNode.x)+abs(taskEndNodeR.y-taskStartNode.y);
					
					
				}
				else
				{
					//当为出入库任务时，自身代价置为0
					selfPriceD=0;	
					relatedPriceD=abs(taskEndNodeR.x-taskStartNode.x)+abs(taskEndNodeR.y-taskStartNode.y);
				}	
				//目标性能为总路程最短
				priceD=selfPriceD+relatedPriceD;
				//目标性能为总时间最短
				priceT=robotIter->tasksPriceD+selfPriceD+relatedPriceD;
				price[i][j]=a*priceD+(1-a)*priceT;
				cout<<"price["<<i<<"]["<<j<<"]:"<<price[i][j]<<"  ";
				taskIter++;
			}
			cout<<endl;
			robotIter++;
		}
		//2.开始竞标计算,获得分配的任务编号及对应机器人的编号
		double temprice=price[0][0];
		int robotNum=0;
		int taskNum=0;
		for(int i=0;i<robotSize;i++)
		{
			for(int j=0;j<unallocatTaskSize;j++)
			{
				if(temprice>price[i][j])
				{
					temprice=price[i][j];
					robotNum=i;	
					taskNum=j;
					
				}
			}
		}
		cout<<"robotNum="<<robotNum<<"  taskNum="<<taskNum<<endl;
		//3.1将刚才分配的任务添加到对应的机器人任务链表
		list<task>::iterator taskIter=unAllocTasks.begin();
		while(taskNum--) taskIter++; 
		task newTask=*taskIter;
		robotIter=robots.begin();
		while(robotNum--) robotIter++;
		robotIter->robotTasks.push_back(newTask);
		//3.2计算累加路程代价
		double newPriceD,selfPriceD,relatPriceD;
		//在机器人任务链表还为空时，以机器人的坐标位置计算
		node backTaskEnd;
		if(robotIter->robotTasks.empty())
		{
			backTaskEnd=robotIter->robotCoord;
		}
		else
		{
			task backTask=*(--(robotIter->robotTasks.end()));
			backTaskEnd=backTask.taskEnd;
		}
		node newTaskStart=newTask.taskStart;
		node newTaskEnd=newTask.taskEnd;
		if(taskIter->isMoveLocation)
		{
			//3.2.1移库任务
			selfPriceD=abs(newTaskEnd.x-newTaskStart.x)+abs(newTaskEnd.y-newTaskStart.y);
			relatPriceD=abs(newTaskStart.x-backTaskEnd.x)+abs(newTaskStart.y-backTaskEnd.y);
			newPriceD=selfPriceD+relatPriceD;
		}
		else
		{
			//3.2.2入库任务
			if(!newTask.isOut)
			{
				selfPriceD=2*(abs(newTaskStart.x-inNode.x)+abs(newTaskStart.y-inNode.y));
				relatPriceD=abs(newTaskStart.x-backTaskEnd.x)+abs(newTaskStart.y-backTaskEnd.y);
				newPriceD=selfPriceD+relatPriceD;
			}
			//3.2.3出库任务
			else
			{
				selfPriceD=2*(abs(newTaskStart.x-outNode.x)+abs(newTaskStart.y-outNode.y));
				relatPriceD=abs(newTaskStart.x-backTaskEnd.x)+abs(newTaskStart.y-backTaskEnd.y);
				newPriceD=selfPriceD+relatPriceD;
			}
		}
		robotIter->tasksPriceD=robotIter->tasksPriceD+newPriceD;
		//3.2将刚才分配的任务从未分配任务链表中删除
		unAllocTasks.erase(taskIter);
		//4.删除刚才申请的数组内存
		for(int i=0;i<robotSize;i++)
		{
			delete [] price[i];
		}
		delete [] price;
		cout<<endl;
	}
		
	cout<<"Allocation is completed!"<<endl;
	
}

void allocation::displayResult()
{
	cout<<"the allocation result are:"<<endl;
	list<robot>::iterator robotIter=robots.begin();
	double *totalDistace=new double[robotSize],meanDistance=0,variance=0;
	for(int i=0;i<robotSize;i++)
	{
		node robotNode=robotIter->robotCoord;
		cout<<"the tasks of robot "<<i<<"[node("<<robotNode.x<<","<<robotNode.y<<")] are: ";
		list<task> tasks=robotIter->robotTasks;
		list<task>::iterator taskIter=tasks.begin();
		for(int j=0;j<tasks.size();j++)
		{
			node taskStartNode=taskIter->taskStart;
			node taskEndNode=taskIter->taskEnd;
			cout<<taskIter->taskNum<<"[isMove:"<<taskIter->isMoveLocation<<",isOut:"<<taskIter->isOut<<",startNode("<<taskStartNode.x<<","<<taskStartNode.y<<"),EndNode("<<taskEndNode.x<<","<<taskEndNode.y<<")]; ";
			taskIter++;
		}
		cout<<"the total Distance is:"<<robotIter->tasksPriceD<<endl;
		totalDistace[i]=robotIter->tasksPriceD;
		robotIter++;
	}
	double sumDistance=0;
	for(int i=0;i<robotSize;i++)	
		sumDistance+=totalDistace[i];
	meanDistance=sumDistance/robotSize;
	double tempV=0;
	for(int i=0;i<robotSize;i++)
		tempV+=pow(totalDistace[i]-meanDistance,2);
	variance=sqrt(tempV);
	cout<<"sumDistance:"<<sumDistance<<endl<<"meanDistance:"<<meanDistance<<endl<<"variance:"<<variance<<endl;
}


























