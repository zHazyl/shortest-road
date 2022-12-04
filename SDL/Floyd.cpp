#include "CommonFunction.h"
//int main()
//{
//    int** D;
//    int** S;
//    int n; // So dinh cua do thi
//    int** G; // Ma tran trong so cua do thi
//    int s, t; // Dinh dau va dinh cuoi cua do thi
//    cin >> s >> t;
//    NhapDoThi(n, G);
//    Floyd(n, G);
//    return 0;
//}
void NhapDoThi(int& n, int**& G, int name)
{
    
    ifstream Input;
    if(name != 0)  
        Input.open("time.txt", ios::in);
    else 
        Input.open("distance.txt", ios::in);
    G = new int* [n];
    for (int i = 1; i <= n; i++)
    {
        G[i] = new int[n];
        for (int j = 1; j <= n; j++)
        {
            Input >> G[i][j];
            if (G[i][j] == 0)
                G[i][j] = VOCUNG;
        }
    }
}
void Floyd(int n, int** G, int** D, int** S)
{
    int i, j, k;
    for (i = 1; i <= n; i++)
    {
        S[i] = new int[n];
        D[i] = new int[n];
        for (j = 1; j <= n; j++)
        {
            D[i][j] = G[i][j];
            if (D[i][j] == VOCUNG)
                S[i][j] = 0;
            else
                S[i][j] = j;
        }
    }
    for (k = n; k >= 1; k--)
        for (i = n; i >= 1; i--)
            for (j = n; j >= 1; j--)
                if (D[i][k] != VOCUNG && D[i][j] > (D[i][k] + D[k][j]))
                {
                    D[i][j] = D[i][k] + D[k][j];
                    S[i][j] = S[i][k];
                }
}
