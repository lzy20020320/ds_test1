# ds_test1
—、目的和要求

1. 掌握图的逻辑结构定义和邻接矩阵储结构的实现。
2. 掌握图的基本操作算法的实现。

3. 根据实际问题的需要，设计算法解决问题。

二、实验内容

模仿无向图的邻接矩阵类模板，完成（带权：非负）有向网的邻接矩阵类模板的设计与实现。要求 实现图的基本运算（如增加删除顶点和弧等），并增加如下成员函数：

1. CountOutDegree(v)，统计顶点 v的出度；

2. CountInDegree(v)，统计顶点 v的入度；

3. SecShortestPath(v1,v2)，求两个顶点之间的次短路径；

4. 判断有向图是否存在环。
   目前采用两种算法，hasCycle() and Cycle(). 算法性能有待测试。
