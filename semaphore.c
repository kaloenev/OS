#include "pch.h"
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

HANDLE semaph;

struct threadinfo
{
      int index;
      HANDLE h;
      DWORD tid;
};



DWORD WINAPI thread_func(void* p)
{
      int m;
      threadinfo* p1 = (threadinfo*)p;
      printf("thread %d started handle = %d\n ", p1->index, (int)p1->h);
      for (m = 0; m < 1000000; m++);

      printf("thread %d terminated handle = %d\n ", p1->index, (int)p1->h);
      ReleaseSemaphore(semaph, 1, NULL);

      return 0;
}



int main()
{
      int n, j, i;
      printf("Enter threads count:\n");
      scanf_s("%d", &n);

      semaph = CreateSemaphore(NULL, 0, n, 0);
      struct threadinfo* thread_arr;
      thread_arr = (struct threadinfo*) malloc(n * sizeof(struct threadinfo));
      for (i = 0; i < n; i++) {
            thread_arr[i].h = CreateThread(NULL, 0, thread_func, thread_arr + i, CREATE_SUSPENDED, &thread_arr[i].tid);
            thread_arr[i].index = i;
      }
      for (j = 0; j < n; j++) {
            ResumeThread(thread_arr[j].h);
      }
      for (int k = 0; k < n; k++) {
            //WaitForSingleObject(semaph, INFINITE);
            WaitForSingleObject(thread_arr[k].h, INFINITE);
      }
      printf("\n main thread terminated");
      ExitThread(0);
}

