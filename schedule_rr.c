#include <stdlib.h>
#include "list.h"
#include "task.h"
#include "schedulers.h"
#include "cpu.h"
#include <stdio.h>

struct node *head = NULL;

void add(char *name, int priority, int burst) { // add a task to the list
    struct task *task = malloc(sizeof(struct task)); // allocate memory for the task
    if (priority > MAX_PRIORITY || priority < MIN_PRIORITY) {
        printf("Invalid priority.\n");
        exit(1);
    } else if (burst < 1) {
        printf("Invalid burst time.\n");
        exit(1);
    } else {
        task->name = name; // set the task's name
        task->priority = priority; // set the task's priority
        task->burst = burst; // set the task's burst time

        insert(&head, task); // insert the task into the list
    }
}

void reverse(struct node **head)
{
    struct node* prev   = NULL; // defines previous node
    struct node* current = *head; // sets current node to head node
    struct node* next; // defines next node
    while (current != NULL) // while current is not, keeps swapping items until the end of the list
    {
        next  = current->next; // next node
        current->next = prev; // current node points to previous node
        prev = current; // previous node is current node
        current = next; // current node is next node
    }
    *head = prev;
}

void schedule() {
    // traverse(head); // print the list before sorting by burst time
    reverse(&head); // sort the list by priority
    // traverse(head); // print the list after sorting by burst time

    int numProcesses = 0;
    float prevWaitTime = 0, currWaitTime = 0, prevBurst = 0, totalWaitTime = 0, totalTurnaroundTime = 0;
    float avg = 0, avgTurnTime = 0;

    while (head != NULL) { // while the list is not empty
        struct node *temp = head; // temp is the current node setting it to the head
        while (temp != NULL) { // while the list is not empty
            
            int burst; // burst time
            if (temp->task->burst > 0) { // if the task has a burst time
                if (temp->task->burst > 10 ) { // if the task has a burst time greater than 10
                    burst = 10; // set the burst time to 10
                    run(temp->task, burst); // run the task
                    
                    currWaitTime = prevWaitTime + prevBurst;
                    totalWaitTime += currWaitTime;
                    totalTurnaroundTime += currWaitTime + burst; 
                    
                    prevWaitTime = currWaitTime;
                    prevBurst = burst;
                    
                    temp->task->burst -= 10; // decrease the burst time by 10
                } else {  // if the task has a burst time less than 10
                    burst = temp->task->burst; // set the burst time to the task's burst time
                    run(temp->task, burst); // run the task
                    
                    currWaitTime = prevWaitTime + prevBurst;
                    totalWaitTime += currWaitTime;
                    totalTurnaroundTime += currWaitTime + burst; 
                    
                    prevWaitTime = currWaitTime;
                    prevBurst = burst;
                    
                    temp->task->burst -= burst; // decrease the burst time by the burst time i.e bringing it to 0
                    numProcesses += 1; // add 1 to the number of processes
                    delete(&head, temp->task); //processes is deleted as burst is finished
                }
            } else {
                numProcesses += 1; // add 1 to the number of processes
                delete(&head, temp->task); //processes is deleted from the list
            }
            temp = temp->next; // move to the next node
        }
    } 

    avg = totalWaitTime / numProcesses; // calculate the average wait time
    avgTurnTime = totalTurnaroundTime / numProcesses; // calculate the average turnaround time

    printf("Average Wait Time: %.3f ms | Turnaround time: %.3f ms\n", avg, avgTurnTime);

}

/*
* References
* https://www.geeksforgeeks.org/program-round-robin-scheduling-set-1/
* https://www.javatpoint.com/program-to-sort-the-elements-of-the-singly-linked-list
*
* Name: Duarte Martinho
* Student Number: 20410974
* I acknowledge DCU's Academic Integrity Policy
*/ 