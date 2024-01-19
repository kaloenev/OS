#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>

//What an array element should contain - an index, a thread handle and an ID. Carries information about an individual thread
//in an array element. Typedef struct - creates it as a type, easier to dynamically allocate an array in main.
typedef struct threadInfo {
    int index;
    HANDLE threadHandle;
    DWORD threadID;
} threadInfo;

DWORD WINAPI threadFunction(void *p) {
    threadInfo *p1 = (threadInfo *)p; //An assumption - threads call the function that way.
    printf("\nThread %d, %d started.", p1->index, p1->threadHandle);
	for (int i = 0; i < 10000000; i++);
    printf("\nThread %d, %d ended.", p1->index, p1->threadHandle);
    return 0;
}

int main() {

    int n; // Number of threads
    printf("Enter the number of threads: ");
    scanf_s("%d", &n);
    //Dynamically allocating an array (malloc necessary). n * sizeof - Every index divides itself in the number of 
    //parameters that are initialized in threadInfo. Creates a "3D array". Vizualised in lu2.
    threadInfo* thread_arr = (threadInfo*)malloc(n*sizeof(threadInfo));
  
	//Necessity for CREATE_SUSPENDED in my notebook (U1, 20/10/22)
	for (int l = 0; l < n; l++) {
		thread_arr[l].threadHandle = CreateThread(NULL, 0, threadFunction, thread_arr + l, CREATE_SUSPENDED, &thread_arr[l].threadID);
		thread_arr[l].index = l;
	}
	for (int l = 0; l < n; l++) {
		ResumeThread(thread_arr[l].threadHandle);
	}
	ExitThread(0); // Stops the thread without killing the process

}