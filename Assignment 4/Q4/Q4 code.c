#include <stdio.h>

int partition(int a[], int low, int high, int *swaps)
{
    int pivot = a[high];

    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (a[j] <= pivot)
        {
            i++;

            int temp = a[i];
            a[i] = a[j];
            a[j] = temp;

            (*swaps)++;
        }
    }

    int temp = a[i + 1];
    a[i + 1] = a[high];
    a[high] = temp;

    (*swaps)++;

    return i + 1;
}

int main()
{
    int a[] = {7, 2, 9, 4, 3, 8, 5};

    int n = 7;
    int swaps = 0;

    int pivot = partition(a, 0, n - 1, &swaps);

    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);

    printf("\n");

    printf("Pivot position = %d\n", pivot);
    printf("Swaps = %d\n", swaps);

    return 0;
}