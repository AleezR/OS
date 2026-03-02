#include <stdio.h>
#include <limits.h>

struct process {
    int pid;
    int at;   // Arrival Time
    int bt;   // Burst Time
    int rt;   // Remaining Time (for RR)
    int pr;   // Priority (for Priority Scheduling)
    int ct;   // Completion Time
    int wt;   // Waiting Time
    int tat;  // Turnaround Time
};

// FCFS Scheduling
void fcfs(int n, struct process p[]) {
    int current_time = 0;
    float total_wt = 0.0;

    // Sort by arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].at > p[j].at) {
                struct process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        if (current_time < p[i].at)
            current_time = p[i].at;
        p[i].ct = current_time + p[i].bt;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        current_time = p[i].ct;
        total_wt += p[i].wt;
    }

    printf("\nFCFS Scheduling:\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }
    printf("Average Waiting Time = %.2f\n", total_wt / n);
}

// Non-Preemptive SJF Scheduling
void sjf(int n, struct process p[]) {
    int current_time = 0, completed = 0, selected;
    float total_wt = 0.0;
    int visited[10] = {0};

    while (completed < n) {
        int min_bt = INT_MAX;
        selected = -1;

        // Find process with minimum burst time among arrived processes
        for (int i = 0; i < n; i++) {
            if (!visited[i] && p[i].at <= current_time && p[i].bt < min_bt) {
                min_bt = p[i].bt;
                selected = i;
            }
        }

        if (selected == -1) {
            current_time++;  // If no process has arrived yet
            continue;
        }

        p[selected].ct = current_time + p[selected].bt;
        p[selected].tat = p[selected].ct - p[selected].at;
        p[selected].wt = p[selected].tat - p[selected].bt;

        total_wt += p[selected].wt;
        current_time = p[selected].ct;
        visited[selected] = 1;
        completed++;
    }

    printf("\nSJF Scheduling (Non-Preemptive):\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");

    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt,
               p[i].ct, p[i].tat, p[i].wt);
    }

    printf("Average Waiting Time = %.2f\n", total_wt / n);
}

// Non-Preemptive Priority Scheduling
void priority_non_preemptive(int n, struct process p[]) {
    int current_time = 0, completed = 0, selected;
    float total_wt = 0.0;
    int visited[10] = {0};

    while (completed < n) {
        int highest_pr = INT_MAX;
        selected = -1;

        for (int i = 0; i < n; i++) {
            if (!visited[i] && p[i].at <= current_time) {
                if (p[i].pr < highest_pr) {
                    highest_pr = p[i].pr;
                    selected = i;
                }
                else if (p[i].pr == highest_pr) {
                    if (p[i].at < p[selected].at)
                        selected = i;
                }
            }
        }

        if (selected == -1) {
            current_time++;
            continue;
        }

        p[selected].ct = current_time + p[selected].bt;
        p[selected].tat = p[selected].ct - p[selected].at;
        p[selected].wt = p[selected].tat - p[selected].bt;

        total_wt += p[selected].wt;
        current_time = p[selected].ct;
        visited[selected] = 1;
        completed++;
    }

    printf("\nNon-Preemptive Priority Scheduling (Lower number = Higher priority):\n");
    printf("PID\tAT\tBT\tPR\tCT\tTAT\tWT\n");

    for (int i = 0; i < n; i++)
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].pr,
               p[i].ct, p[i].tat, p[i].wt);

    printf("Average Waiting Time = %.2f\n", total_wt / n);
}

// Round Robin Scheduling
void round_robin(int n, struct process p[], int quantum) {
    int t = 0, completed = 0;
    float total_wt = 0.0;

    for (int i = 0; i < n; i++)
        p[i].rt = p[i].bt;

    while (completed < n) {
        int idle = 1;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= t && p[i].rt > 0) {
                idle = 0;
                if (p[i].rt > quantum) {
                    p[i].rt -= quantum;
                    t += quantum;
                } else {
                    t += p[i].rt;
                    p[i].rt = 0;
                    p[i].ct = t;
                    p[i].tat = p[i].ct - p[i].at;
                    p[i].wt = p[i].tat - p[i].bt;
                    total_wt += p[i].wt;
                    completed++;
                }
            }
        }
        if (idle) t++;
    }

    printf("\nRound Robin Scheduling (Quantum = %d):\nPID\tAT\tBT\tCT\tTAT\tWT\n", quantum);
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }
    printf("Average Waiting Time = %.2f\n", total_wt / n);
}

// Main function
int main() {
    int n, choice, quantum = 3;
    struct process p[10], p_copy[10];

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter arrival time of P%d: ", p[i].pid);
        scanf("%d", &p[i].at);
        printf("Enter burst time of P%d: ", p[i].pid);
        scanf("%d", &p[i].bt);
        printf("Enter priority of P%d: ", p[i].pid);
        scanf("%d", &p[i].pr);
    }

    // Copy original array for each algorithm
    for (int i = 0; i < n; i++) p_copy[i] = p[i];

    while (1) {
        printf("\n--- Scheduling Menu ---\n");
        printf("1. FCFS\n2. SJF\n3. Non-Preemptive Priority\n4. Round Robin\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        for (int i = 0; i < n; i++) p_copy[i] = p[i]; // reset data before each run

        switch(choice) {
            case 1: fcfs(n, p_copy); break;
            case 2: sjf(n, p_copy); break;
            case 3: priority_non_preemptive(n, p_copy); break;
            case 4: round_robin(n, p_copy, quantum); break;
            case 5: return 0;
            default: printf("Invalid choice!\n");
        }
    }
}
