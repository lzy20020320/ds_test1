#include "AdjListDirNetwork.h"		// �ڽӾ�������ͼ

int main(void)
{
    try									// ��try��װ���ܳ����쳣�Ĵ���
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
            cout << endl << "1. ͼ���.";
            cout << endl << "2. ��ʾͼ.";
            cout << endl << "3. ɾ������.";
            cout << endl << "4. ���붥��.";
            cout << endl << "5. ɾ����.";
            cout << endl << "6. �����.";
            cout << endl << "7. �󶥵�ĳ���.";
            cout << endl << "8. �󶥵�����.";
            cout << endl << "9. �Ƿ��л�.";
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
                    if(g.hasCycle())
                        cout << endl << "�л�";
                    else
                        cout << endl << "û��";
                    break;
            }
        }
    }
    catch (Error err)					// ��׽�������쳣
    {
        err.Show();						// ��ʾ�쳣��Ϣ
    }

    system("PAUSE");					// ���ÿ⺯��system()
    return 0;
}