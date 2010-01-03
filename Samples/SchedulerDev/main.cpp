#include <cstdio>
#include "Scheduler.h"
#include <ctime>
#include <windows.h>

int count = 100;
clock_t t1 = 0;

scheduler::Task handle;

struct TestCPPBind
{
	int count;
	void func()
	{
		--count;
		clock_t t2 = clock();
		printf("1Times %d, elapsed %f s\n", count, float(t2-t1)*1000/CLOCKS_PER_SEC);
		t1 = t2;
		if (count==3)
		{
			scheduler::terminateTask(handle);
			handle = scheduler::addTimedTask<TestCPPBind, &TestCPPBind::func2>(this, 20);
		}
	}

	void func2()
	{
		--count;
		clock_t t2 = clock();
		printf("2Times %d, elapsed %f s\n", count, float(t2-t1)*1000/CLOCKS_PER_SEC);
		t1 = t2;

		if (!count)
		{
			scheduler::terminateLoop();
		}
	}
} test = {10};

void func(void* userData)
{
	--count;
	clock_t t2 = clock();
	printf("Times %d, elapsed %f s\n", count, float(t2-t1)*1000/CLOCKS_PER_SEC);
	t1 = t2;
	if (!count)
	{
		scheduler::terminateLoop();
	}
}

int main()
{
	HANDLE h = GetCurrentThread();
	SetThreadPriority(h, THREAD_PRIORITY_TIME_CRITICAL);
	Sleep(1000);

	scheduler::init();

	//scheduler::addTimedTask(func, 0, 20);
	handle = scheduler::addTimedTask<TestCPPBind, &TestCPPBind::func>(&test, 20);

	t1 = clock();
	scheduler::mainLoop();

	scheduler::cleanup();

	return 0;
}