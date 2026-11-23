#include <stdio.h>

typedef struct
{
    int order_id;
    int amount;
} Order;

long comparisons = 0;

void merge(Order a[], Order temp[],
           int lo, int mid, int hi)
{
    int i = lo;
    int j = mid + 1;
    int k = lo;

    while (i <= mid && j <= hi)
    {
        comparisons++;

        if (a[i].amount <= a[j].amount)
            temp[k++] = a[i++];
        else
            temp[k++] = a[j++];
    }

    while (i <= mid)
        temp[k++] = a[i++];

    while (j <= hi)
        temp[k++] = a[j++];

    for (int p = lo; p <= hi; p++)
        a[p] = temp[p];
}

void merge_sort(Order a[], Order temp[],
                int lo, int hi)
{
    if (lo >= hi)
        return;

    int mid = lo + (hi - lo) / 2;

    merge_sort(a, temp, lo, mid);
    merge_sort(a, temp, mid + 1, hi);

    merge(a, temp, lo, mid, hi);
}

int main()s
{
    Order o[8] =
    {
        {1, 450},
        {2, 120},
        {3, 450},
        {4, 990},
        {5, 120},
        {6, 300},
        {7, 780},
        {8, 300}
    };

    Order temp[8];

    merge_sort(o, temp, 0, 7);

    for (int i = 0; i < 8; i++)
        printf("#%d:%d ",
               o[i].order_id,
               o[i].amount);

    printf("\ncomparisons = %ld\n",
           comparisons);

    return 0;
}