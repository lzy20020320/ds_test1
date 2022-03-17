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
    bool hasCycle(int v);
    bool hasCycle();
    bool Cycle();
    Status GetElem(int v, ElemType &d) const;// �󶥵�� Ԫ��ֵ
    int GetVexNum() const;					// ���ض������
    int GetArcNum() const;					// ���ر���

    int FirstOutAdjVex(int v) const;				 // ���������ж���v�ĵ�һ���ڽӵ�
    int NextOutAdjVex(int v1, int v2) const;		 // ���������ж���v1�������v2����һ���ڽӵ�
    bool CheckRow(int row, int source_vex);

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


