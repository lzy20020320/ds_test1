#include "AdjListDirNetwork.h"		// �ڽӾ�������ͼ
#include "windows.h"
int main(void)
{
    std::vector<int> output;
    try									// ��try��װ���ܳ����쳣�Ĵ���
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
        cout<< "A���������̾���"<<endl;
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
            cout << endl << "1. ͼ���.";
            cout << endl << "2. ��ʾͼ.";
            cout << endl << "3. ɾ������.";
            cout << endl << "4. ���붥��.";
            cout << endl << "5. ɾ����.";
            cout << endl << "6. �����.";
            cout << endl << "7. �󶥵�ĳ���.";
            cout << endl << "8. �󶥵�����.";
            cout << endl << "9. �Ƿ��л�.";
            cout << endl << "0. ��ζ�·��";
            cout << endl << "a. �˳�";

            cout << endl << "ѡ����(1~7):";
            cin >> c;
            switch (c) 		{
                case '1':
                    g.Clear();
                    break;
                case '2':
                    if (g.IsEmpty())
                        cout << "��ͼΪ�ա�" << endl;
                    else
                        g.Display();
                    break;
                case '3':
                    cout << endl << "���뱻ɾ�������ֵ:";
                    cin >> e;
                    g.DeleteVex(e);
                    break;
                case '4':
                    cout << endl << "������붥���ֵ:";
                    cin >> e;
                    g.InsertVex(e);
                    break;
                case '5':
                    cout << endl << "�����뱻ɾ���߹�������������ֵ:";
                    cin >> e1 >> e2;
                    v1 = g.GetOrder(e1);
                    v2 = g.GetOrder(e2);
                    g.DeleteArc(v1, v2);
                    break;
                case '6':
                    cout << endl << "���������߹�������������ֵ��Ȩ��:";
                    cin >> e1 >> e2 >> w;
                    v1 = g.GetOrder(e1);
                    v2 = g.GetOrder(e2);
                    g.InsertArc(v1, v2, w);
                    break;
                case '7':
                    cout << endl << "��������ȵĶ����ֵ:";
                    cin >> e;
                    v = g.GetOrder(e);
                    cout<<g.CountOutDegree(v)<<endl;
                    break;
                case '8':
                    cout << endl << "��������ȵĶ����ֵ:";
                    cin >> e;
                    v = g.GetOrder(e);
                    cout<<g.CountInDegree(v)<<endl;
                    break;
                case '9':
                    std::cout << "����������·: ";
                    if(g.hasCycle())
                        cout<<"�л�";
                    else
                        cout<<"û��";
                    std::cout << '\n';
                    // -------------------------------------------------------------------
                    std::cout << "DFS���·: ";
                    g.ClearTag();
                    output.emplace_back(0);
                    if(g.CycleDFS(0, output))
                    {
                        cout<<"�л� ��Ϊ�� ";
                        std::cout << vexs[output[output.size()-1]];
                        for(int i = output.size() -2 ; i >= 1; i--)
                            std::cout  << " -> "<< vexs[output[i]];
                        output.clear();
                    }
                    else
                        cout<<"û��";
                    std::cout << '\n';
                    // -------------------------------------------------------------------
                    std::cout << "�����������·: ";
                    if(g.Cycle())
                        cout<<"�л�";
                    else
                        cout<<"û��";
                    std::cout << '\n';
                    break;
                case '0':{
                    char v1,v2;
                    cout<<"�����������յ�:";
                    bool flag(true);
                    while(flag){
                        cin>>v1>>v2;
                        if(cin.fail()){
                            cin.clear();//�ѿɻָ��������ָ�
                            cin.ignore();//ע�����������ԭ����istream& ignore(streamsize n=1;int delim=EOF)
                            //��ȡǰn���ַ�����������delim�ַ���ֹͣ���Ѷ�ȡ�Ķ�������
                            //�����൱�ڰѻ��з�ȥ��
                            cout<<"����������������룺";
                        }else
                            flag=false;
                    }
                    g.SecShortestPath(g.GetOrder(v1),g.GetOrder(v2));
                }
            }
        }
    }
    catch (Error err)					// ��׽�������쳣
    {
        err.Show();						// ��ʾ�쳣��Ϣ
    }

    //system("PAUSE");					// ���ÿ⺯��system()
    return 0;
}