// 对于长度相同的2个字符串A和B，其距离定义为相应位置字符距离之和。2个非空格字符的距离是它们的ASCII码之差的绝对值。空格与空格的距离为0，空格与其他字符的距离为一定值k。
// 在一般情况下，字符串A和B的长度不一定相同。字符串A的扩展是在A中插入若干空格字符所产生的字符串。在字符串A和B的所有长度相同的扩展中，有一对距离最小的扩展，该距离称为字符串A和B的扩展距离。

#include <limits.h> // INT_MAX, CHAR_MAX
#include <stdio.h>
#include <stdlib.h> // abs, malloc
#include <string.h> // memset

inline static int min3(int a, int b, int c)
{
    return a > b ? (b > c ? c : b) : (a > c ? c : a);
}
inline static int dist(char a, char b)
{
    return abs(a - b);
}

static char A[BUFSIZ], B[BUFSIZ];
static int I, J;
static int *T; // 动态分配一维数组模拟二维数组

inline static int getTe(int i, int j) // 方便取出一维数组模拟的二维数组
{
    return (i == -1 || j == -1) ? -1 : T[i * J + j]; // 越界返回主要是为了show，因为val已经手动处理了越界
}
static int k;
int val(int i, int j)
{
    if (i == -1 && j == -1) // 两个都取到最前了，不能再递归
        return 0;
    else if (i == -1 || j == -1)
        return INT_MAX - k - CHAR_MAX; // 越界时返回尽量大的值，使最短路径为非越界的那一个
    else if (getTe(i, j) != -1)        // 非越界返回-1，而是备忘区无该值值时为-1
        return getTe(i, j);
    else
        return T[i * J + j] = min3(val(i - 1, j) + k, val(i, j - 1) + k, val(i - 1, j - 1) + dist(A[i], B[j]));
}

void show(int i, int j) // 将会竖着从头显示
{
    int value = getTe(i, j);
    if (value == -1) // 越界
        return;
    if (value == getTe(i - 1, j) + k) // A输出A[i]，B输出空格
    {
        show(i - 1, j);
        printf("%c %c\n", A[i], '_');
    }
    else if (value == getTe(i, j - 1) + k) // A数组输出空格，B输出B[j]
    {
        show(i, j - 1);
        printf("%c %c\n", '_', B[j]);
    }
    else if (value == getTe(i - 1, j - 1) + dist(A[i], B[j])) // A输出A[i]，B输出B[j]
    {
        show(i - 1, j - 1);
        printf("%c %c\n", A[i], B[j]);
    }
}

int main(void)
{
    scanf("%s %s", A + 1, B + 1); // A[0]和B[0]放空格
    A[0] = B[0] = ' ';
    scanf("%d", &k); // 空格和其他字符的距离

    I = strlen(A), J = strlen(B);       // A的长度
    T = calloc(I * J, sizeof(int));     // B的长度
    memset(T, -1, I * J * sizeof(int)); // 备忘区无记录时设为-1

    putchar('\n');
    printf("%d\n", val(I - 1, J - 1));
    show(I - 1, J - 1);

    return 0;
}
