// 试写出一算法，根据依次输入的顶点数据，边的数目，各顶点的信息和各条边的信息，建立一个无向图的邻接表。打印输出该邻接表。
#include <locale.h> // utf-8编码输出不乱码
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char VerTexType; // 顶点的数据类型
typedef int OtherInfo;   // 边的数据类型

typedef struct ArcNode // 边节点
{
    OtherInfo info;          //顶点信息
    struct VNode *adjvex;    // 该边指向的顶点
    struct ArcNode *nextarc; // 下一条边
} * ArcNode;

typedef struct VNode // 顶点
{
    VerTexType data;          // 顶点储存的数据
    struct ArcNode *firstarc; // 指向第一条依附该顶点的边的指针
} * VNode;                    // 邻接表

typedef struct ALGraph
{
    VNode *vertices;    //顶点数组
    int vexnum, arcnum; // 顶点数和边数
} * ALGraph;

ALGraph CreateALGraph(void);                            // 创建无向图
VNode *CreateAdjList(int vexnum);                       // 创建顶点数组
VNode CreateVNode(VerTexType data);                     // 创建顶点
ArcNode CreateArcNode(VNode node, OtherInfo info);      // 创建边
void CreateArcs(ALGraph graph);                         // 交互式获取边的信息并完成边的创建和链接
bool ArcExist(const ALGraph graph, int head, int rear); // 判断边是否存在
int LocaleVex(ALGraph graph, VerTexType value);         // 根据值获取顶点索引
VNode GetVNode(ALGraph graph, int index);               // 根据索引获取顶点
void LinkArc(VNode vn, ArcNode an);                     // 把边插入图中
void DFS(ALGraph graph, int indexN);                    // 深度优先遍历
void PrintALGraph(ALGraph graph);                       // 普通遍历，非深度优先也非广度优先遍历
void ClearInput(void);                                  // 清除多余的输入

// 接口函数
VerTexType GetVerticeData(void);      // 获取顶点名称
OtherInfo GetArcInfo(void);           // 获取边的长度
wchar_t *VerTexToString(VNode vn);    // 把顶点名称转换为宽字符串
wchar_t *ArcInfoToString(ArcNode an); // 把边的长度转换为宽字符串

ALGraph CreateALGraph(void)
{
    ALGraph graph = calloc(1, sizeof(struct ALGraph));

    wprintf(L"请输入顶点数：");
    scanf("%d", &graph->vexnum);
    wprintf(L"请输入边数：");
    scanf("%d", &graph->arcnum);

    printf("\n");
    wprintf(L"输入顶点储存的数据\n");
    graph->vertices = CreateAdjList(graph->vexnum);

    printf("\n");
    wprintf(L"输入无向边的邻接点\n");
    CreateArcs(graph);

    return graph;
}

VNode *CreateAdjList(int vexnum)
{
    VNode *al = calloc(vexnum, sizeof(VNode));

    for (int i = 0; i < vexnum; i++)
    {
        wprintf(L"第%d个顶点：", i + 1);
        al[i] = CreateVNode(GetVerticeData());
    }

    return al;
}

void CreateArcs(ALGraph graph)
{
    for (int i = 0; i < graph->arcnum; i++)
    {
        int head, rear;
        OtherInfo info;

        wprintf(L"第%d条边：\n", i + 1);
        wprintf(L"第一个邻接点：");
        head = LocaleVex(graph, GetVerticeData());
        wprintf(L"第二个邻接点：");
        rear = LocaleVex(graph, GetVerticeData());

        if (ArcExist(graph, head, rear))
        {
            wprintf(L"这条边已经存在！\n：");
            i--;
            continue;
        }

        wprintf(L"边的长度：");
        info = GetArcInfo();

        LinkArc(GetVNode(graph, head), CreateArcNode(GetVNode(graph, rear), info));
        LinkArc(GetVNode(graph, rear), CreateArcNode(GetVNode(graph, head), info));
        printf("\n");
    }
}

void LinkArc(VNode vn, ArcNode an)
{
    ArcNode tempan;

    if (vn->firstarc == NULL)
    {
        vn->firstarc = an;
        return;
    }
    else
        tempan = vn->firstarc;

    while (tempan->nextarc != NULL)
        tempan = tempan->nextarc;

    tempan->nextarc = an;
}

VNode CreateVNode(VerTexType data)
{
    VNode vn = calloc(1, sizeof(struct VNode));
    vn->data = data;
    return vn;
}

ArcNode CreateArcNode(VNode adjvex, OtherInfo info)
{
    ArcNode an = calloc(1, sizeof(struct ArcNode));
    an->adjvex = adjvex;
    an->info = info;
    return an;
}

