#include <stdio.h>
#include <stdlib.h>

#define TABLE 131071
#define MAXORD 100000

typedef struct Order
{
    int id;
    int restaurant;
    int amount;
    int promised;
    int sequence;
    int cancelled;
    int assigned;

    struct Order *next;

} Order;

typedef struct
{
    Order *bucket[TABLE];

    Order **heap;

    int hsize;

} System;

unsigned hash_id(int id)
{
    unsigned x = (unsigned)id;

    x ^= x >> 16;
    x *= 0x7feb352dU;
    x ^= x >> 15;

    return x % TABLE;
}

void hash_put(System *s, Order *o)
{
    unsigned b = hash_id(o->id);

    o->next = s->bucket[b];

    s->bucket[b] = o;
}

Order *hash_get(System *s, int id)
{
    unsigned b = hash_id(id);

    for (Order *p = s->bucket[b];
         p;
         p = p->next)
    {
        if (p->id == id)
            return p;
    }

    return NULL;
}

int higher(Order *a, Order *b)
{
    if (a->promised != b->promised)
        return a->promised < b->promised;

    return a->sequence < b->sequence;
}

void heap_swap(Order **a, Order **b)
{
    Order *t = *a;

    *a = *b;
    *b = t;
}

void heap_push(System *s, Order *o)
{
    int i = s->hsize++;

    s->heap[i] = o;

    while (i > 0)
    {
        int p = (i - 1) / 2;

        if (!higher(s->heap[i],
                    s->heap[p]))
            break;

        heap_swap(&s->heap[i],
                  &s->heap[p]);

        i = p;
    }
}

Order *heap_pop(System *s)
{
    if (s->hsize == 0)
        return NULL;

    Order *top = s->heap[0];

    s->heap[0] =
        s->heap[--s->hsize];

    int i = 0;

    while (1)
    {
        int l = 2 * i + 1;
        int r = l + 1;
        int best = i;

        if (l < s->hsize &&
            higher(s->heap[l],
                   s->heap[best]))
            best = l;

        if (r < s->hsize &&
            higher(s->heap[r],
                   s->heap[best]))
            best = r;

        if (best == i)
            break;

        heap_swap(&s->heap[i],
                  &s->heap[best]);

        i = best;
    }

    return top;
}

void place(System *s, Order *o)
{
    hash_put(s, o);

    heap_push(s, o);
}

Order *assign_next(System *s)
{
    while (s->hsize)
    {
        Order *o = heap_pop(s);

        if (!o->cancelled &&
            !o->assigned)
        {
            o->assigned = 1;

            return o;
        }
    }

    return NULL;
}

void cancel_order(System *s, int id)
{
    Order *o = hash_get(s, id);

    if (!o)
    {
        printf("Order %d not found\n", id);
        return;
    }

    o->cancelled = 1;

    printf("Order %d cancelled\n", id);
}

int main()
{
    System s = {0};

    s.heap =
        malloc(sizeof(Order *) *
               MAXORD);

    if (!s.heap)
        return 1;

    Order orders[6] =
    {
        {1001, 1, 500, 20, 1, 0, 0, NULL},
        {1002, 2, 300, 10, 2, 0, 0, NULL},
        {1003, 1, 800, 30, 3, 0, 0, NULL},
        {1004, 3, 250, 15, 4, 0, 0, NULL},
        {1005, 2, 600, 5, 5, 0, 0, NULL},
        {1006, 4, 450, 25, 6, 0, 0, NULL}
    };

    for (int i = 0; i < 6; i++)
        place(&s, &orders[i]);

    printf("Searching order 1003:\n");

    Order *o = hash_get(&s, 1003);

    if (o)
    {
        printf("Found Order ID: %d\n",
               o->id);

        printf("Amount: %d\n",
               o->amount);
    }

    cancel_order(&s, 1004);

    printf("\nAssigning orders:\n");

    for (int i = 0; i < 5; i++)
    {
        Order *next =
            assign_next(&s);

        if (next)
        {
            printf("Assigned Order: %d\n",
                   next->id);
        }
        else
        {
            printf("No active order\n");
            break;
        }
    }

    free(s.heap);

    return 0;
}