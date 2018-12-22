// 将数组a[0,n-1]划分为根号n个子数组，每个子数组有根号n个元素。
// 然后递归地对分割后的子数组进行排序，最后将所得到的根号n个排好序的子数组合并排序。

#include <assert.h> // assert
#include <math.h>   // pow
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> // malloc, free
#include <string.h> // memcpy

static void MergeSortInternal(int array[], const int from, const int to)
{
    const int length = to - from;
    if (from >= to)
        assert(false);
    else if (length == 1)
        return;

    const int step = (int)pow(length, 0.5); // 每个子问题的长度为根号n
    int from2 = from;                       // 子问题开头
    int to2 = from + step;                  // 子问题结尾
    while (to2 < to)
    {
        MergeSortInternal(array, from2, to2);
        from2 = to2; // 下一个子问题
        to2 += step;
    }
    MergeSortInternal(array, from2, to); // 最后一个子问题长度可能不足根号n

    int *newArray = malloc((length) * sizeof(int));
    int *p = newArray;
    // char flags[length];
    bool *flags = calloc(length, sizeof(bool)); // 标识是否已经取出

    while (p < newArray + length) // 合并
    {
        int mini = -1; // 最小项的下标

        from2 = from;
        to2 = from + step;
        while (to2 < to)
        {
            for (int i = from2; i < to2; i++)                                       // 遍历子问题
                if (flags[i - from] == 0 && (mini == -1 || array[i] < array[mini])) // 未被取出时，最小项不存在或小于最小项
                    mini = i;
            from2 = to2;
            to2 += step;
        }
        for (int i = from2; i < to; i++)
            if (flags[i - from] == 0 && (mini == -1 || array[i] < array[mini]))
                mini = i;

        flags[mini - from] = 1; // 注意偏移
        *p++ = array[mini];
    }

    memcpy(array + from, newArray, length * sizeof(int)); // 注意偏移
    free(newArray);
    free(flags);
}

void MergeSort(int length, int array[length])
{
    return MergeSortInternal(array, 0, length);
}

static int arr[] = {85, 75, 75, 59, 7, 30, 36, 16, 27, 80, 53, 78, 69, 85, 75};
int main(void)
{
    int length = sizeof(arr) / sizeof(*arr);

    for (int i = 0; i < length; i++)
        printf("%2d ", arr[i]);
    putchar('\n');

    MergeSort(length, arr);
    for (int i = 0; i < length; i++)
        printf("%2d ", arr[i]);
    putchar('\n');

    return 0;
}
