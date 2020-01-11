#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>

using namespace std;


///utvonal grafjanak szomsedsagi matrix beolvasasa

struct Ut_adatai
{
    int kapcsoltsag, ido, ateresztokepesseg, aramlas;
};

struct Ut
{
    int suly; //az útvonal összsúlya (összideje)
    vector<int> ut; //az útvonal által érintett csúcsok (az érintés sorrendjében)
    Ut(int suly_){suly = suly_;} //konstruktor függvény
};

bool operator<(Ut a, Ut b)
{
    return a.suly > b.suly;
}

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

    cout << "Szomszedasagi matrix:" << endl;

    for (x:matrix)
    {
        for(int i  = 0; i < meret; i++)
        cout << x[i] << " ";
        cout << endl;
    }
    cout << endl;
}

///itt hozom letre az idobeli tartomanyokat megallok kozul matrixos alakban

void idobeli_matrix_letrehozasa(vector<vector<int>>& matrix1, vector<vector<int>>& matrix2)
{
    matrix1 = matrix2;

    //legyen minden megallo kozul 2 ora az ut

    for(int i = 0; i < matrix1.size(); i++)
    {
        for(int j = 0; j < matrix1[i].size(); j++)
        {
            if(matrix1[i][j] != 0)
                matrix1[i][j] = 2;
        }
    }

    cout << "Idobeli matrix:" << endl;

    //ellenorzes:

    for (x:matrix1)
    {
        for(int i  = 0; i < matrix1.size(); i++)
        cout << x[i] << " ";
        cout << endl;
    }
    cout << endl;


}

///itt hozom letre a graf eleinek az atereszhetokepesseget a kocsik kapacitasanak megfeleloen

void kapacitasi_matrix_letrehozasa(vector<vector<int>>& matrix1, vector<vector<int>>& matrix2)
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
        for(int i  = 0; i < matrix1.size(); i++)
        cout << x[i] << " ";
        cout << endl;
    }
    cout << endl;

}

///itt hozom letre a graf eleinek az aramlasat a csomagok mennyisegenek megfeleloen

void aramlasi_matrix_letrehozasa(vector<vector<int>>& matrix1, vector<vector<int>>& matrix2)
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

    matrix1[3][4] = 4;
    matrix1[4][3] = 4;

    cout << "Aramlasi matrix:" << endl;

    //ellenorzes:

    for (x:matrix1)
    {
        for(int i  = 0; i < matrix1.size(); i++)
        cout << x[i] << " ";
        cout << endl;
    }
    cout << endl;

}

void graf_matrixbol(map<int, map<int, Ut_adatai>>& graf, vector<vector<int>>& matrix1, vector<vector<int>>& matrix2, vector<vector<int>>& matrix3, vector<vector<int>>& matrix4)
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
                ut.ido = matrix2[i][j];
                ut.ateresztokepesseg = matrix3[i][j];
                ut.aramlas = matrix4[i][j];
                graf[i].insert(make_pair(j, ut));
            }
        }
    }

    //ellenorzes:

    for (map<int, map<int, Ut_adatai>>::iterator it1=graf.begin(); it1!=graf.end(); ++it1)
    {
        for (map<int, Ut_adatai>::iterator it2=it1->second.begin(); it2!=it1->second.end(); ++it2)
            cout << "sorszam: " << it1->first << ", oszlopszam: " <<  it2->first << ", kapcsoltsag: " << it2->second.kapcsoltsag << ", ido: " << it2->second.ido << ", ateresztokepesseg: " << it2->second.ateresztokepesseg << ", aramlas: " << it2->second.aramlas << endl;
    }
    cout << endl;
}

Ut szelessegi_bejaras(int kezdo, int cel, map<int, map<int, Ut_adatai>> graf)
{
    priority_queue<Ut> sor;

    Ut aktualis(0);
    aktualis.ut.push_back(kezdo);

    sor.push(aktualis); //hozzáadjuk az elemet a sorhoz

    while (!sor.empty())
    {
        /*a ciklusfeltétel akkor nem teljesül, ha bejártuk a teljes gráfot
        anélkül, hogy elértük volna a célt, ami azt jelenti, hogy nincs
        útvonal a kezdőpontból a célba a gráfban. */

        aktualis = sor.top(); //lekérjük a legkisebb súlyú útvonalat a sorból
        sor.pop(); //Töröljük a legkisebb súlyú útvonalat

        int utolso = aktualis.ut.back();

        if (utolso == cel) //elértük a célt, biztosan optimális súllyal
            return aktualis;

        for (auto szomszed: graf[utolso])
        {
            //végigmegyünk utolso szomszédain
            Ut gyerek = aktualis;
            gyerek.ut.push_back(szomszed.first); //az útvonalhoz hozzáadjuk a szomszédot

            gyerek.suly += szomszed.second.ido; //megnöveljük az összsúlyt a legutóbb hozzáadott él súlyával

            sor.push(gyerek); //hozzáadjuk az újonnan képzett útvonalat a sorhoz
        }
    }
}

void szabalyok(map<int, map<int, Ut_adatai>> graf)
{
    int legrovidebb_ut_ideje = 0;

    for (map<int, map<int, Ut_adatai>>::iterator it1=graf.begin(); it1!=graf.end(); ++it1)
    {
        for (map<int, Ut_adatai>::iterator it2=it1->second.begin(); it2!=it1->second.end(); ++it2)
        {
            if(it2->second.ateresztokepesseg >= it2->second.aramlas)
            {
                Ut legjobb = szelessegi_bejaras(0, 4, graf);
                legrovidebb_ut_ideje = legjobb.suly;

            }
            else
            {
                cout << "(" << it1->first << ";" << it2->first << ")" << " ponton a kocsik szama: " << it2->second.ateresztokepesseg << ", de a csomagok szama: " << it2->second.aramlas << endl;
                legrovidebb_ut_ideje = 0;
            }
        }
    }

    if(legrovidebb_ut_ideje > 0)
        cout <<"Legrovidebb utvonal ideje : " << legrovidebb_ut_ideje << " ora" << endl;
    else
        cout <<"A szallitas nem lehetseges" << endl;

}

int main()
{
    vector<vector<int>> szomszedsagi_matrix;
    beolvas_matrix(szomszedsagi_matrix, "vasut_matrix.txt");

    vector<vector<int>> idobeli_matrix;
    idobeli_matrix_letrehozasa(idobeli_matrix, szomszedsagi_matrix);

    vector<vector<int>> kapacitasi_matrix; //atengedő képesség
    kapacitasi_matrix_letrehozasa(kapacitasi_matrix, szomszedsagi_matrix);

    vector<vector<int>> aramlasi_matrix; //csomagok mennyisege
    aramlasi_matrix_letrehozasa(aramlasi_matrix, szomszedsagi_matrix);

    map<int, map<int, Ut_adatai>> graf;
    graf_matrixbol(graf, szomszedsagi_matrix, idobeli_matrix, kapacitasi_matrix, aramlasi_matrix);

    szabalyok(graf);

    return 0;
}
