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
    // �ڽӾ�������ݳ�Ա:
    int vexNum, vexMaxNum, arcNum;			// ������Ŀ������Ķ��������Ŀ�ͱ���
    int infinity;
    int** arcs;							    // ��ű���Ϣ�ڽӾ���
    ElemType* vertexes;						// ��Ŷ�����Ϣ������
    mutable Status* tag;
public:
// �ڽӾ������͵ķ�������:
    AdjMatrixdirNetwork(ElemType es[], int vertexNum, int vertexMaxNum = DEFAULT_SIZE,int inf = DEFAULT_INFINITY);
    // ������es[]Ϊ����,�������ΪvertexNum,����Ķ��������ĿΪvertexMaxNum,����Ϊ0������ͼ
    explicit AdjMatrixdirNetwork(int vertexMaxNum = DEFAULT_SIZE,int inf = INFINITY);
    // ��������Ķ��������ĿΪvertexMaxNum,����Ϊ0������ͼ
    ~AdjMatrixdirNetwork();					// ��������
    void Clear();			              // ���ͼ
    bool IsEmpty();                 // �ж�����ͼ�Ƿ�Ϊ��
    bool hasCycle(int v);   /// TODO �ǵ�дע�ͣ���
    bool hasCycle();    /// �ǵ�дע�ͣ���

    // Yunzhe Li
    void Traverse(void (*Visit)(const ElemType &));                // ����
    bool Cycle();                                                 // �ж��Ƿ��л�����������ʵ��
    bool CycleDFS(int v, std::vector<int> &output);               // ��ȱ����Ƿ��л�
    void SetTagVisited(int v);                                    // ��Ƕ����Ѿ�����
    Status GetTag(int v) const;                                   // ����tag
    void ClearTag();


    Status GetElem(int v, ElemType &d) const;// �󶥵�� Ԫ��ֵ
    int GetVexNum() const;					// ���ض������
    int GetArcNum() const;					// ���ر���

    int FirstOutAdjVex(int v) const;				 // ���������ж���v�ĵ�һ���ڽӵ�
    int NextOutAdjVex(int v1, int v2) const;		 // ���������ж���v1�������v2����һ���ڽӵ�
    void SecShortestPath(int v1,int v2);    // �������֮��ĵڶ���·��

    void Dijkstra(int start,vector<int> &pre_point,vector<int> &distance);  // ���·���㷨��pre_pointΪǰ�����㣬distanceΪ����
    void DijkstraSec(int start,vector<int> &pre_point,vector<int> &min, vector<int> &sec_min, vector<int> &pre_point_sec);
    int CountOutDegree(int v) const;
    int CountInDegree(int v) const;
    int GetOrder(ElemType &d) const;// �󶥵�����
    void InsertVex(const ElemType &d);			 // ����Ԫ��ֵΪd�Ķ���
    void InsertArc(int v1, int v2, int weight);			     // ���붥��Ϊv1��v2�ı�
    void DeleteVex(const ElemType &d);			 // ɾ��Ԫ��ֵΪd�Ķ���
    void DeleteArc(int v1, int v2);			     // ɾ������Ϊv1��v2�ı�
    AdjMatrixdirNetwork(const AdjMatrixdirNetwork<ElemType> &g);	// ���ƹ��캯��
    AdjMatrixdirNetwork<ElemType> &operator =(const AdjMatrixdirNetwork<ElemType> &g);
    // ��ֵ�������
    void Display();	                         // ��ʾ�ڽӾ�������ͼ
};

// ����ͼ���ڽӾ������ʵ�ֲ���
template <class ElemType>
AdjMatrixdirNetwork<ElemType>::AdjMatrixdirNetwork(ElemType es[], int vertexNum, int vertexMaxNum, int inf)
// �����������������Ԫ��Ϊes[],�������ΪvertexNum,����Ķ��������ĿΪvertexMaxNum,����Ϊ0������ͼ

