#include <iostream>
#include <fstream>
#include <vector>
#include <map>


using namespace std;

///обработка файла (матрицы)

void beolvas_matrix(vector<vector<int>>& matrix, string fajlnev)
{
    ifstream befajl(fajlnev);

    if (!befajl.good())
    {
        cout << "Nem sikerült a beolvasás." << endl;
        return;
    }

    int meret;
    int elem;

    befajl >> meret;

    for (unsigned i=0; i<meret; i++)
    {
        vector<int> sor;

        for (unsigned j=0; j<meret; j++)
        {
            befajl >> elem;
            sor.push_back(elem);
        }

        matrix.push_back(sor);
    }

    //ellenorzes
    /*
    for (x:matrix)
    {
        for(int i  = 0; i < meret; i++)
        cout << x[i] << " ";
        cout << endl;
    }
    */
}

///создаем граф из обработанного файла (матрицы)

void graf_matrixbol(map<int, map<int, int>>& graf, string fajlnev)
{
    vector<vector<int>> matrix;

    beolvas_matrix(matrix, fajlnev);

    for (unsigned i=0; i<matrix.size(); i++)
        graf.insert(make_pair(i, map<int, int>()));

    for (unsigned i=0; i<matrix.size(); i++)
    {
        for (unsigned j=0; j<matrix[i].size(); j++)
        {
            if (matrix[i][j] != 0)
            graf[i].insert(make_pair(j, matrix[i][j]));
        }
    }
}

int main()
{
    vector<vector<int>> matrix;
    beolvas_matrix(matrix, "vasut_matrix.txt");

    map<int, map<int, int>> graf;
    graf_matrixbol(graf, "vasut_matrix.txt");

    return 0;
}
