#include "Scheduler.h"
#include <queue>
#include <cassert>
#include <ctime>
#include <windows.h>
#include <list>
#include <algorithm>
#include <sdl.h>

namespace mt
{
	enum TaskFlags
	{
		SCHED_DESC_IMMEDIATE=0,
		SCHED_DESC_TIMED=1,
		SCHED_MASK_TASK_TIME_MODE=1,

		SCHED_DESC_ONE_TIME=0,
		SCHED_DESC_RECURRING=2,
		SCHED_MASK_TASK_REPEAT_MODE=2,

		SCHED_DESC_STOP_TASK=4,
		SCHED_MASK_TASK_CONTROL=4,
	};

	enum SchedEntryType
	{
		SCHED_ENTRY_TASK,
		SCHED_ENTRY_TIMED_TASK,
		SCHED_ENTRY_JOB,
	};

	struct SchedEntry
	{
		size_t			flags;
		SchedEntryType	type;
		union
		{
			struct
			{
				TaskFunc	taskFunc;
				void*		userData;
				size_t		frameToExecute;
			} taskDesc;

			struct
			{
				TimedTaskFunc	taskFunc;
				void*			userData;
				size_t			msTimeout;
				size_t			msDueTime;
			} timedTaskDesc;
		};
	};

	struct TaskPool
	{
		std::list<SchedEntry*>	inactiveTasks;

		static void delTask(SchedEntry* entry) {delete entry;}

		~TaskPool()
		{
			std::for_each(inactiveTasks.begin(), inactiveTasks.end(), &TaskPool::delTask);
		}

		SchedEntry* get()
		{
			if (!inactiveTasks.empty())
			{
				SchedEntry* entry = inactiveTasks.back();
				inactiveTasks.pop_back();
				return entry;
			}
			else
				return new SchedEntry();
		}

		void put(SchedEntry* entry)
		{
			inactiveTasks.push_back(entry);
		}
	};

	struct TaskHandle
	{
		SchedEntry*	entry;

		TaskHandle(SchedEntry* other)
		{
			entry = other;
		}

		operator SchedEntry&()
		{
			return *entry;
		}

		TaskHandle& operator=(SchedEntry* other)
		{
			entry = other;
		}
	};

	struct compareTimedTasks
	{
		bool operator()(const TaskHandle& left, const TaskHandle& right) const
		{
			assert(left.entry->type==SCHED_ENTRY_TIMED_TASK && right.entry->type==SCHED_ENTRY_TIMED_TASK);
			return left.entry->timedTaskDesc.msDueTime > right.entry->timedTaskDesc.msDueTime;
		}
	};

	struct compareTasks
	{
		bool operator()(const TaskHandle& left, const TaskHandle& right) const
		{
			assert(left.entry->type==SCHED_ENTRY_TASK && right.entry->type==SCHED_ENTRY_TASK);
			return left.entry->taskDesc.frameToExecute > right.entry->taskDesc.frameToExecute;
		}
	};

	//bool operator<(const TaskHandle& lhs, const TaskHandle& rhs)
	//{
	//	assert(lhs.entry->type==SCHED_ENTRY_TIMED_TASK && rhs.entry->type==SCHED_ENTRY_TIMED_TASK);
	//	return lhs.entry->timedTaskDesc.msDueTime > rhs.entry->timedTaskDesc.msDueTime;
	//}

	bool		isInitialized=false;
	bool		doTerminate=false;
	size_t		curFrame=0;
	clock_t		startTime=0;
	TaskPool	taskPool;

	std::priority_queue<TaskHandle, std::vector<TaskHandle>, compareTimedTasks>	timedTaskQueue;
	std::priority_queue<TaskHandle, std::vector<TaskHandle>, compareTasks>		taskQueue;

	size_t	timeElapsedMS()
	{
		assert(isInitialized);
		return (clock()-startTime)*1000/CLOCKS_PER_SEC;
	}

	void init()
	{
		if (isInitialized)
			return;

		isInitialized = true;
		doTerminate=false;
		assert(timedTaskQueue.empty());
		assert(taskQueue.empty());
		startTime = clock();
		curFrame = 0;
	}

