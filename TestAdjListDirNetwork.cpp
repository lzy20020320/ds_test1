#include "AdjListDirNetwork.h"		// 邻接矩阵有向图
#include "windows.h"
int main(void)
{
    std::vector<int> output;
    try									// 用try封装可能出现异常的代码
    {
        char vexs[] = {'A', 'B', 'C', 'D', 'E'};
        int m[5][5] = {
                {0               , 5               , 5               , DEFAULT_INFINITY, 10              },
                {DEFAULT_INFINITY, 0               , 100             , DEFAULT_INFINITY, 8               },
                {DEFAULT_INFINITY, DEFAULT_INFINITY, 0               , 4               , 5               },
                {DEFAULT_INFINITY, DEFAULT_INFINITY, DEFAULT_INFINITY, 0               , 2               },
                {DEFAULT_INFINITY, 1               , DEFAULT_INFINITY, DEFAULT_INFINITY, 0               },
        };
        char c = '0', e, e1, e2;
        int n = 5, v, v1, v2,w;

        AdjMatrixdirNetwork<char> g(vexs, n, 7);

        for (int j = 0; j < n; j++)
            for (int i = 0; i < n; i++)
                if(i != j)  g.InsertArc(j, i, m[j][i]);

        vector<int> pre_points,distance,sec,pre_points_sec;
        cout<< "A到各点的最短距离"<<endl;
        g.DijkstraSec(0,pre_points,distance,sec,pre_points_sec);
        for(auto &point:pre_points)
            cout<<point<<'\t';
        cout<<endl;
        for(auto &dis:distance)
            if(dis==DEFAULT_INFINITY)
                cout<<"INFINITY"<<'\t';
            else
                cout<<dis<<'\t';
        cout<<endl;
        for(auto &point:pre_points_sec)
            cout<<point<<'\t';
        cout<<endl;
        for(auto &dis:sec)
            if(dis==DEFAULT_INFINITY)
                cout<<"INFINITY"<<'\t';
            else
                cout<<dis<<'\t';

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
            cout << endl << "0. 求次短路径";
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
                    std::cout << "广度优先求回路: ";
                    if(g.hasCycle())
                        cout<<"有环";
                    else
                        cout<<"没环";
                    std::cout << '\n';
                    // -------------------------------------------------------------------
                    std::cout << "DFS求回路: ";
                    g.ClearTag();
                    output.emplace_back(0);
                    if(g.CycleDFS(0, output))
                    {
                        cout<<"有环 环为： ";
                        std::cout << vexs[output[output.size()-1]];
                        for(int i = output.size() -2 ; i >= 1; i--)
                            std::cout  << " -> "<< vexs[output[i]];
                        output.clear();
                    }
                    else
                        cout<<"没环";
                    std::cout << '\n';
                    // -------------------------------------------------------------------
                    std::cout << "拓扑排序求回路: ";
                    if(g.Cycle())
                        cout<<"有环";
                    else
                        cout<<"没环";
                    std::cout << '\n';
                    break;
                case '0':{
                    char v1,v2;
                    cout<<"请输入起点和终点:";
                    bool flag(true);
                    while(flag){
                        cin>>v1>>v2;
                        if(cin.fail()){
                            cin.clear();//把可恢复的流都恢复
                            cin.ignore();//注意这个函数的原型是istream& ignore(streamsize n=1;int delim=EOF)
                            //读取前n个字符或者在遇到delim字符就停止，把读取的东西丢掉
                            //这里相当于把换行符去掉
                            cout<<"输入错误请重新输入：";
                        }else
                            flag=false;
                    }
                    g.SecShortestPath(g.GetOrder(v1),g.GetOrder(v2));
                }
            }
        }
    }
    catch (Error err)					// 捕捉并处理异常
    {
        err.Show();						// 显示异常信息
    }

    //system("PAUSE");					// 调用库函数system()
    return 0;
}