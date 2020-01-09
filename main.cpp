#include <iostream>
#include <fstream>
#include <vector>
#include <map>


using namespace std;


///utvonal grafjanak szomsedsagi matrix beolvasasa
//minden megallo kozott kb 1 ora az ut, tehat ezt is mutatja a szomszedasgi matrix

struct Ut_adatai
{
    int kapcsoltsag, ateresztokepesseg, aramlas;
};

void beolvas_matrix(vector<vector<int>>& matrix, string fajlnev, int & dimenzioszam)
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

    dimenzioszam = meret;

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

    cout << "Szomszedasagi matrix:" << endl;

    for (x:matrix)
    {
        for(int i  = 0; i < meret; i++)
        cout << x[i] << " ";
        cout << endl;
    }
    cout << endl;
}

///itt hozom letre a graf eleinek az atereszhetokepesseget a kocsik kapacitasanak megfeleloen

void kapacitasi_matrix_letrehozasa(vector<vector<int>>& matrix1, vector<vector<int>>& matrix2, int dimenzio_szam)
{
    //kapacitasi matrix letrehozasa, csak ott lehetnek sulyok az eleknek, ahol vannak kapcsolatok csucsok kozott,
    //tehat ha minden allomason csak 1 kocsi van, amelynek kapacitasa 1 csomag, ez megegyezik a szomsedsagi matrixszal

    matrix1 = matrix2;

    //legyen a kovetkezo a pelda:

    //A,1 (tehat A->B vonalon az el maximalis kapacitasa 1)
    //B,1 (tehat B->C vonalon az el maximalis kapacitasa 1)
    //C-n nincs kocsi (tehat C->D vonalon az el maximalis kapacitasa 0)
    //D,2 (tehat B->C vonalon az el maximalis kapacitasa 1)

    //most ugy oldottam meg a feladatot, hogy visszafele a pontokon nincsenek kocsik, tehat nincsenek csomagok sem

    matrix1[2][3] = 0;
    matrix1[3][2] = 0;

    matrix1[3][4] = 2;
    matrix1[4][3] = 2;

    cout << "Kapacitasi matrix:" << endl;

    //ellenorzes:

    for (x:matrix1)
    {
        for(int i  = 0; i < dimenzio_szam; i++)
        cout << x[i] << " ";
        cout << endl;
    }
    cout << endl;

}


///itt hozom letre a graf eleinek az aramlasat a csomagok mennyisegenek megfeleloen

void aramlasi_matrix_letrehozasa(vector<vector<int>>& matrix1, vector<vector<int>>& matrix2, int dimenzio_szam)
{
    //aramlasi matrix letrehozasa, jelzi, hogy mennyi csomag erkezik az egyik ponttol a masikba

    matrix1 = matrix2;

    //legyen a kovetkezo a pelda:

    //A,1 és 1 db csomag A->B
    //B,1 és 1 db csomag B->C
    //C-n nincs kocsi és 0 db csomag C->D
    //D,2 és 2 db csomag D->E
    //visszafele nincsenek csomagok

    matrix1[2][3] = 0;
    matrix1[3][2] = 0;

    matrix1[3][4] = 2;
    matrix1[4][3] = 2;

    cout << "Aramlasi matrix:" << endl;

    //ellenorzes:

    for (x:matrix1)
    {
        for(int i  = 0; i < dimenzio_szam; i++)
        cout << x[i] << " ";
        cout << endl;
    }
    cout << endl;

}

void graf_matrixbol(map<int, map<int, Ut_adatai>>& graf, vector<vector<int>>& matrix1, vector<vector<int>>& matrix2, vector<vector<int>>& matrix3)
{
    Ut_adatai ut;

    for (unsigned i=0; i<matrix1.size(); i++)
        graf.insert(make_pair(i, map<int, Ut_adatai>()));

    for (unsigned i=0; i<matrix1.size(); i++)
    {
        for (unsigned j=0; j<matrix1[i].size(); j++)
        {
            if (matrix1[i][j] != 0)
            {
                ut.kapcsoltsag = matrix1[i][j];
                ut.ateresztokepesseg = matrix2[i][j];
                ut.aramlas = matrix3[i][j];
                graf[i].insert(make_pair(j, ut));
            }
        }
    }

    //ellenorzes:

    for (map<int, map<int, Ut_adatai>>::iterator it1=graf.begin(); it1!=graf.end(); ++it1)
    {
        cout << "sorszam:" << it1->first << " ";
        for (map<int, Ut_adatai>::iterator it2=it1->second.begin(); it2!=it1->second.end(); ++it2)
            cout << "oszlopszam:" <<  it2->first << "kapcsoltsag :" << it2->second.kapcsoltsag << ", ateresztokepesseg :" << it2->second.ateresztokepesseg << ", aramlas :" << it2->second.aramlas << endl;
    }
}

int main()
{
    int dimenzio_szam = 0;
    vector<vector<int>> szomszedsagi_matrix;
    beolvas_matrix(szomszedsagi_matrix, "vasut_matrix.txt", dimenzio_szam);

    vector<vector<int>> kapacitasi_matrix; //atengedő képesség
    kapacitasi_matrix_letrehozasa(kapacitasi_matrix, szomszedsagi_matrix, dimenzio_szam);

    vector<vector<int>> aramlasi_matrix; //csomagok mennyisege
    aramlasi_matrix_letrehozasa(aramlasi_matrix, szomszedsagi_matrix, dimenzio_szam);

    map<int, map<int, Ut_adatai>> graf;
    graf_matrixbol(graf, szomszedsagi_matrix, kapacitasi_matrix, aramlasi_matrix);

    //szelessegi_bejaras( );

    return 0;
}
