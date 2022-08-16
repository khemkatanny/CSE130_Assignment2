/************************************************************************
 *
 * CSE130 Assignment 2
 *
 * Copyright (C) 2020-2022 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 *
 ************************************************************************/

// Sources:
// https://man7.org/linux/man-pages/man3/pthread_create.3.html
// https://stackoverflow.com/questions/6990888/c-how-to-create-thread-using-pthread-create-function

#include "merge.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define THREADS_NUM 4

void singleThreadedMergeSort(int *, int, int);

struct sort_args {
  int left;
  int right;
  int *arr;
} sort_args;

void *thread_func_1(void *arg) 
{
  //accessing thread arguments
  struct sort_args *arg_thread = (struct sort_args *) arg;
  int left = arg_thread->left;
  int right = arg_thread->right;
  int *arr = arg_thread->arr;
  singleThreadedMergeSort(arr, left, right);
  pthread_exit(NULL);
}

void *thread_func_2(void *arg) 
{
  //accessing thread arguments
  struct sort_args *arg_thread2 = (struct sort_args *) arg;
  int left = arg_thread2->left;
  int right = arg_thread2->right;
  int *arr = arg_thread2->arr;

  //singleThreadedMergeSort(arr, ((left+right)/2)+1, right);
  pthread_t threads[THREADS_NUM/2];
  struct sort_args t_args[THREADS_NUM/2];

  t_args[0].left = left;
  t_args[0].right = ((left+right)/2);
  t_args[0].arr = arr;
  t_args[1].left = (left+right)/2 + 1;
  t_args[1].right = right;
  t_args[1].arr = arr;
  
  //creating threads to sort the two arrays
  for (int i = 0; i < THREADS_NUM/2; i++)
  {
    pthread_create(&threads[i], NULL, thread_func_1, (void*)&t_args[i]); 
  }

  //joining and waiting for them to finish
  for(int i = 0; i < THREADS_NUM/2; i++)
  {
    pthread_join(threads[i], NULL);
  }

  merge(arr, left, (left+right)/2, right);
  
  return NULL;
}


/* LEFT index and RIGHT index of the sub-array of ARR[] to be sorted */
void singleThreadedMergeSort(int arr[], int left, int right) 
{
  if (left < right) 
  {
    int middle = (left+right)/2;
    singleThreadedMergeSort(arr, left, middle); 
    singleThreadedMergeSort(arr, middle+1, right); 
    merge(arr, left, middle, right); 
  } 
}

/* 
 * This function stub needs to be completed
 */
void multiThreadedMergeSort(int arr[], int left, int right) 
{
  // Delete this line, it's only here to fail the code quality check
  //int i = 0;

  // Your code goes here 
  pthread_t threads[THREADS_NUM/2];
  struct sort_args t_args[THREADS_NUM/2];

  t_args[0].left = left;
  t_args[0].right = ((left+right)/2);
  t_args[0].arr = arr;
  t_args[1].left = (left+right)/2 + 1;
  t_args[1].right = right;
  t_args[1].arr = arr;
  
  //creating threads to sort the two arrays
  for (int i = 0; i < THREADS_NUM/2; i++)
  {
    pthread_create(&threads[i], NULL, thread_func_2, (void*)&t_args[i]); 
  }

  //joining and waiting for them to finish
  for(int i = 0; i < THREADS_NUM/2; i++)
  {
    pthread_join(threads[i], NULL);
  }

  merge(arr, left, (left+right)/2, right);

}