	void cleanup()
	{
		if (!isInitialized)
			return;

		while (!timedTaskQueue.empty())
		{
			taskPool.put(timedTaskQueue.top().entry);
			timedTaskQueue.pop();
		}
		while (!taskQueue.empty())
		{
			taskPool.put(taskQueue.top().entry);
			taskQueue.pop();
		}

		isInitialized = false;
		doTerminate=true;
	}

	//void addTask(TaskFunc code, void* userData/*, size_t flags*/)
	//{
	//}

	//Adds timed task to execution queue
	Task addTimedTask(TimedTaskFunc func, void* userData, size_t msTimeout/*, size_t flags*/)
	{
		SchedEntry* entry = taskPool.get();
		entry->flags = SCHED_DESC_RECURRING|SCHED_DESC_TIMED;
		entry->type  = SCHED_ENTRY_TIMED_TASK;
		entry->timedTaskDesc.taskFunc = func;
		entry->timedTaskDesc.userData = userData;
		entry->timedTaskDesc.msTimeout = msTimeout;
		entry->timedTaskDesc.msDueTime = timeElapsedMS();
		timedTaskQueue.push(entry);

		return entry;
	}

	Task addFrameTask(TaskFunc func, void* userData/*, size_t flags*/)
	{
		SchedEntry* entry = taskPool.get();
		entry->flags = SCHED_DESC_RECURRING|SCHED_DESC_IMMEDIATE;
		entry->type  = SCHED_ENTRY_TASK;
		entry->taskDesc.taskFunc = func;
		entry->taskDesc.userData = userData;
		entry->taskDesc.frameToExecute = curFrame;
		taskQueue.push(entry);

		return entry;
	}

	void terminateLoop()
	{
		doTerminate = true;
	}

	void terminateTask(Task handle)
	{
		handle->flags |= SCHED_DESC_STOP_TASK;
	}

#include "Timer.h"

	void mainLoop()
	{
		while(!doTerminate)
		{
			size_t time = timeElapsedMS();

			while (!timedTaskQueue.empty())
			{
				SchedEntry& entry = timedTaskQueue.top();
				if (entry.timedTaskDesc.msDueTime>time)
					break;
				assert(entry.type == SCHED_ENTRY_TIMED_TASK);
				timedTaskQueue.pop();
				if (entry.flags&SCHED_DESC_STOP_TASK)
				{
					taskPool.put(&entry);
				}
				else
				{
					entry.timedTaskDesc.taskFunc(entry.timedTaskDesc.userData);
					entry.timedTaskDesc.msDueTime += entry.timedTaskDesc.msTimeout;
					if ((entry.flags&SCHED_MASK_TASK_REPEAT_MODE) == SCHED_DESC_RECURRING)
						timedTaskQueue.push(&entry);
				}
			}

			while (!taskQueue.empty())
			{
				SchedEntry& entry = taskQueue.top();
				if (entry.taskDesc.frameToExecute>curFrame)
					break;
				assert(entry.type == SCHED_ENTRY_TASK);
				taskQueue.pop();
				if (entry.flags&SCHED_DESC_STOP_TASK)
				{
					taskPool.put(&entry);
				}
				else
				{
					entry.taskDesc.taskFunc(entry.taskDesc.userData);
					++entry.taskDesc.frameToExecute;
					if ((entry.flags&SCHED_MASK_TASK_REPEAT_MODE) == SCHED_DESC_RECURRING)
						taskQueue.push(&entry);
				}
			}
			++curFrame;
		}
	}

	Thread createThread(int (__cdecl *fn)(void *), void *data)
	{
		return SDL_CreateThread(fn, data);
	}

	void waitThread(Thread thread, int *status)
	{
		SDL_WaitThread(thread, status);
	}

	Mutex createMutex()
	{
		return SDL_CreateMutex();
	}

	int lockMutex(Mutex mtx)
	{
		return SDL_LockMutex(mtx);
	}

	int unlockMutex(Mutex mtx)
	{
		return SDL_UnlockMutex(mtx);
	}

	void destroyMutex(Mutex mtx)
	{
		SDL_DestroyMutex(mtx);
	}
}
