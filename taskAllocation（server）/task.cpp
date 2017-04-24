
#include"task.h"

using namespace std;
task::task()
{
	;
}
task::task(node start,node end, bool isMove)
{
	taskStart=start;
	taskEnd=end;
	isMoveLocation=isMove;
}
task::~task()
{
	;
}

