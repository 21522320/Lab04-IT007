/* *
 * UIT
 * Team: WINter
 * Course ID: IT007.N11.2
 * File: SRT.cpp
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
	float updated_sta;
	float updated_fin;
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
		cout << name << '\t' << response << "\t  " << waiting << "\t  " << turnaround << '\t' << finish << endl;
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
	cout << "Enter the number of processes: ";
	int n;
	cin >> n;

	pro *process = new pro[n];
	int i;
	for (i = 0; i < n; i++)
	{
		process[i].input();
		process[i].remaining = process[i].burst;
	}

	// Initiate values
	int n_neww = n;
	pro *neww = process;
	sort(neww, neww + n_neww, CompareRemaining);
	sort(neww, neww + n_neww, CompareArrival);

	int n_ready = 0;
	pro *ready = new pro[n];

	pro running;

	int n_terminated = 0;
	pro *terminated = new pro[n];

	pro proc;
	
	// Run processes
	add_tail(ready, neww[0], n_ready);
	delete_head(neww, n_neww);

	running = ready[0];
	delete_head(ready, n_ready);
	
	float current_time = running.arrival, estimated_finish;
	int flag_swap;

	while (n_terminated != n)
	{

		// update start when this is the first time the current process running
		if (running.remaining == running.burst)
			running.start = current_time;

		estimated_finish = current_time + running.remaining;
		flag_swap = 0;

		// add to ready queue processes arriving during the current process running
		while (n_neww != 0 && neww[0].arrival < estimated_finish)
		{
			add_tail(ready, neww[0], n_ready);
			delete_head(neww, n_neww);

			// sort Ready queue by remaining time
			sort(ready, ready + n_ready, CompareRemaining);

			if (ready[0].arrival > current_time) {
				running.remaining = running.remaining - (ready[0].arrival - current_time);
				current_time = ready[0].arrival;
			}

			if (ready[0].remaining < running.remaining)
			{
				flag_swap = 1;
				proc = running;
				running = ready[0];
				delete_head(ready, n_ready);
				add_tail(ready, proc, n_ready);
				sort(ready, ready + n_ready, CompareRemaining);
				break;
			}
		}
		if (flag_swap == 0)
		{
			running.finish = current_time + running.remaining;
			running.remaining = 0;
			running.response = running.start - running.arrival;
			running.turnaround = running.finish - running.arrival;
			running.waiting = running.turnaround - running.burst;
			current_time = running.finish;

			add_tail(terminated, running, n_terminated);

			if (n_ready == 0)
			{
				add_tail(ready, neww[0], n_ready);
				delete_head(neww, n_neww);
				current_time = ready[0].arrival;
			}

			running = ready[0];
			delete_head(ready, n_ready);
		}
	}
	
	// Calculate average values
	float average_response = 0;
	for (i = 0; i < n_terminated; i++)
		average_response += terminated[i].response;
	average_response /= n_terminated;

	float average_waiting = 0;
	for (i = 0; i < n_terminated; i++)
		average_waiting += terminated[i].waiting;
	average_waiting /= n_terminated;

	float average_turnaround = 0;
	for (i = 0; i < n_terminated; i++)
		average_turnaround += terminated[i].turnaround;
	average_turnaround /= n_terminated;
	
	// Output
	cout << "Name\tResponse Waiting TAT Finish\n";
	for (i = 0; i < n; i++)
	{
		terminated[i].output();
	}
	
	cout << "\nAverage response time: " << average_response << endl;
	cout << "Average waiting time: " << average_waiting << endl;
	cout << "Average turnaround time: " << average_turnaround << endl;
	
	return 0;
}
