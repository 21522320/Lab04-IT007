/* *
 * UIT
 * Team: WINter
 * Course ID: IT007.N11.2
 * File: SJF.c
 * */

#include <stdio.h>
#include <stdlib.h>

#define MAX 10

struct Process
{
    int pn;    // process name
    int arr;   // arrival time
    int bur;   // burst time
    int start; // start time
    int fin;   // finish time
    int tat;   // turnaround time
    int wt;    // waiting time
    int rt;     // response time

    int order;
};
typedef struct Process Process;

struct PStack
{
    Process s[MAX];
    int top;
};
typedef struct PStack PStack;

PStack New, Ready, Term; // create 3 queue, corresponding to new queue, ready queue and terminated queue

int comp_arrival(const void *p, const void *q);
int comp_burst(const void *p, const void *q);

void main()
{
    // initialize queue
    New.top = -1;
    Ready.top = -1;
    Term.top = -1;

    int i, n;
    int totwt = 0, tottat = 0; // total waiting time, total turnaround time

    // input
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    Process temp;
    for (i = 0; i < n; i++)
    {
        printf("Enter the Process Name, Arrival Time, Burst Time: ");
        scanf("%d%d%d", &temp.pn, &temp.arr, &temp.bur);
        // append(New, temp)
        New.top++;
        New.s[i] = temp;
        New.s[i].order = i;
    }

    // sorting New by arrival time with quick sort algo
    qsort((void *)New.s, n, sizeof(Process), comp_arrival);
    // update order to stablize the order  
    for (i = 0; i < n; i++)
        New.s[i].order = i;

    int curr_arr_time = 0;
    i = 0;
    int j = 0;

    for (i = 0; i < n; i++)
    {
        curr_arr_time = New.s[i].arr;

        // add to Ready queue all processes that arrive at the same time
        while (j < n && curr_arr_time == New.s[j].arr)
        {
            // append(Ready, New.s[j])
            Ready.top++;
            Ready.s[Ready.top] = New.s[j];

            New.top--; // pop
            j++;
        }

        // sorting current Ready by burst time with quick sort algo
        qsort((void *)Ready.s, Ready.top + 1, sizeof(Process), comp_burst);
        
        // calculating
        if (Term.top == -1)
        {
            Ready.s[Ready.top].start = Ready.s[Ready.top].arr;
        }
        else
        {
            if (Ready.s[Ready.top].arr <= Term.s[Term.top].fin)
            {
                Ready.s[Ready.top].start = Term.s[Term.top].fin;
            }
            else
            {
                Ready.s[Ready.top].start = Ready.s[Ready.top].arr;
            }
        }
        Ready.s[Ready.top].fin = Ready.s[Ready.top].start + Ready.s[Ready.top].bur;
        Ready.s[Ready.top].tat = Ready.s[Ready.top].fin - Ready.s[Ready.top].arr;
        Ready.s[Ready.top].wt = Ready.s[Ready.top].tat - Ready.s[Ready.top].bur;
        Ready.s[Ready.top].rt = Ready.s[Ready.top].wt;

        // append to Term queue
        Term.top++;
        Term.s[Term.top] = Ready.s[Ready.top];

        // pop
        Ready.s[Ready.top].bur = 0;
        Ready.top--;

        // add to Ready queue the processes that arrive during the previous process running
        while (j < n && New.s[j].arr <= Term.s[Term.top].fin)
        {
            // append(Ready, New.s[j])
            Ready.top++;
            Ready.s[Ready.top] = New.s[j];
            New.top--;
            j++;
        }
    }

    // print results
    printf("\nPName \tArrtime \tBurtime Start \tTAT \tFinish\n");
    for (i = 0; i < n; i++)
    {
        printf("%d\t%6d\t\t%6d\t%6d\t%6d\t%6d\n", Term.s[i].pn, Term.s[i].arr,
               Term.s[i].bur, Term.s[i].start, Term.s[i].tat, Term.s[i].fin);
        totwt += Term.s[i].wt;
        tottat += Term.s[i].tat;
    }

    printf("Average waiting time = %.2f\n", (float)totwt / n);
    printf("Average turnaround = %.2f\n", (float)tottat / n);
}

// compare arrival time desc -> stablized
int comp_arrival(const void *p, const void *q)
{
    Process *a = (Process *)p;
    Process *b = (Process *)q;

    int arr = a->arr - b->arr;
    return arr != 0 ?  arr : b->order - a->order;
}

// compare burst time asc -> stablized 
int comp_burst(const void *p, const void *q)
{
    Process *a = (Process *)p;
    Process *b = (Process *)q;

    int bur = b->bur - a->bur;
    return bur != 0 ? bur : b->order - a->order;
}
