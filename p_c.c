#include <stdio.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0;    // next index to produce
int out = 0;   // next index to consume
int count = 0; // number of items in buffer

/* Producer function */
void producer(int id, int item)
{
    if(count == BUFFER_SIZE)
    {
        printf("Buffer full! Producer %d cannot produce item %d\n", id, item);
    }
    else
    {
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        count++;
        printf("Producer %d produced item %d\n", id, item);
    }
}

/* Consumer function */
void consumer(int id)
{
    if(count == 0)
    {
        printf("Buffer empty! Consumer %d cannot consume\n", id);
    }
    else
    {
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;
        printf("Consumer %d consumed item %d\n", id, item);
    }
}

int main()
{
    int choice, id, item, n;

    printf("Enter number of operations: ");
    scanf("%d", &n);

    for(int i=0; i<n; i++)
    {
        printf("\nChoose operation: 1. Producer  2. Consumer\n");
        scanf("%d", &choice);

        if(choice == 1)
        {
            printf("Enter Producer ID and item: ");
            scanf("%d %d", &id, &item);
            producer(id, item);
        }
        else if(choice == 2)
        {
            printf("Enter Consumer ID: ");
            scanf("%d", &id);
            consumer(id);
        }
        else
        {
            printf("Invalid choice! Try again.\n");
            i--; // repeat this iteration
        }
    }

    return 0;
}
