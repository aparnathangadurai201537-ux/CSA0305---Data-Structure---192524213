#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void merge(int a[], int temp[],
           int lo, int mid, int hi)
{
    int i = lo;
    int j = mid + 1;
    int k = lo;

    while (i <= mid && j <= hi)
    {
        if (a[i] <= a[j])
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

void merge_sort(int a[], int temp[],
                int lo, int hi)
{
    if (lo >= hi)
        return;

    int mid = lo + (hi - lo) / 2;

    merge_sort(a, temp, lo, mid);
    merge_sort(a, temp, mid + 1, hi);

    merge(a, temp, lo, mid, hi);
}

void parallel_merge_sort(int a[], int temp[],
                         int lo, int hi, int depth)
{
    if (lo >= hi)
        return;

    int mid = lo + (hi - lo) / 2;

    if (depth > 0)
    {
        #pragma omp parallel sections
        {
            #pragma omp section
            parallel_merge_sort(a, temp,
                                lo, mid,
                                depth - 1);

            #pragma omp section
            parallel_merge_sort(a, temp,
                                mid + 1, hi,
                                depth - 1);
        }
    }
    else
    {
        merge_sort(a, temp, lo, mid);
        merge_sort(a, temp, mid + 1, hi);
    }

    merge(a, temp, lo, mid, hi);
}

int main()
{
    int n = 20;

    int *a = malloc(n * sizeof(int));
    int *temp = malloc(n * sizeof(int));

    if (!a || !temp)
        return 1;

    for (int i = 0; i < n; i++)
        a[i] = rand() % 100;

    printf("Before sorting:\n");

    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);

    printf("\n");

    parallel_merge_sort(a, temp,
                        0, n - 1, 3);

    printf("\nAfter sorting:\n");

    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);

    printf("\n");

    free(a);
    free(temp);

    return 0;
}