bool ArcExist(const ALGraph graph, int head, int rear)
{
    VNode vn = GetVNode(graph, head);

    for (ArcNode an = vn->firstarc; an != NULL; an = an->nextarc)
        if (LocaleVex(graph, an->adjvex->data) == rear)
            return true;

    return false;
}

int LocaleVex(ALGraph graph, VerTexType value)
{
    for (int i = 0; i < graph->vexnum; i++)
        if (graph->vertices[i]->data == value)
            return i;

    return -1;
}

VNode GetVNode(ALGraph graph, int index)
{
    if (graph == NULL)
        return NULL;
    else
        return graph->vertices[index];
}

void DFS(ALGraph graph, int index) // 从index的节点开始遍历
{
    // static bool visited[graph->vexnum];

    int needNewLine;             // 单个无边顶点或有边且遍历完后需要换行，已完全遍历过的无需换行
    static bool firstarc = true; // 指示是否需要空格
    static bool firstrun = true; // 初始化
    static int level = 0;        // 指示是否是最外层循环

    static bool *visited; // 指示是否遍历过

    if (firstrun)
    {
        wprintf(L"深度优先搜索遍历图：\n");
        visited = calloc(graph->vexnum, sizeof(bool));
        firstrun = false;
    }

    while (index < graph->vexnum)
    {
        if (visited[index] == false)
        {
            wprintf(L"%s", VerTexToString(GetVNode(graph, index))); // 输出单个无边顶点或第二个邻接点
            needNewLine = true;
        }
        visited[index] = true;

        for (ArcNode an = GetVNode(graph, index)->firstarc; an != NULL; an = an->nextarc) // 如果是单个无边顶点，不会进下面的此循环
        {
            needNewLine = false;
            int location = LocaleVex(graph, an->adjvex->data);

            if (visited[location] == false)
            {
                if (!firstarc) //非每行第一个输出则需要输出自己（第一个邻接点）并空格
                    wprintf(L"  %s", VerTexToString(GetVNode(graph, index)));
                wprintf(L"-[%s]-", ArcInfoToString(an)); // 输出边的长度
                level++;
                firstarc = false;
                needNewLine = true;
                DFS(graph, location);
            }
        }

        if (level != 0)
        {
            level--;
            return;
        }
        else
        {
            if (needNewLine)
                printf("\n");
            index++;
            firstarc = true;
        }
    }
}

void PrintALGraph(ALGraph graph)
{
    VNode *al = graph->vertices;
    bool *visited = calloc(graph->vexnum, sizeof(bool));
    bool needNewLine;

    wprintf(L"普通遍历图：\n");
    for (int i = 0; i < graph->vexnum; i++)
    {
        needNewLine = false;
        if (al[i]->firstarc == NULL && visited[i] == false)
        {
            wprintf(L"%s", VerTexToString(al[i]));
            needNewLine = true;
        }

        visited[i] = true;

        for (ArcNode an = al[i]->firstarc; an != NULL; an = an->nextarc)
        {
            int location = LocaleVex(graph, an->adjvex->data);
            if (!visited[location])
            {
                wprintf(L"%s-[%s]-%s  ", VerTexToString(al[i]), ArcInfoToString(an), VerTexToString(an->adjvex));
                needNewLine = true;
                visited[location] = true;
            }
        }

        if (needNewLine)
            printf("\n");
    }
}

// 实现接口函数

VerTexType GetVerticeData(void)
{
    VerTexType data;
    ClearInput();
    scanf("%c", &data);
    return data;
}

OtherInfo GetArcInfo(void)
{
    OtherInfo info;
    ClearInput();
    scanf("%d", &info);
    return info;
}

wchar_t *VerTexToString(VNode vn)
{
    const int BUFFERSIZE = 20;
    wchar_t *buffer = calloc(BUFFERSIZE, sizeof(wchar_t));

    // wcscpy(buffer, (wchar_t *)&vn->data);
    // wcscat(buffer, L"\0");

    buffer[0] = vn->data;
    buffer[1] = '\0';

    return buffer;
}

wchar_t *ArcInfoToString(ArcNode an)
{
    const int BUFFERSIZE = 20;
    wchar_t *buffer = calloc(BUFFERSIZE, sizeof(wchar_t));

    // char tempchar = an->info + '0';
    // wcscpy(buffer, (wchar_t *)&tempchar);

    buffer[0] = an->info + '0';
    buffer[1] = '\0';

    return buffer;
}

void ClearInput(void)
{
    while (getchar() != '\n')
        ;
}

int main(void)
{
    setlocale(LC_ALL, "chs");

    ALGraph graph = CreateALGraph();
    DFS(graph, 0);
    printf("\n");
    PrintALGraph(graph);

    system("pause");
    return 0;
}
