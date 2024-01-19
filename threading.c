#include <iostream>
#include <thread>
#include <Windows.h>


struct threadings {
	HANDLE h;
	DWORD tid;
	
};
int notSynchThreadsCounter = 0;
DWORD currentThreadId;
HANDLE mutex;
DWORD WINAPI threadFunctionName(void* p) {

	threadings* p1 = (threadings*)p;
	printf("\nthread %d started", p1->h);
	WaitForSingleObject(mutex, INFINITE);
	for (size_t i = 0; i < 5000; i++)
	{
		
		currentThreadId = p1->tid;
		for (size_t j = 0; j < 1; j++);
		if (p1->tid != currentThreadId) {

			notSynchThreadsCounter++;
		}
	}
	ReleaseMutex(mutex);
	printf("\nthread %d terminated", p1->h);
	return 0;
};
int main() {
	int n;
	printf("Type amount of threads that you want to have!\n");
	scanf_s("%d", &n);
	printf("You want %d threads!", n);
	mutex = CreateMutex(0, FALSE, 0);
	threadings* allThreads = new threadings[n];
	for (size_t i = 0; i < n; i++)
	{
		if (&allThreads[i].tid == NULL) {
			printf("Error");
		}
		else {
			allThreads[i].h = CreateThread(NULL, 0, threadFunctionName, &allThreads[i], CREATE_SUSPENDED, &allThreads[i].tid);
		}
	}
	for (size_t i = 0; i < n; i++)
	{
		ResumeThread(allThreads[i].h);
	}
	printf("main thread terminated!");
	system("pause");
	printf("Not Sync Threads: %d", notSynchThreadsCounter);
	ExitThread(0);

}





