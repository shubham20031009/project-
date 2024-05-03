#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int remaining_time;
};
void sjf_preemptive(struct Process processes[], int n) {
    int time = 0;
    int completed = 0;
    int *waiting_time = (int *)malloc(n * sizeof(int));
    int *turnaround_time = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        waiting_time[i] = 0;
        turnaround_time[i] = 0;
    }

    while (completed < n) {
        int shortest = -1;
        int min_burst = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= time && processes[i].remaining_time < min_burst && processes[i].remaining_time > 0) {
                shortest = i;
                min_burst = processes[i].remaining_time;
            }
        }

        if (shortest == -1) {
            time++;
            continue;
        }

        processes[shortest].remaining_time--;
        time++;

        if (processes[shortest].remaining_time == 0) {
            completed++;
            waiting_time[shortest] = time - processes[shortest].arrival_time - processes[shortest].burst_time;
            turnaround_time[shortest] = time - processes[shortest].arrival_time;
        }
    }

    printf("\nShortest Job First (Preemptive) Scheduling:\n");
    printf("Process\tWaiting Time\tTurnaround Time\n");

    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\n", i + 1, waiting_time[i], turnaround_time[i]);
    }

    free(waiting_time);
    free(turnaround_time);
}
void round_robin(struct Process processes[], int n, int time_quantum) {
    int *waiting_time = (int *)malloc(n * sizeof(int));
    int *turnaround_time = (int *)malloc(n * sizeof(int));
    int *remaining_time = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        remaining_time[i] = processes[i].burst_time;
        waiting_time[i] = 0;
        turnaround_time[i] = 0;
    }
    int time = 0;
    int completed = 0;

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (remaining_time[i] > 0) {
                if (remaining_time[i] <= time_quantum) {
                    time += remaining_time[i];
                    turnaround_time[i] = time - processes[i].arrival_time; 
                    waiting_time[i] = turnaround_time[i] - processes[i].burst_time; 
                    remaining_time[i] = 0;
                    completed++;
                } else {
                    time += time_quantum;
                    remaining_time[i] -= time_quantum;

                    if (waiting_time[i] == 0) {
                        waiting_time[i] = time - processes[i].arrival_time - processes[i].burst_time;
                    }
                }
            }
        }
    }

    printf("\nRound Robin Scheduling:\n");
    printf("Process\tWaiting Time\tTurnaround Time\n");

    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\n", i + 1, waiting_time[i], turnaround_time[i]);
    }

    free(waiting_time);
    free(turnaround_time);
    free(remaining_time);
}




int main() {
    int n, time_quantum;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process *processes = (struct Process *)malloc(n * sizeof(struct Process));

    printf("Enter the arrival time and burst time for each process:\n");
    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        printf("Process %d:\n", i + 1);
        printf("Arrival Time: ");
        scanf("%d", &processes[i].arrival_time);
        printf("Burst Time: ");
        scanf("%d", &processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time;
    }

    printf("Enter the time quantum for Round Robin: ");
    scanf("%d", &time_quantum);

    sjf_preemptive(processes, n);
    round_robin(processes, n, time_quantum);

    free(processes);

    return 0;
}
