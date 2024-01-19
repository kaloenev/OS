#include <stdio.h>
#include <math.h>
#include <Windows.h>
#include <stdlib.h>

typedef struct threadInfo {
	HANDLE threadHandle;
	DWORD threadID;
	int Index;
	int Start_Index, End_Index, Sum;
} threadInfo;
int* arr;
DWORD WINAPI threadFunction(void* p) {
	threadInfo* p1 = (threadInfo*)p;
	printf("\nThread %d, %d started: ", (int)p1->Index, (int)p1->threadHandle);
	for (int f = p1->Start_Index; f <= p1->End_Index; f++) {
		p1->Sum += arr[f];
	}
	printf("\nThread %d, %d eliminated: ", (int)p1->Index, (int)p1->threadHandle);
	return 0;
}
int main() {
	int g;
	int n;
	int total = 0;
	printf("\nNumber of threads: ");
	scanf_s("%d", &n);
	int m;
	printf("\nNumber of elements in array: ");
	scanf_s("%d", &m);
	arr = (int*)calloc(m, sizeof(int));
	threadInfo* thread_arr = (threadInfo*)calloc(n, sizeof(threadInfo));
	int r = (m / n);
	for (int i = 0; i < m; i++) {
		arr[i] = i;
	}
	for (int l = 0; l < n; l++) {
		thread_arr[l].threadHandle = CreateThread(NULL, 0, threadFunction, thread_arr + l, CREATE_SUSPENDED, &thread_arr[l].threadID);
		thread_arr[l].Index = l;
		thread_arr[l].Start_Index = (l * r);
		thread_arr[l].End_Index = ((l * r) + r - 1);
		thread_arr[l].Sum = 0;
	}
	thread_arr[n - 1].End_Index = m - 1;
	for (int l = 0; l < n; l++) {
		ResumeThread(thread_arr[l].threadHandle);
	}
	scanf_s("%d", &g);
	for (int l = 0; l < n; l++) {
		printf("\nThread %d sum: %d", thread_arr[l].Index, thread_arr[l].Sum);
		total += thread_arr[l].Sum;
	}
	printf("\nTotal sum: %d", total);
	ExitThread(0);
	return 0;
}