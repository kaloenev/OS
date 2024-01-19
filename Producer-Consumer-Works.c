#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 100

int buffer[SIZE];
HANDLE Se;
HANDLE Sf;
HANDLE Mutex1;
HANDLE Mutex2;
HANDLE mutex_r;
int random;

typedef struct threadInfo {
	int threadIndex;
	HANDLE threadHandle;
	DWORD threadID;
} threadInfo;

int random_num() {
	int m;
	WaitForSingleObject(mutex_r, INFINITE);
	m = (rand() % 1000) + 1;
	ReleaseMutex(mutex_r);
	return m;
}

DWORD WINAPI Producer(void* p) {
	threadInfo* p1 = (threadInfo*)p;
	printf("\nPRODUCER %d, %d started: ", (int)p1->threadIndex, (int)p1->threadHandle);
	for (;;) {
		//Not sure if this is the critical section. Check if errors.
		Sleep((rand() % 5000) + 5000);
		random = random_num(); //Generates the random number up to 1000. Idk why I couldn't figure it out in the first place.
		int index = 0;
		WaitForSingleObject(Se, INFINITE); // Checks if there's at least one empty element to write in.
		WaitForSingleObject(Mutex1, INFINITE);
		for (int i = 0; i < SIZE; i++) {
			if (buffer[i] == 0) {
				buffer[i] = random;
				index = i;
				break;
			}
		}
		// Goes through the entire array. When there's an empty element it writes down the value. Index marks in which place the value was written.
		// Break so it doesn't loop unnecessairly too many times. Gives the info to consumer thread through the semaphores.
		ReleaseMutex(Mutex1);
		ReleaseSemaphore(Sf, 1, NULL); //Increment with one when there's something written.
		printf("\nProducer thread %d, Buffer index %d = %d", (int)p1->threadIndex, index, random);
	}
	printf("\nPRODUCER %d, %d terminated: ", (int)p1->threadIndex, (int)p1->threadHandle);
	return 0;
}

DWORD WINAPI Consumer(void* p) {
	threadInfo* p1 = (threadInfo*)p;
	printf("\nCONSUMER %d, %d started: ", (int)p1->threadIndex, (int)p1->threadHandle);
	for (;;) {
		//Not sure if this is the critical section. Check if errors. - Edit: Critical section is okay.
		Sleep((rand() % 5000) + 5000); //Puts the thread to sleep for a given amount of miliseconds. rand % 5000 - between 0-4999. 
		int index = 0;
		int temp;
		WaitForSingleObject(Sf, INFINITE);
		WaitForSingleObject(Mutex2, INFINITE);
		for (int i = 0; i < SIZE; i++) {
			if (buffer[i] != 0) {
				temp = buffer[i];
				buffer[i] = 0;
				index = i;
				break;
			}
		}
		// Temp holds the recorded value in the array. Empties it. Marks the index of the array that is emptied out.
		ReleaseMutex(Mutex2);
		ReleaseSemaphore(Se, 1, NULL);
		printf("\nConsumer thread %d, Buffer index %d = %d", (int)p1->threadIndex, index, temp);
	}
	printf("\nCONSUMER %d, %d terminated: ", (int)p1->threadIndex, (int)p1->threadHandle);
	return 0;
}

int main() {
	int n, c;
	printf("Enter the number of producer threads: ");
	scanf_s("%d", &n);
	printf("\nEnter the number of consumer threads: ");
	scanf_s("%d", &c);
	if (n >= c) {
		//Initializing every element in the buffer to 0 using loop:
		for (int i = 0; i < SIZE; i++) {
			buffer[i] = 0;
		}
		Se = CreateSemaphore(NULL, SIZE, SIZE, NULL);
		Sf = CreateSemaphore(NULL, 0, SIZE, NULL);
		Mutex1 = CreateMutex(NULL, FALSE, NULL);
		Mutex2 = CreateMutex(NULL, FALSE, NULL);
		mutex_r = CreateMutex(NULL, FALSE, NULL);
		threadInfo* thread_arr = (threadInfo*)calloc(c, sizeof(threadInfo));
		threadInfo* thread_arr1 = (threadInfo*)calloc(n, sizeof(threadInfo));
		for (int l = 0; l < n; l++) {
			thread_arr1[l].threadHandle = CreateThread(NULL, 0, Producer, thread_arr1 + l, CREATE_SUSPENDED, &thread_arr1[l].threadID);
			thread_arr1[l].threadIndex = l;
		}
		for (int l = 0; l < c; l++) {
			thread_arr[l].threadHandle = CreateThread(NULL, 0, Consumer, thread_arr + l, CREATE_SUSPENDED, &thread_arr[l].threadID);
			thread_arr[l].threadIndex = l;
		}
		for (int l = 0; l < c; l++) {
			ResumeThread(thread_arr[l].threadHandle);
		}
		for (int l = 0; l < n; l++) {
			ResumeThread(thread_arr1[l].threadHandle);
		}
	}
	else {
		printf("\nThe number of producers should be larger than the one of consumers.");
		// Consumer thread might not manage to make it to a larger array index because consumer number < producer number. 
		// If the consumer threads are more than the producer threads, this problem won't arise.
	}
	ExitThread(0);
	return 0;
}