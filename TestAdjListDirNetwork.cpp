#include "AdjListDirNetwork.h"		// 邻接矩阵有向图

int main(void)
{
    try									// 用try封装可能出现异常的代码
    {
        char vexs[] = {'A', 'B', 'C', 'D', 'E'};
        int m[5][5] = {
                {0, DEFAULT_INFINITY, DEFAULT_INFINITY, DEFAULT_INFINITY, DEFAULT_INFINITY},
                {1, 0, 1, 1, 0},
                {DEFAULT_INFINITY, 0, 0, 2, 1},
                {1, 1, 1, 0, 0},
                {1, 0, 1, 0, 0},
        };
        char c = '0', e, e1, e2;
        int n = 5, v, v1, v2,w;

        AdjMatrixdirNetwork<char> g(vexs, n, 7);

        for (int j = 0; j < n; j++)
            for (int i = 0; i < n; i++)
                if(i != j)  g.InsertArc(j, i, m[j][i]);

        while (c != 'a')	{
            cout << endl << "1. 图清空.";
            cout << endl << "2. 显示图.";
            cout << endl << "3. 删除顶点.";
            cout << endl << "4. 插入顶点.";
            cout << endl << "5. 删除边.";
            cout << endl << "6. 插入边.";
            cout << endl << "7. 求顶点的出度.";
            cout << endl << "8. 求顶点的入度.";
            cout << endl << "9. 是否有环.";
            cout << endl << "a. 退出";

            cout << endl << "选择功能(1~7):";
            cin >> c;
            switch (c) 		{
                case '1':
                    g.Clear();
                    break;
                case '2':
                    if (g.IsEmpty())
                        cout << "该图为空。" << endl;
                    else
                        g.Display();
                    break;
                case '3':
                    cout << endl << "输入被删除顶点的值:";
                    cin >> e;
                    g.DeleteVex(e);
                    break;
                case '4':
                    cout << endl << "输入插入顶点的值:";
                    cin >> e;
                    g.InsertVex(e);
                    break;
                case '5':
                    cout << endl << "输入与被删除边关联的两个顶点值:";
                    cin >> e1 >> e2;
                    v1 = g.GetOrder(e1);
                    v2 = g.GetOrder(e2);
                    g.DeleteArc(v1, v2);
                    break;
                case '6':
                    cout << endl << "输入与插入边关联的两个顶点值和权重:";
                    cin >> e1 >> e2 >> w;
                    v1 = g.GetOrder(e1);
                    v2 = g.GetOrder(e2);
                    g.InsertArc(v1, v2, w);
                    break;
                case '7':
                    cout << endl << "输入求出度的顶点的值:";
                    cin >> e;
                    v = g.GetOrder(e);
                    cout<<g.CountOutDegree(v)<<endl;
                    break;
                case '8':
                    cout << endl << "输入求入度的顶点的值:";
                    cin >> e;
                    v = g.GetOrder(e);
                    cout<<g.CountInDegree(v)<<endl;
                    break;
                case '9':
                    if(g.hasCycle())
                        cout << endl << "有环";
                    else
                        cout << endl << "没环";
                    break;
            }
        }
    }
    catch (Error err)					// 捕捉并处理异常
    {
        err.Show();						// 显示异常信息
    }

    system("PAUSE");					// 调用库函数system()
    return 0;
}