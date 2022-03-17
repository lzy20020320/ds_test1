#ifndef __ADJ_MATRIX_UNDIR_GRAPH_H__
#define __ADJ_MATRIX_UNDIR_GRAPH_H__
#include <iostream>
#include <queue>
#include <vector>
#include "Assistance.h"
using namespace std;

template <class ElemType>
class AdjMatrixdirNetwork
{
private:
    // 邻接矩阵的数据成员:
    int vexNum, vexMaxNum, arcNum;			// 顶点数目、允许的顶点最大数目和边数
    int infinity;
    int** arcs;							    // 存放边信息邻接矩阵
    ElemType* vertexes;						// 存放顶点信息的数组
    mutable Status* tag;
public:
// 邻接矩阵类型的方法声明:
    AdjMatrixdirNetwork(ElemType es[], int vertexNum, int vertexMaxNum = DEFAULT_SIZE,int inf = DEFAULT_INFINITY);
    // 以数组es[]为顶点,顶点个数为vertexNum,允许的顶点最大数目为vertexMaxNum,边数为0的有向图
    explicit AdjMatrixdirNetwork(int vertexMaxNum = DEFAULT_SIZE,int inf = INFINITY);
    // 构造允许的顶点最大数目为vertexMaxNum,边数为0的有向图
    ~AdjMatrixdirNetwork();					// 析构函数
    void Clear();			              // 清空图
    bool IsEmpty();                 // 判断有向图是否为空
    bool hasCycle(int v);
    bool hasCycle();
    bool Cycle();
    Status GetElem(int v, ElemType &d) const;// 求顶点的 元素值
    int GetVexNum() const;					// 返回顶点个数
    int GetArcNum() const;					// 返回边数

    int FirstOutAdjVex(int v) const;				 // 求有向网中顶点v的第一个邻接点
    int NextOutAdjVex(int v1, int v2) const;		 // 求有向网中顶点v1的相对于v2的下一个邻接点
    bool CheckRow(int row, int source_vex);

    int CountOutDegree(int v) const;
    int CountInDegree(int v) const;
    int GetOrder(ElemType &d) const;// 求顶点的序号
    void InsertVex(const ElemType &d);			 // 插入元素值为d的顶点
    void InsertArc(int v1, int v2, int weight);			     // 插入顶点为v1和v2的边
    void DeleteVex(const ElemType &d);			 // 删除元素值为d的顶点
    void DeleteArc(int v1, int v2);			     // 删除顶点为v1和v2的边
    AdjMatrixdirNetwork(const AdjMatrixdirNetwork<ElemType> &g);	// 复制构造函数
    AdjMatrixdirNetwork<ElemType> &operator =(const AdjMatrixdirNetwork<ElemType> &g);
    // 赋值语句重载
    void Display();	                         // 显示邻接矩阵有向图
};

// 有向图的邻接矩阵类的实现部分
template <class ElemType>
AdjMatrixdirNetwork<ElemType>::AdjMatrixdirNetwork(ElemType es[], int vertexNum, int vertexMaxNum, int inf)
// 操作结果：构造数据元素为es[],顶点个数为vertexNum,允许的顶点最大数目为vertexMaxNum,边数为0的有向图

{
    if (vertexMaxNum < 0)
        throw Error("允许的顶点最大数目不能为负!");        // 抛出异常

    if (vertexMaxNum < vertexNum)
        throw Error("顶点数目不能大于允许的顶点最大数目!");// 抛出异常

    infinity = inf;
    vexNum = vertexNum;
    vexMaxNum = vertexMaxNum;
    arcNum = 0;

    vertexes = new ElemType[vexMaxNum];      // 生成生成顶点信息数组
    tag = new Status[vexMaxNum];			       // 生成标志数组
    arcs = (int **)new int *[vexMaxNum];     // 生成邻接矩阵
    for (int v = 0; v < vexMaxNum; v++)
        arcs[v] = new int[vexMaxNum];

    for (int v = 0; v < vexNum; v++) {
        vertexes[v] = es[v];
        tag[v] = UNVISITED;
        for (int u = 0; u < vexNum; u++)
            if(v == u)
                arcs[v][u] = 0;
            else
                arcs[v][u] = infinity;
    }
}

