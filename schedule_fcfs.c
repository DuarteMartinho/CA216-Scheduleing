#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "task.h"
#include "schedulers.h"
#include "cpu.h"

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
    // traverse(head); // print the list before reversing
    reverse(&head);
    // traverse(head); // print the list after reversing

    int numProcesses = 0, totalWaitTime = 0, totalTurnaroundTime = 0;
    int prevWaitTime = 0, currWaitTime = 0, prevBurst = 0;
    float avg = 0, avgTurnTime = 0;

    struct node *temp = head; // temp is the current node setting it to the head
    while (temp != NULL) { // while the list is not empty
        run(temp->task, temp->task->burst); // run the task
        
        currWaitTime = prevWaitTime + prevBurst;
        totalWaitTime += currWaitTime;
        totalTurnaroundTime += currWaitTime + temp->task->burst;
        prevWaitTime = currWaitTime;
        prevBurst = temp->task->burst;

        numProcesses += 1; // add 1 to the number of processes
        temp = temp->next; // move to the next node
    }

    avg = totalWaitTime / numProcesses; // calculate the average wait time
    avgTurnTime = totalTurnaroundTime / numProcesses; // calculate the average turnaround time

    printf("Average Wait Time: %.3f. Turnaround time: %.3f\n", avg, avgTurnTime);
}

/*
* References
* Linked List Reverse - https://www.geeksforgeeks.org/data-structures-linked-list-question-5/
*
* Name: Duarte Martinho
* Student Number: 20410974
* I acknowledge DCU's Academic Integrity Policy
*/  