{
    if (vertexMaxNum < 0)
        throw Error("����Ķ��������Ŀ����Ϊ��!");        // �׳��쳣

    if (vertexMaxNum < vertexNum)
        throw Error("������Ŀ���ܴ�������Ķ��������Ŀ!");// �׳��쳣

    infinity = inf;
    vexNum = vertexNum;
    vexMaxNum = vertexMaxNum;
    arcNum = 0;

    vertexes = new ElemType[vexMaxNum];      // �������ɶ�����Ϣ����
    tag = new Status[vexMaxNum];			       // ���ɱ�־����
    arcs = (int **)new int *[vexMaxNum];     // �����ڽӾ���
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
// �������������������������ĿΪvertexMaxNum�Ŀ�����ͼ
{
    if (vertexMaxNum < 0)
        throw Error("����Ķ��������Ŀ����Ϊ��!");  // �׳��쳣

    vexNum = 0;
    vexMaxNum = vertexMaxNum;
    arcNum = 0;
    infinity = inf;

    vertexes = new ElemType[vexMaxNum];             // �������ɶ�����Ϣ����
    tag = new Status[vexMaxNum];			        // ���ɱ�־����
    arcs = (int **)new int *[vexMaxNum];            // �����ڽӾ���
    for (int v = 0; v < vexMaxNum; v++)
        arcs[v] = new int[vexMaxNum];
}

template <class ElemType>
void AdjMatrixdirNetwork<ElemType>::Clear()
// ���������������ͼ�Ķ������ͱ�������Ϊ0.
{
    vexNum = 0;
    arcNum = 0;
}

template <class ElemType>
bool AdjMatrixdirNetwork<ElemType>::IsEmpty()
// ����������������ͼΪ�շ���true,���򷵻�false.
{
    return vexNum == 0;
}

template <class ElemType>
AdjMatrixdirNetwork<ElemType>::~AdjMatrixdirNetwork()
// ����������ͷŶ�����ռ�õĿռ�
{
    delete []vertexes;					// �ͷŶ�������
    delete []tag;						    // �ͷű�־

    for (int v = 0; v < vexMaxNum; v++)	// �ͷ��ڽӾ������
        delete []arcs[v];
    delete []arcs;					    // �ͷ��ڽӾ���
}


template <class ElemType>
Status AdjMatrixdirNetwork<ElemType>::GetElem(int v, ElemType &d) const
// ��������������Ϊv�Ķ���ֵ, v��ȡֵ��ΧΪ0 �� v �� vexNum, v�Ϸ�ʱ����
// ͨ��dȡ�ö���ֵ��������ENTRY_FOUND������������NOT_PRESENT
{
    if (v < 0 || v >= vexNum)
        return NOT_PRESENT;	// v��Χ��,����Ԫ�ز�����
    else	{
        d = vertexes[v];	  // ������v��Ԫ��ֵ����d
        return ENTRY_FOUND;	// ����Ԫ�ش���
    }
}


template <class ElemType>
int AdjMatrixdirNetwork<ElemType>::GetVexNum() const
// ������������ض������
{
    return vexNum;
}

template <class ElemType>
int AdjMatrixdirNetwork<ElemType>::GetArcNum() const
// ������������ر���
{
    return arcNum;
}


template <class ElemType>
void AdjMatrixdirNetwork<ElemType>::InsertVex(const ElemType &d)
// ������������붥��d
{
    if (vexNum == vexMaxNum)
        throw Error("ͼ�Ķ��������ܳ�������������!");	// �׳��쳣

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
// ���������������������v1��v2�ı�
{
    if (v1 < 0 || v1 >= vexNum)
        throw Error("v1���Ϸ�!");	// �׳��쳣
    if (v2 < 0 || v2 >= vexNum)
        throw Error("v2���Ϸ�!");	// �׳��쳣
    if (v1 == v2)
        throw Error("v1���ܵ���v2!");		// �׳��쳣
    if (weight <= 0 )
        throw Error("������ͬ�����֮���Ȩ��Ҫ����0!");



    if (arcs[v1][v2] == infinity)	{	  // ԭ����ͼ��û�б�(v1, v2)
        arcNum++;
        if(weight<infinity)
            arcs[v1][v2] = weight;
        else
            arcs[v1][v2] = infinity;
    }
}

template <class ElemType>
void AdjMatrixdirNetwork<ElemType>::DeleteVex(const ElemType &d)
// ���������ɾ��Ԫ��Ϊd�Ķ���
{
    int v;
    for (v = 0; v < vexNum; v++)
        if	(vertexes[v] == d)
            break;
    if (v == vexNum)
        throw Error("ͼ�в�����Ҫɾ���Ķ���!");	// �׳��쳣

    for (int u = 0; u < vexNum; u++)                // ɾ���붥��d������ı�
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
// ���������ɾ����������v1��v2�ı�
{
    if (v1 < 0 || v1 >= vexNum)
        throw Error("v1���Ϸ�!");	// �׳��쳣
    if (v2 < 0 || v2 >= vexNum)
        throw Error("v2���Ϸ�!");	// �׳��쳣

    if (arcs[v1][v2] != infinity)	{	// ԭ����ͼ���ڱ�(v1, v2)
        arcNum--;
        arcs[v1][v2] = infinity;
    }
}


template <class ElemType>
AdjMatrixdirNetwork<ElemType>::AdjMatrixdirNetwork(const AdjMatrixdirNetwork<ElemType> &g)
// ���������������ͼ���ڽӾ���g����������ͼ���ڽӾ���g�������ƹ��캯��
{
    vexNum = g.vexNum;
    vexMaxNum = g.vexMaxNum;
    arcNum = g.arcNum;
    infinity = g.infinity;

    vertexes = new ElemType[vexMaxNum];		// ���ɶ�����������
    tag = new Status[vexMaxNum];			// ���ɱ�־����
    arcs = (int **)new int *[vexMaxNum];	// �����ڽӾ���
    for (int v = 0; v < vexMaxNum; v++)
        arcs[v] = new int[vexMaxNum];

    for (int v = 0; v < vexNum; v++)	{	// ���ƶ�����������
        vertexes[v] = g.vertexes[v];
        tag[v] = g.tag[v];
        for (int u = 0; u < vexNum; u++)
            arcs[v][u] = g.arcs[v][u];
    }
}

template <class ElemType>
AdjMatrixdirNetwork<ElemType> &AdjMatrixdirNetwork<ElemType>::operator =(const AdjMatrixdirNetwork<ElemType> &g)
// ���������������ͼ���ڽӾ���g��ֵ����ǰ����ͼ���ڽӾ��󡪡���ֵ�������
{
    if (&g != this)	{
        delete []vertexes;				// �ͷŶ�������
        delete []tag;						  // �ͷű�־

        for (int v = 0; v < vexMaxNum; v++)	// �ͷ��ڽӾ������
            delete []arcs[v];
        delete []arcs;					  // �ͷ��ڽӾ���
        vexNum = g.vexNum;
        vexMaxNum = g.vexMaxNum;
        arcNum = g.arcNum;

        vertexes = new ElemType[vexMaxNum];	 // ���ɶ�����������
        tag = new Status[vexMaxNum];			   // ���ɱ�־����
        arcs = (int **)new int *[vexMaxNum]; // �����ڽӾ���
        for (int v = 0; v < vexMaxNum; v++)
            arcs[v] = new int[vexMaxNum];

        for (int v = 0; v < vexNum; v++)	{	 // ���ƶ�����������
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
// �������: ��ʾ�ڽӾ�������ͼ
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
            return v;	    // ����d����,�����������
    return -1;	        // ����d������,����-1
}

template<class ElemType>
int AdjMatrixdirNetwork<ElemType>::CountOutDegree(int v) const {
    int out_degree = 0;
    if (v < 0 || v >= vexNum)
        throw Error("v1���Ϸ�!");	// �׳��쳣
    for (int u = 0; u < vexNum; u++)
        if(arcs[v][u] != infinity && arcs[v][u] != 0 )  out_degree++;
    return out_degree;
}

template<class ElemType>
int AdjMatrixdirNetwork<ElemType>::CountInDegree(int v) const {
    int in_degree = 0;
    if (v < 0 || v >= vexNum)
        throw Error("v1���Ϸ�!");	// �׳��쳣
    for (int u = 0; u < vexNum; u++)
        if(arcs[u][v] != infinity && arcs[u][v] != 0)  in_degree++;
    return in_degree;
}


template<class ElemType>
bool AdjMatrixdirNetwork<ElemType>::hasCycle(int v) {
    std::queue<int> elem_queue;
    elem_queue.push(v);                                     // �����д������Ҫ�ߵĽڵ�
    for(int i = 0;i<vexNum && !elem_queue.empty();i++){     // ������ڻ���ô·������<=vexNum�������п�˵���ߵ���ͷ�߲���ȥ��
        int temp,next_Vex;                                  // next_Vex Ϊ�ýڵ�����ߵ��ĸ�������
        temp = elem_queue.front();
        elem_queue.pop();
        next_Vex = this->FirstOutAdjVex(temp);
        if(next_Vex == v)    return true;                   // �ߵ�����ĵ�ʹ���Ȧ
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
    /// ˼·���ο���������
    /// ���������Ϊ0�ĵ���ӣ�ÿ�δӶ�����popһ�����㣬ֱ��Ϊ�գ�
    /// ����������pop����������������Ķ��㣬��������������ȼ�һ������һ�����Ϊ0����ӡ�
    /// ������cnt == ���������˵�����ж��㶼�����ʵ���˵��ûcycle������˵���ж�����Ȳ�Ϊ1.
    /// ʱ�临�Ӷȣ�O(n*n), �ռ临�Ӷȣ�O(1)
    int cnt = 0;
    queue<int> queue;
    std::vector<int> in_degree;
    std::vector<int> linked_vex;
    // ����������
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
        for(int i = 0; i < vexNum; i++)          // �ҳ���pop���������Ķ��㣬push��linked_vex
        {
            if(arcs[front_elem][i] != infinity && front_elem != i)
                linked_vex.push_back(i);
        }
        queue.pop();
        cnt ++;
        for(int i : linked_vex)                   // ����������㣬ɾ��
        {
            if (--in_degree[i] == 0)              // ��ɾ�ߺ󣬽�����Ϊ0��push���
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
    /// ������ȱ��������·
    /// ��v���㿪ʼ�����η��ʲ�DFS��֮�����Ķ��㣬���ʵ��Ķ������tag���б�ǣ����ٴμ������ʵ��ѱ����ʵ��Ķ���ʱ�������л�
    /// ��DFS����ͼ��δ���֣����޻���
    /// ����ɾ��������ͼ��ֻ�ܴ���ɾ�����һ����������
    /// ʱ�临�Ӷȣ�O(n*n), �ռ临�Ӷȣ�O(n)
    bool flag = false;              // ���ڱ���Ƿ��л�
    int pre_vex = v;                // ���ڼ�¼��һ�����
    SetTagVisited(v);               // �����ʵ��Ľ����ΪVisited

    for(int w = FirstOutAdjVex(v); w != -1; w = NextOutAdjVex(v, w))    // ������v�������Ķ���
    {
        if (GetTag(w) == VISITED && w != pre_vex)                // ����v�����Ķ����Ѿ������ʹ����Ҳ��Ǹ����㣬���ǻ�·��ͷ��β��
        {
            output.emplace_back(w);                             // ��ͷ��β��push_back
            return true;
        }
        else if (GetTag(w) == UNVISITED)                         // ��δ���ʹ�
        {
            flag = CycleDFS(w, output);                     // ����v�����ĵ�һ�������DFS
            if(flag && output[0] == 0)                          // ���ڻ���ʱ��¼��·β�����Ƿ񱻷��ʹ��������Ϊ1��������
            {
                output.emplace_back(w);                         // ����ʱpush_back�����Ķ���
                if(w == output[1])                              // ���ٴλ��ݵ���ͬ�Ľ�㣬����output�ĵ�һ��Ԫ�أ����Ϊ1
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
        distance.emplace_back(arcs[start][i]);    // ��ʼ������
        pre_point.emplace_back(start);  // ��ʼ��ǰ������Ϊ0
    }
    flag[start]=true, distance[start]=0; // ��ʼ�����
    for(int i=1;i<vexNum;++i){     // ѭ��n-1�Σ������Լ���
        int min_distance(infinity),point(0);
        for(int j=0;j<vexNum;++j)   // �ҵ���������ĵ�
            if(!flag[j] && distance[j]<min_distance){
                min_distance=distance[j];
                point=j;
            }
        flag[point] = true;   // ����ѵõ���start��point�����·��
        for(int j=0;j<vexNum;++j){
            int start2next_distance = arcs[point][j] == infinity ? infinity : min_distance + arcs[point][j]; // ��ֹ���
            if(!flag[j] && start2next_distance < distance[j]){
                distance[j]=start2next_distance;    // ����·�������������̾����ǰ����
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
        min_distance.emplace_back(arcs[start][i]);    // ��ʼ������
        pre_point.emplace_back(start);  // ��ʼ��ǰ������Ϊ0
        sec_min_distance.emplace_back(infinity);
        pre_point_sec.emplace_back(start);
    }
    flag[start]=true, min_distance[start]=0; // ��ʼ�����
    for(int i=1;i<vexNum;++i){     // ѭ��n-1�Σ������Լ���
        int min_distance_from_start(infinity),point(0);
        for(int j=0;j<vexNum;++j)   // �ҵ���������ĵ�
            if(!flag[j] && min_distance[j] < min_distance_from_start){
                min_distance_from_start=min_distance[j];
                point=j;
            }
        flag[point] = true;   // ����ѵõ���start��point�����·��
        for(int j=0;j<vexNum;++j){
            int start2next_distance = arcs[point][j] == infinity ? infinity : min_distance_from_start + arcs[point][j]; // ��ֹ���
            if(!flag[j] && start2next_distance < min_distance[j]){
                sec_min_distance[j]=min_distance[j];
                pre_point_sec[j]=pre_point[j];
                min_distance[j]=start2next_distance;    // ����·�������������̾����ǰ����
                pre_point[j]=point;
            }else if(start2next_distance < sec_min_distance[j] && start2next_distance > min_distance[j]){   // ɸѡ�ڶ�����
                sec_min_distance[j] = start2next_distance;
                pre_point_sec[j]=point;
            }
        }
    }
    if(sec_min_distance[end]==infinity) // ���
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
        min.emplace_back(arcs[start][i]);    // ��ʼ������
        pre_point.emplace_back(start);  // ��ʼ��ǰ������Ϊ0
        sec_min.emplace_back(infinity);
        pre_point_sec.emplace_back(start);
    }
    flag[start]=true, min[start]=0; // ��ʼ�����
    for(int i=1;i<vexNum;++i){     // ѭ��n-1�Σ������Լ���
        int min_distance(infinity),point(0);
        for(int j=0;j<vexNum;++j)   // �ҵ���������ĵ�
            if(!flag[j] && min[j]<min_distance){
                min_distance=min[j];
                point=j;
            }
        flag[point] = true;   // ����ѵõ���start��point�����·��
        for(int j=0;j<vexNum;++j){
            int start2next_distance = arcs[point][j] == infinity ? infinity : min_distance + arcs[point][j]; // ��ֹ���
            if(!flag[j] && start2next_distance < min[j]){
                sec_min[j]=min[j];
                pre_point_sec[j]=pre_point[j];
                min[j]=start2next_distance;    // ����·�������������̾����ǰ����
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


