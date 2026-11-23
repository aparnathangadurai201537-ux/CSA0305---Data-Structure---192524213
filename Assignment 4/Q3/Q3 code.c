#include <stdio.h>

#define MAX 32

typedef struct
{
    int severity;
    int patient;
} Case;

Case heap[MAX];
int size = 0;

void swap(Case *a, Case *b)
{
    Case t = *a;
    *a = *b;
    *b = t;
}

void push(int severity, int patient)
{
    if (size == MAX)
        return;

    heap[size] = (Case){severity, patient};

    int i = size++;

    while (i > 0)
    {
        int p = (i - 1) / 2;

        if (heap[p].severity >= heap[i].severity)
            break;

        swap(&heap[p], &heap[i]);

        i = p;
    }
}

Case pop()
{
    Case top = heap[0];

    heap[0] = heap[--size];

    int i = 0;

    while (1)
    {
        int l = 2 * i + 1;
        int r = l + 1;
        int best = i;

        if (l < size &&
            heap[l].severity > heap[best].severity)
            best = l;

        if (r < size &&
            heap[r].severity > heap[best].severity)
            best = r;

        if (best == i)
            break;

        swap(&heap[i], &heap[best]);

        i = best;
    }

    return top;
}

int main()
{
    int sev[] = {3, 9, 5, 7, 1, 8};

    for (int p = 0; p < 6; p++)
        push(sev[p], 101 + p);

    while (size)
    {
        Case c = pop();

        printf("P%d(%d) ",
               c.patient,
               c.severity);
    }

    printf("\n");

    return 0;
}