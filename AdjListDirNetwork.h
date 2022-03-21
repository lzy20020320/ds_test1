#ifndef __ADJ_MATRIX_UNDIR_GRAPH_H__
#define __ADJ_MATRIX_UNDIR_GRAPH_H__
#include <iostream>
#include <queue>
#include <vector>
#include <ctime>
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
    bool hasCycle(int v);   /// TODO 记得写注释！！
    bool hasCycle();    /// 记得写注释！！

    // Yunzhe Li
    void Traverse(void (*Visit)(const ElemType &));                // 遍历
    bool Cycle();                                                 // 判断是否有环，拓扑排序实现
    bool CycleDFS(int v, std::vector<int> &output);               // 深度遍历是否有环
    void SetTagVisited(int v);                                    // 标记顶点已经访问
    Status GetTag(int v) const;                                   // 返回tag
    void ClearTag();


    Status GetElem(int v, ElemType &d) const;// 求顶点的 元素值
    int GetVexNum() const;					// 返回顶点个数
    int GetArcNum() const;					// 返回边数

    int FirstOutAdjVex(int v) const;				 // 求有向网中顶点v的第一个邻接点
    int NextOutAdjVex(int v1, int v2) const;		 // 求有向网中顶点v1的相对于v2的下一个邻接点
    void SecShortestPath(int v1,int v2);    // 输出两点之间的第二短路径

    void Dijkstra(int start,vector<int> &pre_point,vector<int> &distance);  // 最短路径算法，pre_point为前驱顶点，distance为距离
    void DijkstraSec(int start,vector<int> &pre_point,vector<int> &min, vector<int> &sec_min, vector<int> &pre_point_sec);
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
        if (arcs[v][u] != infinity && v != u) {
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

//-------------------------------------------------------------------------------------------------
template<class ElemType>
bool AdjMatrixdirNetwork<ElemType>::Cycle()
{
    /// 思路：参考拓扑排序
    /// 将所有入度为0的点入队；每次从队列中pop一个顶点，直至为空；
    /// 遍历所有与pop出来这个顶点相连的顶点，并将相连顶点入度减一，若减一后入度为0，入队。
    /// 若最终cnt == 顶点个数，说明所有顶点都被访问到，说明没cycle，否则说明有顶点入度不为1.
    /// 时间复杂度：O(n*n), 空间复杂度：O(1)
    int cnt = 0;
    queue<int> queue;
    std::vector<int> in_degree;
    std::vector<int> linked_vex;
    // 算各顶点入度
    for(int vex = 0; vex < vexNum; vex++)
    {
        in_degree.push_back(this->CountInDegree(vex));
        if(in_degree[vex] == 0)
            queue.push(vex);
    }
    while(!queue.empty())
    {
        linked_vex.clear();
        int front_elem = queue.front();
        for(int i = 0; i < vexNum; i++)          // 找出与pop顶点相连的顶点，push入linked_vex
        {
            if(arcs[front_elem][i] != infinity && front_elem != i)
                linked_vex.push_back(i);
        }
        queue.pop();
        cnt ++;
        for(int i : linked_vex)                   // 遍历相连结点，删边
        {
            if (--in_degree[i] == 0)              // 若删边后，结点入度为0，push入队
                queue.push(i);
        }
    }
    if(cnt == vexNum)
        return false;
    else
        return true;
}

template<class ElemType>
void AdjMatrixdirNetwork<ElemType>::Traverse(void (*Visit) (const ElemType &))
{
    for(int vex : vertexes)
    {
        Visit(vex);
    }
}
template<class ElemType>
bool AdjMatrixdirNetwork<ElemType>::CycleDFS(int v, std::vector<int> &output)
{
    /// 深度优先遍历，求回路
    /// 从v顶点开始，依次访问并DFS与之相连的顶点，访问到的顶点对其tag进行标记，当再次即将访问到已被访问到的顶点时，即：有环
    /// 若DFS整个图后，未发现，则无环。
    /// 对于删除顶点后的图，只能处理删除最后一个顶点的情况
    /// 时间复杂度：O(n*n), 空间复杂度：O(n)
    bool flag = false;              // 用于标记是否有环
    int pre_vex = v;                // 用于记录上一个结点
    SetTagVisited(v);               // 将访问到的结点标记为Visited

    for(int w = FirstOutAdjVex(v); w != -1; w = NextOutAdjVex(v, w))    // 遍历与v点相连的顶点
    {
        if (GetTag(w) == VISITED && w != pre_vex)                // 若与v相连的顶点已经被访问过，且不是父顶点，则是回路的头（尾）
        {
            output.emplace_back(w);                             // 将头（尾）push_back
            return true;
        }
        else if (GetTag(w) == UNVISITED)                         // 若未访问过
        {
            flag = CycleDFS(w, output);                     // 对与v相连的第一个点进行DFS
            if(flag && output[0] == 0)                          // 用于回溯时记录回路尾顶点是否被访问过，若标记为1，则跳过
            {
                output.emplace_back(w);                         // 回溯时push_back经过的顶点
                if(w == output[1])                              // 若再次回溯到相同的结点，则标记output的第一个元素，标记为1
                    output[0] = 1;
            }
            return flag;
        }
    }
    return false;
}

template<class ElemType>
void AdjMatrixdirNetwork<ElemType>::SetTagVisited(int v)
{
    tag[v] = VISITED;
}

template<class ElemType>
Status AdjMatrixdirNetwork<ElemType>::GetTag(int v) const
{
    return tag[v];
}

template<class ElemType>
void AdjMatrixdirNetwork<ElemType>::ClearTag()
{
    for(int vex = 0; vex < vexNum; ++vex)
        tag[vex] = UNVISITED;

}


//-------------------------------------------------------------------------------------------------

template<class ElemType>
void AdjMatrixdirNetwork<ElemType>::Dijkstra(int start, vector<int> &pre_point, vector<int> &distance) {
    bool *flag = new bool [vexNum];
    distance.clear();
    pre_point.clear();
    for(int i=0;i<vexNum;++i){
        flag[i]=false;
        distance.emplace_back(arcs[start][i]);    // 初始化距离
        pre_point.emplace_back(start);  // 初始化前驱顶点为0
    }
    flag[start]=true, distance[start]=0; // 初始化起点
    for(int i=1;i<vexNum;++i){     // 循环n-1次（除了自己）
        int min_distance(infinity),point(0);
        for(int j=0;j<vexNum;++j)   // 找到距离最近的点
            if(!flag[j] && distance[j]<min_distance){
                min_distance=distance[j];
                point=j;
            }
        flag[point] = true;   // 标记已得到从start到point的最短路径
        for(int j=0;j<vexNum;++j){
            int start2next_distance = arcs[point][j] == infinity ? infinity : min_distance + arcs[point][j]; // 防止溢出
            if(!flag[j] && start2next_distance < distance[j]){
                distance[j]=start2next_distance;    // 若次路径更短则更新最短距离和前驱点
                pre_point[j]=point;
            }
        }
    }
    delete[] flag;
}

template<class ElemType>
void AdjMatrixdirNetwork<ElemType>::SecShortestPath(int start, int end) {
    vector<int> min_distance,pre_point,sec_min_distance,pre_point_sec;
    bool *flag = new bool [vexNum];
    for(int i=0;i<vexNum;++i){
        flag[i]=false;
        min_distance.emplace_back(arcs[start][i]);    // 初始化距离
        pre_point.emplace_back(start);  // 初始化前驱顶点为0
        sec_min_distance.emplace_back(infinity);
        pre_point_sec.emplace_back(start);
    }
    flag[start]=true, min_distance[start]=0; // 初始化起点
    for(int i=1;i<vexNum;++i){     // 循环n-1次（除了自己）
        int min_distance_from_start(infinity),point(0);
        for(int j=0;j<vexNum;++j)   // 找到距离最近的点
            if(!flag[j] && min_distance[j] < min_distance_from_start){
                min_distance_from_start=min_distance[j];
                point=j;
            }
        flag[point] = true;   // 标记已得到从start到point的最短路径
        for(int j=0;j<vexNum;++j){
            int start2next_distance = arcs[point][j] == infinity ? infinity : min_distance_from_start + arcs[point][j]; // 防止溢出
            if(!flag[j] && start2next_distance < min_distance[j]){
                sec_min_distance[j]=min_distance[j];
                pre_point_sec[j]=pre_point[j];
                min_distance[j]=start2next_distance;    // 若次路径更短则更新最短距离和前驱点
                pre_point[j]=point;
            }else if(start2next_distance < sec_min_distance[j] && start2next_distance > min_distance[j]){   // 筛选第二长的
                sec_min_distance[j] = start2next_distance;
                pre_point_sec[j]=point;
            }
        }
    }
    if(sec_min_distance[end]==infinity) // 输出
        cout<<"not found second shortest path"<<endl;
    else{
        cout<<vertexes[end];
        for(int i=pre_point_sec[end];i != start;i = (sec_min_distance[i] > min_distance[i]) ?
                pre_point[i]:pre_point_sec[i])
            cout<<" <- "<<vertexes[i];
        cout<<" <- "<<vertexes[start]<<endl;
    }
    delete[] flag;
}

template<class ElemType>
void AdjMatrixdirNetwork<ElemType>::DijkstraSec(int start, vector<int> &pre_point,
                                                vector<int> &min, vector<int> &sec_min, vector<int> &pre_point_sec) {
    bool *flag = new bool [vexNum];
    min.clear();
    pre_point.clear();
    sec_min.clear();
    pre_point_sec.clear();
    for(int i=0;i<vexNum;++i){
        flag[i]=false;
        min.emplace_back(arcs[start][i]);    // 初始化距离
        pre_point.emplace_back(start);  // 初始化前驱顶点为0
        sec_min.emplace_back(infinity);
        pre_point_sec.emplace_back(start);
    }
    flag[start]=true, min[start]=0; // 初始化起点
    for(int i=1;i<vexNum;++i){     // 循环n-1次（除了自己）
        int min_distance(infinity),point(0);
        for(int j=0;j<vexNum;++j)   // 找到距离最近的点
            if(!flag[j] && min[j]<min_distance){
                min_distance=min[j];
                point=j;
            }
        flag[point] = true;   // 标记已得到从start到point的最短路径
        for(int j=0;j<vexNum;++j){
            int start2next_distance = arcs[point][j] == infinity ? infinity : min_distance + arcs[point][j]; // 防止溢出
            if(!flag[j] && start2next_distance < min[j]){
                sec_min[j]=min[j];
                pre_point_sec[j]=pre_point[j];
                min[j]=start2next_distance;    // 若次路径更短则更新最短距离和前驱点
                pre_point[j]=point;
            }else if(start2next_distance < sec_min[j] && start2next_distance >min[j]){
                sec_min[j] = start2next_distance;
                pre_point_sec[j]=point;
            }
        }
    }
    delete[] flag;
}


#endif


