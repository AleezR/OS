#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX 5000
#define N 10

// generate random requests
void gen(int r[]) {
    for (int i = 0; i < N; i++) {
        r[i] = rand() % MAX;
    }
}

// sort array
void sort(int r[]) {
    for (int i = 0; i < N - 1; i++) {
        for (int j = 0; j < N - i - 1; j++) {
            if (r[j] > r[j + 1]) {
                int t = r[j];
                r[j] = r[j + 1];
                r[j + 1] = t;
            }
        }
    }
}

// SSTF
int sstf(int r[], int h) {
    int used[N] = {0};
    int total = 0;

    for (int i = 0; i < N; i++) {
        int min = 100000, pos = -1;

        for (int j = 0; j < N; j++) {
            if (!used[j]) {
                int d = abs(h - r[j]);
                if (d < min) {
                    min = d;
                    pos = j;
                }
            }
        }

        used[pos] = 1;
        total += abs(h - r[pos]);
        h = r[pos];
    }

    return total;
}

// LOOK
int look(int r[], int h) {
    int a[N], total = 0;

    for (int i = 0; i < N; i++) a[i] = r[i];
    sort(a);

    int i;
    for (i = 0; i < N; i++) {
        if (a[i] >= h) break;
    }

    for (int j = i; j < N; j++) {
        total += abs(h - a[j]);
        h = a[j];
    }

    for (int j = i - 1; j >= 0; j--) {
        total += abs(h - a[j]);
        h = a[j];
    }

    return total;
}

// C-SCAN
int cscan(int r[], int h) {
    int a[N], total = 0;

    for (int i = 0; i < N; i++) a[i] = r[i];
    sort(a);

    int i;
    for (i = 0; i < N; i++) {
        if (a[i] >= h) break;
    }

    for (int j = i; j < N; j++) {
        total += abs(h - a[j]);
        h = a[j];
    }

    total += abs(h - (MAX - 1));   // go to end
    total += (MAX - 1);            // jump to 0
    h = 0;

    for (int j = 0; j < i; j++) {
        total += abs(h - a[j]);
        h = a[j];
    }

    return total;
}

int main() {
    int h;
    int r[N];

    printf("Enter initial head position (0-4999): ");
    scanf("%d", &h);

    if (h < 0 || h >= MAX) {
        printf("Invalid head position!\n");
        return 0;
    }

    srand(time(0));
    gen(r);

    printf("\nRequests: ");
    for (int i = 0; i < N; i++) {
        printf("%d ", r[i]);
    }

    printf("\nInitial Head: %d\n", h);

    printf("\nTotal Head Movement:\n");
    printf("SSTF  = %d\n", sstf(r, h));
    printf("LOOK  = %d\n", look(r, h));
    printf("CSCAN = %d\n", cscan(r, h));

    return 0;
}
