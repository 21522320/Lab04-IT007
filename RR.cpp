/* *
 * UIT
 * Team: WINter
 * Course ID: IT007.N11.2
 * File: RR.cpp
 * */

#include <iostream>
#include <algorithm>
using namespace std;

typedef struct Process
{
    string name;
    float arrival;
    float burst;
    float remaining;
    float start;
    float finish;
    float response;
    float waiting;
    float turnaround;

    void input()
    {
        cout << "Enter the Process Name, Arrival Time& Burst Time :\n";
        cin >> name >> arrival >> burst;
    }
    void output()
    {
        cout << name << ' ' << response << ' ' << waiting << ' ' << turnaround << endl;
    }
} pro;

bool CompareArrival(const pro &x, const pro &y)
{
    return x.arrival < y.arrival;
}

bool CompareRemaining(const pro &x, const pro &y)
{
    return x.remaining < y.remaining;
}

void delete_head(pro *&processes, int &n)
{
    if (n > 0)
    {
        for (int i = 0; i < n - 1; i++)
            processes[i] = processes[i + 1];
        n--;
    }
    return;
}

void add_tail(pro *&processes, pro process, int &n)
{
    processes[n] = process;
    n++;
    return;
}

int main()
{
    // Enter processes
    float quantum_time;
    cout << "Enter quantum time: ";
    cin >> quantum_time;

    cout << "Enter the number of processes: ";
    int n;
    cin >> n;

    pro *process = new pro[n];
    int i, j;
    for (i = 0; i < n; i++)
    {
        process[i].input();
        process[i].remaining = process[i].burst;
    }

    // Initiate values
    int n_neww = n;
    pro *neww = process;
    sort(neww, neww + n_neww, CompareArrival);

    int n_ready = 0;
    pro *ready = new pro[n];

    pro running;

    int n_terminated = 0;
    pro *terminated = new pro[n];

    i = 0;
    j = 0;
    pro proc;

    add_tail(ready, neww[0], n_ready);
    delete_head(neww, n_neww);

    running = ready[0];
    delete_head(ready, n_ready);

    float current_time = running.arrival;

    while (n_terminated != n)
    {

        if (running.remaining == running.burst)
            running.start = current_time;

        if (running.remaining <= quantum_time)
        {
            running.finish = current_time + running.remaining;
            running.remaining = 0;
            running.response = running.start - running.arrival;
            running.turnaround = running.finish - running.arrival;
            running.waiting = running.turnaround - running.burst;
            current_time = running.finish;

            add_tail(terminated, running, n_terminated);
        }

        else
        {
            running.remaining -= quantum_time;
            current_time += quantum_time;
        }

        while (n_neww != 0 && neww[0].arrival <= current_time)
        {
            add_tail(ready, neww[0], n_ready);
            delete_head(neww, n_neww);
        }

        if (running.remaining != 0)
            add_tail(ready, running, n_ready);

        if (n_ready == 0)
        {
            add_tail(ready, neww[0], n_ready);
            delete_head(neww, n_neww);
            current_time = ready[0].arrival;
        }

        running = ready[0];
        delete_head(ready, n_ready);
    }

    for (i = 0; i < n; i++)
    {
        terminated[i].output();
    }
    return 0;
}
