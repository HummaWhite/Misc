# L

斜率优化dp

转移方程$dp_i = \min(dp_j+(sum_i-sum_j+i-j-1-L)^2)$其中$sum_i$为前缀和。令$a_i = sum_i + i$，则化为$dp_i = \min(dp_j + (a_i - a_j - 1 - L)^2)$

设$dp_i$从$dp_x$ 、$dp_y$为转移来，$dp_x$比$dp_y$更优且$x > y$，有

$dp_x + (a_i - a_x - 1 - L)^2 < dp_y + (a_i - a_y - 1 - L)^2$

一系列移项等操作后，得到

$a_i > \frac{dp_x + 2(L+1)a_x + a_x^2 - dp_y - 2(L+1)a_y - a_y^2}{a_x - a_y}$

这里就是斜率了，那么我们可以直接根据斜率判断那个状态更优。由于$a_i$是递增的，用单调队列维护斜率增大的序列（下凸）。