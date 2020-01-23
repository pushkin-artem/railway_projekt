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
#include <set>
#include <cstdlib>
#include <cmath>

using namespace std;

struct Kocsi
{
    string kocsi_nev;
    int kocsi_kapacitas = 0;
    int benne = 0;
    string kocsi_helye = "x";
    bool kocsi_szabad = true;
};

struct Vonat
{
    string vonat_nev="vvv";
    int vonat_biras = 0;
    Kocsi kocsi;
};

struct Csomag
{
    string csomag_nev;
    string csomag_kezdohelye = "x", csomag_celhelye="x";
    int csomag_mennyiseg = 0;
};

class Allomas
{
public:

    Vonat vonat;                //один поезд
    string allomas_nev="";
    int allomas_ora;           //расписание одного поезда!!!!!
    string allomas_vonat = "";
    Kocsi kocsi;
    vector<Kocsi> kocsik;
    Csomag csomag;
};


void beolvasas(string fajlnev, Allomas& a, vector<Allomas>& allomasok)
{
    ifstream befajl(fajlnev);

    if (!befajl.good())
    {
        cout << "Nem sikerült a beolvasás." << endl;
        return;
    }

    Kocsi k;
    string tmp;

    if(fajlnev == "vonat.txt")
    {
        while(befajl.good())
        {
            getline(befajl, a.vonat.vonat_nev, ' ');
            getline(befajl, tmp);
            a.vonat.vonat_biras = atoi(tmp.c_str());
        }
        //cout << v.vonat_nev << " " << v.vonat_biras << endl;
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
    }

    else if(fajlnev == "kocsik.txt")
    {
        while(befajl.good())
        {
            getline(befajl, a.kocsi.kocsi_nev, ' ');
            getline(befajl, tmp, ' ');
            a.kocsi.kocsi_kapacitas = atoi(tmp.c_str());
            getline(befajl, a.kocsi.kocsi_helye);

            for(int i = 0; i < allomasok.size(); i++)
            {
                if(a.kocsi.kocsi_helye == allomasok[i].allomas_nev)
                {
                    allomasok[i].kocsik.push_back(a.kocsi);
                    /*
                    allomasok[i].kocsi.kocsi_nev = a.kocsi.kocsi_nev;
                    allomasok[i].kocsi.kocsi_kapacitas = a.kocsi.kocsi_kapacitas;
                    allomasok[i].kocsi.kocsi_helye = a.kocsi.kocsi_helye;
                    */
                }
            }
        }

    }

    else if(fajlnev == "csomagok.txt")
    {
        while(befajl.good())
        {
            getline(befajl, a.csomag.csomag_nev, ' ');
            getline(befajl, a.csomag.csomag_kezdohelye, ' ');
            getline(befajl, a.csomag.csomag_celhelye, ' ');
            getline(befajl, tmp);
            a.csomag.csomag_mennyiseg = atoi(tmp.c_str());

            map<string,int> check;
            for(int i = 0; i < allomasok.size(); i++)
            {
                ++check[allomasok[i].allomas_nev];
                if(a.csomag.csomag_kezdohelye == allomasok[i].allomas_nev && check[allomasok[i].allomas_nev] == 1)
                {
                    allomasok[i].csomag.csomag_nev = a.csomag.csomag_nev;
                    allomasok[i].csomag.csomag_kezdohelye = a.csomag.csomag_kezdohelye;
                    allomasok[i].csomag.csomag_celhelye = a.csomag.csomag_celhelye;
                    allomasok[i].csomag.csomag_mennyiseg = a.csomag.csomag_mennyiseg;
                }
                 if(a.csomag.csomag_celhelye == allomasok[i].allomas_nev)
                {
                    allomasok[i].csomag.csomag_nev = a.csomag.csomag_nev;
                    //allomasok[i].csomag.csomag_kezdohelye = a.csomag.csomag_kezdohelye;
                    allomasok[i].csomag.csomag_celhelye = a.csomag.csomag_celhelye;
                   // allomasok[i].csomag.csomag_mennyiseg = a.csomag.csomag_mennyiseg;
                }


            }
        }
    }
}