template <class ElemType>
AdjMatrixdirNetwork<ElemType>::AdjMatrixdirNetwork(int vertexMaxNum, int inf)
// 操作结果：构造允许顶点的最大数目为vertexMaxNum的空有向图
{
    if (vertexMaxNum < 0)
        throw Error("允许的顶点最大数目不能为负!");  // 抛出异常

    vexNum = 0;
    vexMaxNum = vertexMaxNum;
    arcNum = 0;
    infinity = inf;

    vertexes = new ElemType[vexMaxNum];             // 生成生成顶点信息数组
    tag = new Status[vexMaxNum];			        // 生成标志数组
    arcs = (int **)new int *[vexMaxNum];            // 生成邻接矩阵
    for (int v = 0; v < vexMaxNum; v++)
        arcs[v] = new int[vexMaxNum];
}

template <class ElemType>
void AdjMatrixdirNetwork<ElemType>::Clear()
// 操作结果：把有向图的顶点数和边数设置为0.
{
    vexNum = 0;
    arcNum = 0;
}

template <class ElemType>
bool AdjMatrixdirNetwork<ElemType>::IsEmpty()
// 操作结果：如果有向图为空返回true,否则返回false.
{
    return vexNum == 0;
}

template <class ElemType>
AdjMatrixdirNetwork<ElemType>::~AdjMatrixdirNetwork()
// 操作结果：释放对象所占用的空间
{
    delete []vertexes;					// 释放顶点数据
    delete []tag;						    // 释放标志

    for (int v = 0; v < vexMaxNum; v++)	// 释放邻接矩阵的行
        delete []arcs[v];
    delete []arcs;					    // 释放邻接矩阵
}


template <class ElemType>
Status AdjMatrixdirNetwork<ElemType>::GetElem(int v, ElemType &d) const
// 操作结果：求序号为v的顶点值, v的取值范围为0 ≤ v ＜ vexNum, v合法时函数
// 通过d取得顶点值，并返回ENTRY_FOUND；否则函数返回NOT_PRESENT
{
    if (v < 0 || v >= vexNum)
        return NOT_PRESENT;	// v范围错,返回元素不存在
    else	{
        d = vertexes[v];	  // 将顶点v的元素值赋给d
        return ENTRY_FOUND;	// 返回元素存在
    }
}


template <class ElemType>
int AdjMatrixdirNetwork<ElemType>::GetVexNum() const
// 操作结果：返回顶点个数
{
    return vexNum;
}

template <class ElemType>
int AdjMatrixdirNetwork<ElemType>::GetArcNum() const
// 操作结果：返回边数
{
    return arcNum;
}


template <class ElemType>
void AdjMatrixdirNetwork<ElemType>::InsertVex(const ElemType &d)
// 操作结果：插入顶点d
{
    if (vexNum == vexMaxNum)
        throw Error("图的顶点数不能超过允许的最大数!");	// 抛出异常

    vertexes[vexNum] = d;
    tag[vexNum] = UNVISITED;
    for (int v = 0; v <= vexNum; v++) {
        arcs[vexNum][v] = infinity;
        arcs[v][vexNum] = infinity;
    }
    arcs[vexNum][vexNum] = 0;

    vexNum++;
}

template <class ElemType>
void AdjMatrixdirNetwork<ElemType>::InsertArc(int v1, int v2, int weight)
// 操作结果：插入依附顶点v1和v2的边
{
    if (v1 < 0 || v1 >= vexNum)
        throw Error("v1不合法!");	// 抛出异常
    if (v2 < 0 || v2 >= vexNum)
        throw Error("v2不合法!");	// 抛出异常
    if (v1 == v2)
        throw Error("v1不能等于v2!");		// 抛出异常
    if (weight <= 0 )
        throw Error("两个不同顶点点之间的权重要大于0!");



    if (arcs[v1][v2] == infinity)	{	  // 原有向图中没有边(v1, v2)
        arcNum++;
        if(weight<infinity)
            arcs[v1][v2] = weight;
        else
            arcs[v1][v2] = infinity;
    }
}

