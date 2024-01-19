#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <math.h>

int count = 0;
HANDLE globalh;

typedef struct threadInfo {
	int Index;
	HANDLE threadHandle;
	DWORD threadID;
} threadInfo;

DWORD WINAPI threadFunction(void* p) {
	threadInfo* p1 = (threadInfo*)p;
	printf("\nThread %d, %d started.", (int)p1->Index, (int)p1->threadHandle);
	for (int i = 0; i < 1000; i++) {
		globalh = p1->threadHandle;
		for (int i = 0; i < 100; i++);
		if (globalh != p1->threadHandle) {
			count++;
		}
	}
	printf("\nThread %d, %d terminated.", (int)p1->Index, (int)p1->threadHandle);
	return 0;
}

int main() {
	int n;
	int g;
	printf("Enter the number of threads: ");
	scanf_s("%d", &n);
	threadInfo* thread_arr = (threadInfo*)calloc(n, sizeof(threadInfo));
	for (int l = 0; l < n; l++) {
		thread_arr[l].threadHandle = CreateThread(NULL, 0, threadFunction, thread_arr + l, CREATE_SUSPENDED, &thread_arr[l].threadID);
		thread_arr[l].Index = l;
	}
	for (int l = 0; l < n; l++) {
		ResumeThread(thread_arr[l].threadHandle);
	}
	scanf_s("%d", &g);
	printf("\nCount: %d", count);
	scanf_s("%d", &g);
	ExitThread(0);
	return 0;
}