void szabalyok(vector<Allomas> allomasok)
{
    for(auto x : allomasok)
    {
        cout << "Allomas nev: " << x.allomas_nev << endl;
        cout << "Ora: " << x.allomas_ora << endl;
        cout << "Kocsik adatai: ";
        for(auto y : x.kocsik)
            cout << y.kocsi_nev << " " << y.kocsi_kapacitas << " " << y.kocsi_helye << ", ";
        cout << endl;
        //cout << "Kocsi adatai: " << x.kocsi.kocsi_nev << " " << x.kocsi.kocsi_kapacitas << " " << x.kocsi.kocsi_helye << endl;
        cout << "Csomag adatai: " << x.csomag.csomag_nev << " " << x.csomag.csomag_kezdohelye << " " << x.csomag.csomag_celhelye << " " << x.csomag.csomag_mennyiseg << endl;
        cout << endl;
    }
    cout << endl;


    map <string,int> shipment;
    int ido = 0;
    int timer = 0;

    while(true)
    {

        cout<< "Time is: "<< timer << endl;

        //Первый шаг - проверка всех станций в момент времени 0
        for(auto & x : allomasok)
        {

            //Грузы и вагоны на станциях
                for(unsigned i = 0; i < x.kocsik.size(); i++)
                {
                    if(x.kocsik[i].kocsi_helye != "x" && x.csomag.csomag_kezdohelye != "x")
                    {
                        if(x.kocsik[i].kocsi_helye == x.csomag.csomag_kezdohelye)
                        {
                            if(x.csomag.csomag_mennyiseg >= x.kocsik[i].kocsi_kapacitas && x.kocsik[i].kocsi_kapacitas != 0)
                            {
                                x.csomag.csomag_mennyiseg = x.csomag.csomag_mennyiseg - x.kocsik[i].kocsi_kapacitas;

                                x.vonat.kocsi.benne = x.vonat.kocsi.benne + x.kocsik[i].kocsi_kapacitas;

                                cout << "pakol " << x.kocsik[i].kocsi_nev << " " << x.csomag.csomag_nev << " " << x.vonat.kocsi.benne << endl;

                                shipment[x.csomag.csomag_nev] = x.vonat.kocsi.benne;
                                x.vonat.kocsi.benne = 0;
                                x.kocsik[i].kocsi_kapacitas = 0;
                            }
                            else if(x.csomag.csomag_mennyiseg > 0 && x.kocsik[i].kocsi_kapacitas != 0)
                            {
                                x.vonat.kocsi.benne = x.vonat.kocsi.benne + x.csomag.csomag_mennyiseg;

                                x.csomag.csomag_mennyiseg = 0;

                                shipment[x.csomag.csomag_nev] = x.vonat.kocsi.benne;

                                cout << "pakol :" << x.kocsik[i].kocsi_nev << " " << x.csomag.csomag_nev << " " << x.vonat.kocsi.benne << endl;
                            }
                        }
                    }
                }


            if(ido == x.allomas_ora)
            {
                for(unsigned i = 0; i < x.kocsik.size(); i++)
                {
                    if(x.kocsik[i].kocsi_szabad == true && x.kocsik[i].kocsi_helye != "x" && x.vonat.vonat_biras > 0)
                    {
                        cout << "csatol " << x.kocsik[i].kocsi_nev << " " << x.vonat.vonat_nev << endl;
                        x.kocsi.kocsi_szabad = false;
                        //for(unsigned i = 0; i < x.kocsik.size(); i++)

                        int ido_2 = ido + 1;
                        for(y : allomasok)
                        {
                            if(ido_2 == y.allomas_ora)
                                y.kocsik.push_back(x.kocsik[i]);
                                //x.kocsik.erase(x.kocsik.begin() + i);
                        }


                        x.vonat.vonat_biras = x.vonat.vonat_biras - 1;
                    }
                }

                if(x.csomag.csomag_celhelye == x.allomas_nev)
                {
                    for(unsigned i = 0; i < x.kocsik.size(); i++)
                    cout << "lecsatol " << x.kocsik[i].kocsi_nev << " " << x.vonat.vonat_nev << endl;

                    //cout<< " Privezli na stanciyu "<<endl;

                    for(auto y:shipment)
                    {
                        //cout<< y.first << " " << y.second << " ";

                        shipment[y.first]=0;
                    }

                    cout << endl;
                }

                x.allomas_ora = timer;
                //объект поезд создали из файла и просто выводим для каждой стрнции где совпадает расписание
                //cout << "Menetrend: " << x.allomas_nev << " " << x.allomas_ora << " nuzhno zagruzit "<<x.csomag.csomag_mennyiseg << ", Vonat:  " << x.vonat.vonat_nev << " " << x.vonat.vonat_biras <<endl;
            }
        }

        cout << endl;
        for(auto x : allomasok)
        {
        cout << "Allomas nev: " << x.allomas_nev << endl;
        cout << "Ora: " << x.allomas_ora << endl;
        cout << "Kocsik adatai: ";
        for(auto y : x.kocsik)
            cout << y.kocsi_nev << " " << y.kocsi_kapacitas << " " << y.kocsi_helye << ", ";
        cout << endl;
        //cout << "Kocsi adatai: " << x.kocsi.kocsi_nev << " " << x.kocsi.kocsi_kapacitas << " " << x.kocsi.kocsi_helye << endl;
        cout << "Csomag adatai: " << x.csomag.csomag_nev << " " << x.csomag.csomag_kezdohelye << " " << x.csomag.csomag_celhelye << " " << x.csomag.csomag_mennyiseg << endl;
        cout << endl;
    }
    cout << endl;


        //поезд с вагонами

        if (ido == allomasok[allomasok.size()-1].allomas_ora)
        {
            ido = 0;
        }

        ido++;
        timer++;

        /*
        cout<< "in train "<<endl;
        for(auto y:shipment)
        {
            cout<< " product: "<< y.first<< " amount: "<<y.second;
        }
        cout << endl;
        */
        //system("pause");
        std::cin.get();
        cout << endl;

    }
    cout << endl;
}

int main()
{
    Allomas a;
    vector<Allomas> allomasok;

    beolvasas("vonat.txt", a, allomasok);
    beolvasas("menetrend.txt", a, allomasok);
    beolvasas("kocsik.txt", a, allomasok);
    beolvasas("csomagok.txt", a, allomasok);

    szabalyok(allomasok);

    return 0;
}