template <class ElemType>
void AdjMatrixdirNetwork<ElemType>::DeleteVex(const ElemType &d)
// 操作结果：删除元素为d的顶点
{
    int v;
    for (v = 0; v < vexNum; v++)
        if	(vertexes[v] == d)
            break;
    if (v == vexNum)
        throw Error("图中不存在要删除的顶点!");	// 抛出异常

    for (int u = 0; u < vexNum; u++)                // 删除与顶点d相关联的边
        if (arcs[v][u] == 1) {
            arcNum--;
            arcs[v][u] = infinity;
            arcs[u][v] = infinity;
        }

    vexNum--;
    if (v < vexNum) {
        vertexes[v] = vertexes[vexNum];
        tag[v] = tag[vexNum];
        for (int u = 0; u <= vexNum; u++)
            arcs[v][u] = arcs[vexNum][u];
        for (int u = 0; u <= vexNum; u++)
            arcs[u][v] = arcs[u][vexNum];
    }
}

template <class ElemType>
void AdjMatrixdirNetwork<ElemType>::DeleteArc(int v1, int v2)
// 操作结果：删除依附顶点v1和v2的边
{
    if (v1 < 0 || v1 >= vexNum)
        throw Error("v1不合法!");	// 抛出异常
    if (v2 < 0 || v2 >= vexNum)
        throw Error("v2不合法!");	// 抛出异常

    if (arcs[v1][v2] != infinity)	{	// 原有向图存在边(v1, v2)
        arcNum--;
        arcs[v1][v2] = infinity;
    }
}


template <class ElemType>
AdjMatrixdirNetwork<ElemType>::AdjMatrixdirNetwork(const AdjMatrixdirNetwork<ElemType> &g)
// 操作结果：由有向图的邻接矩阵g构造新有向图的邻接矩阵g——复制构造函数
{
    vexNum = g.vexNum;
    vexMaxNum = g.vexMaxNum;
    arcNum = g.arcNum;
    infinity = g.infinity;

    vertexes = new ElemType[vexMaxNum];		// 生成顶点数据数组
    tag = new Status[vexMaxNum];			// 生成标志数组
    arcs = (int **)new int *[vexMaxNum];	// 生成邻接矩阵
    for (int v = 0; v < vexMaxNum; v++)
        arcs[v] = new int[vexMaxNum];

    for (int v = 0; v < vexNum; v++)	{	// 复制顶点数据数组
        vertexes[v] = g.vertexes[v];
        tag[v] = g.tag[v];
        for (int u = 0; u < vexNum; u++)
            arcs[v][u] = g.arcs[v][u];
    }
}

template <class ElemType>
AdjMatrixdirNetwork<ElemType> &AdjMatrixdirNetwork<ElemType>::operator =(const AdjMatrixdirNetwork<ElemType> &g)
// 操作结果：将有向图的邻接矩阵g赋值给当前有向图的邻接矩阵——赋值语句重载
{
    if (&g != this)	{
        delete []vertexes;				// 释放顶点数据
        delete []tag;						  // 释放标志

        for (int v = 0; v < vexMaxNum; v++)	// 释放邻接矩阵的行
            delete []arcs[v];
        delete []arcs;					  // 释放邻接矩阵
        vexNum = g.vexNum;
        vexMaxNum = g.vexMaxNum;
        arcNum = g.arcNum;

        vertexes = new ElemType[vexMaxNum];	 // 生成顶点数据数组
        tag = new Status[vexMaxNum];			   // 生成标志数组
        arcs = (int **)new int *[vexMaxNum]; // 生成邻接矩阵
        for (int v = 0; v < vexMaxNum; v++)
            arcs[v] = new int[vexMaxNum];

        for (int v = 0; v < vexNum; v++)	{	 // 复制顶点数据数组
            vertexes[v] = g.vertexes[v];
            tag[v] = g.tag[v];
            for (int u = 0; u < vexNum; u++)
                arcs[v][u] = g.arcs[v][u];
        }
    }
    return *this;
}

template <class ElemType>
void AdjMatrixdirNetwork<ElemType>::Display()
// 操作结果: 显示邻接矩阵有向图
{

    for (int v = 0; v < vexNum; v++)
        cout << "\t" << vertexes[v];
    cout << endl;

    for (int v = 0; v < vexNum; v++)	{
        cout << vertexes[v];
        for (int u = 0; u < vexNum; u++)
            if(arcs[v][u] == infinity)
                cout<<"\t"<<"inf";
            else
                cout << "\t" << arcs[v][u];
        cout << endl;
    }
}

template<class ElemType>
int AdjMatrixdirNetwork<ElemType>::GetOrder(ElemType &d) const {
    for (int v = 0; v < vexNum; v++)
        if (vertexes[v] == d)
            return v;	    // 顶点d存在,返回它的序号
    return -1;	        // 顶点d不存在,返回-1
}

