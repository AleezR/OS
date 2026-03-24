#include <stdio.h>

#define MAX_PAGES 50
#define MAX_FRAMES 10

// Print frames and page hit/miss status
void printTable(int ref[], int table[MAX_FRAMES][MAX_PAGES], int status[], int n, int frames) {
    printf("\nReference String: ");
    for (int i = 0; i < n; i++) printf("%d  ", ref[i]);
    printf("\n\n");

    for (int i = 0; i < frames; i++) {
        printf("Frame %d: ", i);
        for (int j = 0; j < n; j++) {
            if (table[i][j] == -1) printf("-  ");
            else printf("%d  ", table[i][j]);
        }
        printf("\n");
    }

    printf("\nStatus:   ");
    int faults = 0, hit = 0;
    for (int i = 0; i < n; i++) {
        if (status[i] == 0) {
            printf("M  ");
            faults++;
        } else {
            printf("H  ");
            hit++;
        }
    }
    printf("\n\nTotal Page Faults = %d\n", faults);
    printf("Total Page Hits = %d\n", hit);
}

// Corrected FIFO
void fifo(int ref[], int n, int frames) {
    int frame[MAX_FRAMES], table[MAX_FRAMES][MAX_PAGES], status[MAX_PAGES];
    int index = 0;

    // Initialize frames to -1
    for (int i = 0; i < frames; i++) frame[i] = -1;

    for (int i = 0; i < n; i++) {
        int hit = 0;
        for (int j = 0; j < frames; j++)
            if (frame[j] == ref[i]) hit = 1;

        if (!hit) {
            frame[index] = ref[i];      // Replace the oldest page
            index = (index + 1) % frames; // Move FIFO pointer correctly
            status[i] = 0;              // Miss
        } else {
            status[i] = 1;              // Hit
        }

        for (int j = 0; j < frames; j++) table[j][i] = frame[j];
    }

    printf("\nFIFO Algorithm");
    printTable(ref, table, status, n, frames);
}

// Corrected LRU
void lru(int ref[], int n, int frames) {
    int frame[MAX_FRAMES], table[MAX_FRAMES][MAX_PAGES], status[MAX_PAGES], time[MAX_FRAMES];
    int counter = 0;

    for (int i = 0; i < frames; i++) {
        frame[i] = -1;
        time[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        counter++;
        int hit = -1;
        for (int j = 0; j < frames; j++) {
            if (frame[j] == ref[i]) {
                hit = j;               // store index of hit
                break;
            }
        }

        if (hit != -1) {
            status[i] = 1;             // Hit
            time[hit] = counter;       // Update last used time
        } else {
            // Find least recently used
            int lru_index = 0;
            for (int j = 1; j < frames; j++) {
                if (time[j] < time[lru_index]) lru_index = j;
            }
            frame[lru_index] = ref[i];
            time[lru_index] = counter;
            status[i] = 0;             // Miss
        }

        for (int j = 0; j < frames; j++) table[j][i] = frame[j];
    }

    printf("\nLRU Algorithm");
    printTable(ref, table, status, n, frames);
}

// Corrected Optimal
void optimal(int ref[], int n, int frames) {
    int frame[MAX_FRAMES], table[MAX_FRAMES][MAX_PAGES], status[MAX_PAGES];

    for (int i = 0; i < frames; i++) frame[i] = -1;

    for (int i = 0; i < n; i++) {
        int hit = 0;
        for (int j = 0; j < frames; j++)
            if (frame[j] == ref[i]) hit = 1;

        if (!hit) {
            int pos = -1, farthest = -1;

            // Check for empty frame
            for (int j = 0; j < frames; j++) {
                if (frame[j] == -1) {
                    pos = j;
                    break;
                }
            }

            // If no empty, find the page used farthest in future
            if (pos == -1) {
                for (int j = 0; j < frames; j++) {
                    int k;
                    for (k = i + 1; k < n; k++)
                        if (frame[j] == ref[k]) break;
                    if (k > farthest) {
                        farthest = k;
                        pos = j;
                    }
                }
            }

            frame[pos] = ref[i];
            status[i] = 0;  // Miss
        } else {
            status[i] = 1;  // Hit
        }

        for (int j = 0; j < frames; j++) table[j][i] = frame[j];
    }

    printf("\nOptimal Algorithm");
    printTable(ref, table, status, n, frames);
}

int main() {
    int ref[MAX_PAGES], n, frames, choice;

    printf("Enter number of pages: ");
    scanf("%d", &n);
    if (n > MAX_PAGES) { printf("Max pages = %d\n", MAX_PAGES); return 1; }

    printf("Enter reference string: ");
    for (int i = 0; i < n; i++) scanf("%d", &ref[i]);

    printf("Enter number of frames: ");
    scanf("%d", &frames);
    if (frames > MAX_FRAMES) { printf("Max frames = %d\n", MAX_FRAMES); return 1; }

    while (1) {
        printf("\n1. FIFO\n2. LRU\n3. Optimal\n4. Exit\nChoice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: fifo(ref, n, frames); break;
            case 2: lru(ref, n, frames); break;
            case 3: optimal(ref, n, frames); break;
            case 4: printf("Exiting\n"); return 0;
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}
