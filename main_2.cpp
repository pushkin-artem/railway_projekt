/*

Menetrend: Egy A-B-C útvonal:

V1 2 //vonat nev es hogy hany kocsit bir el  // название поезда, и сколько вагонов можно прицеплять
A 1
B 2
C 4
B 6
A 7

Kocsik:

a 10 A   //kocsi nev, kapacitasa, es hol van  // название вагона, его вместимость и его место нахождение
b 15 B

Áruk:

cucc1 A C 35  //csomag nev, indulo es celhelye, hany db
cucc2 B C 15


Kimeneti akcióterv:

0 pakol a cucc1 10
// van egy óránk pakolni, mert csak 1-kor indul a vonat
0 pakol b cucc2 15
1 csatol a V1
2 csatol b V1
4 lecsatol a V1
4 lecsatol b V1
5 kipakol a cucc1 10
5 kipakol b cucc2 15
// ebben a pillanatban már a cucc2 teljesen megérkezett, a cucc1-ből még van 25 csomag A-ban, de elment a vonat, ezért meg kell várni a csatolással mire újra itt jár, mert az üres kocsikat vissza kell vinni hogy feltölthessük
11 csatol a V1
// 11 = 7+4, ilyenkor van másodszor C-ben V1 vonat
11 csatol b V1
14 lecsatol a V1
14 lecsatol b V1
14 pakol a cucc1 10
14 pakol b cucc1 15
15 csatol a V1
15 csatol b V1
18 lecsatol a V1
18 lecsatol b V1
19 kipakol a cucc1 10
19 kipakol b cucc1 15


Adott órában kocsi töltése áruval (csak nem csatlakoztatott kocsit szabad tölteni)
Adott órában kocsi csatlakoztatása vonathoz (nem lehet ugyanazon órában tölteni és csatlakoztatni)
Adott órában kocsi leválasztása kocsiról (egy órában egy kocsi leválasztása és csatlakoztatása átcsatolást jelent egy másik vonatra, szabad lecsatolni és pakolni ugyanazon órában)
Adott órában kocsi kipakolása (csak nem csatlakoztatott kocsit szabad kipakolni)

Nem szabad 1 oraban:
            - lecsatolni es kipakolni
            - csatolni es pakolni

Tehat 1 ora alatt vegzunk a kovetkezoket:
            - kocsi csatolas (csak nem csatolt kocsit szabad pakolni)
            - kocsi lecsatolasa
            - kocsi pakolas (lehet lecsatolni es pakolni egy oraban)
            - kocsi kipakolas (csak nem csatolt kocsit szabad kipakolni)

            - egy órában egy kocsi lecsatolasa és csatolasa átcsatolást jelent egy másik vonatra
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdlib>
#include <cmath>

using namespace std;

struct Vonat
{
    string vonat_nev;
    int vonat_biras;
};

struct Menetrend
{
    string allomas_nev;
    int allomas_ora;
};

struct Kocsi
{
    string kocsi_nev;
    int kocsi_kapacitas;
    string kocsi_helye;
};

struct Csomag
{
    string csomag_nev;
    string csomag_kezdohelye, csomag_celhelye;
    int csomag_mennyiseg;
};

class Allomas
{
public:

    Vonat vonat;
    Menetrend menetrend;
    Kocsi kocsi;
    Csomag csomag;

    Allomas(Vonat vonat1){vonat = vonat1;}
};



void beolvasas(string fajlnev, Vonat & v, vector<Menetrend>& allomasok, vector<Kocsi>& kocsik, vector<Csomag>& csomagok)
{

    ifstream befajl(fajlnev);

    if (!befajl.good())
    {
        cout << "Nem sikerült a beolvasás." << endl;
        return;
    }

    //Vonat v;
    Menetrend a;
    Kocsi k;
    Csomag c;

    string tmp;

    if(fajlnev == "vonat.txt")
    {
        while(befajl.good())
        {
            getline(befajl, v.vonat_nev, ' ');
            getline(befajl, tmp);
            v.vonat_biras = atoi(tmp.c_str());
        }
        cout << v.vonat_nev << " " << v.vonat_biras << endl;
    }

    else if(fajlnev == "menetrend.txt")
    {
        while(befajl.good())
        {
            getline(befajl, a.allomas_nev, ' ');
            getline(befajl, tmp);
            a.allomas_ora = atoi(tmp.c_str());

            allomasok.push_back(a);
        }

        for (x:allomasok)
        {
            cout << x.allomas_nev << " " << x.allomas_ora << endl;
        }
    }

    else if(fajlnev == "kocsik.txt")
    {
        while(befajl.good())
        {
            getline(befajl, k.kocsi_nev, ' ');
            getline(befajl, tmp, ' ');
            k.kocsi_kapacitas = atoi(tmp.c_str());
            getline(befajl, k.kocsi_helye);

            kocsik.push_back(k);
        }

        for (x:kocsik)
        {
            cout << x.kocsi_nev << " " << x.kocsi_kapacitas << " " << x.kocsi_helye << endl;
        }
    }

    else if(fajlnev == "csomagok.txt")
    {
        while(befajl.good())
        {
            getline(befajl, c.csomag_nev, ' ');
            getline(befajl, c.csomag_kezdohelye, ' ');
            getline(befajl, c.csomag_celhelye, ' ');
            getline(befajl, tmp);
            c.csomag_mennyiseg = atoi(tmp.c_str());

            csomagok.push_back(c);
        }

        for (x:csomagok)
        {
            cout << x.csomag_nev << " " << x.csomag_kezdohelye << " " << x.csomag_celhelye <<  " " << x.csomag_mennyiseg << endl;
        }
    }

}


int main()
{
    vector<Menetrend> allomasok;
    vector<Kocsi> kocsik;
    vector<Csomag> csomagok;

    Vonat v;

    beolvasas("vonat.txt", v, allomasok, kocsik, csomagok);
    beolvasas("menetrend.txt", v, allomasok, kocsik, csomagok);
    beolvasas("kocsik.txt", v, allomasok, kocsik, csomagok);
    beolvasas("csomagok.txt", v, allomasok, kocsik, csomagok);

    cout << v.vonat_nev << endl;

    Allomas x(v);
    cout << x.vonat.vonat_nev << " " << x.vonat.vonat_biras << endl;

    return 0;
}