template<class ElemType>
int AdjMatrixdirNetwork<ElemType>::CountOutDegree(int v) const {
    int out_degree = 0;
    if (v < 0 || v >= vexNum)
        throw Error("v1不合法!");	// 抛出异常
    for (int u = 0; u < vexNum; u++)
        if(arcs[v][u] != infinity && arcs[v][u] != 0 )  out_degree++;
    return out_degree;
}

template<class ElemType>
int AdjMatrixdirNetwork<ElemType>::CountInDegree(int v) const {
    int in_degree = 0;
    if (v < 0 || v >= vexNum)
        throw Error("v1不合法!");	// 抛出异常
    for (int u = 0; u < vexNum; u++)
        if(arcs[u][v] != infinity && arcs[u][v] != 0)  in_degree++;
    return in_degree;
}

//template<class ElemType>
//bool AdjMatrixdirNetwork<ElemType>::hasCycle() {
//    bool flag = false;
//    for(int i = 0; i < vexNum; i++)
//    {
//        for(int j = 0; j < vexNum; j++)
//        {
//            if(arcs[i][j] == 1)
//                flag = CheckRow(j, i);
//        }
//        if(flag)
//            break;
//    }
//    return flag;
//}


template<class ElemType>
bool AdjMatrixdirNetwork<ElemType>::hasCycle(int v) {
    std::queue<int> elem_queue;
    elem_queue.push(v);                                     // 队列中存接下来要走的节点
    for(int i = 0;i<vexNum && !elem_queue.empty();i++){     // 如果存在环那么路径长度<=vexNum，当队列空说明走到尽头走不下去了
        int temp,next_Vex;                                  // next_Vex 为该节点可以走到哪个及诶点
        temp = elem_queue.front();
        elem_queue.pop();
        next_Vex = this->FirstOutAdjVex(temp);
        if(next_Vex == v)    return true;                   // 走到最初的点就存在圈
        if(next_Vex != -1)  elem_queue.push(next_Vex);
        while(next_Vex != -1){
            next_Vex = NextOutAdjVex(temp, next_Vex);
            if(next_Vex == v)    return true;
            if(next_Vex != -1)  elem_queue.push(next_Vex);
        }
    }
    return false;
}

template<class ElemType>
bool AdjMatrixdirNetwork<ElemType>::CheckRow(int row, int source_vex)
{
    for(int i = 0; i < vexNum; i++)
    {
        if(i == source_vex && arcs[row][i] != infinity)
            return true;
//		if(i == source_vex && arcs[row][i] == infinity)
//			return false;
        if(arcs[row][i] != infinity)
            CheckRow(i, source_vex);
    }
    return false;
}

template<class ElemType>
int AdjMatrixdirNetwork<ElemType>::FirstOutAdjVex(int v) const {
    if (v<0 && v>=vexNum)
        return -1;
    for(int i = 0;i<vexNum;i++)
        if(arcs[v][i] != infinity && arcs[v][i] != 0)
            return i;
    return -1;

}

template<class ElemType>
int AdjMatrixdirNetwork<ElemType>::NextOutAdjVex(int v1, int v2) const {
    if (v2<0 && v2>=vexNum)
        return -1;
    for(int i = v2 + 1;i<vexNum;i++)
        if(arcs[v1][i] != infinity && arcs[v1][i] != 0)
            return i;
    return -1;
}

template<class ElemType>
bool AdjMatrixdirNetwork<ElemType>::hasCycle() {
    for(int i = 0 ;i<vexNum;i++)
        if(hasCycle(i)) return true;

    return false;
}

template<class ElemType>
bool AdjMatrixdirNetwork<ElemType>::Cycle()
{
    int cnt = 0;
    queue<int> queue;
    std::vector<int> in_degree;
    for(int vex = 0; vex < vexNum; vex++)
    {
        in_degree.push_back(this->CountInDegree(vex));
        if(in_degree[vex] == 0)
            queue.push(vex);
    }

    while(!queue.empty())
    {
        int front_elem = queue.front();
        queue.pop();
        cnt ++;
        for(int i = 0; i < vexNum; i++)
        {
            if (i == front_elem)
                continue;
            if (--in_degree[i] == 0)
                queue.push(i);
        }
    }
    if(cnt == vexNum)
        return false;
    else
        return true;
}


#endif


