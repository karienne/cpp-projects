#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;



void wyswietl_plansze(vector<vector<int>> plansza)
{
    cout << "  0 1 2" << endl;

    for (int i = 0; i < 3; i++)
    {
        cout << i << " ";
        for (int j = 0; j < 3; j++)
        {
            if (plansza[i][j] == 1)
                cout << "O ";
            else if (plansza[i][j] == 2)
                cout << "X ";
            else
                cout << "  ";
        }
        cout << endl;
    }
}

vector<int> ruch_gracza(vector<vector<int>> plansza, int gracz)
{
    vector<int> pozycja;
    int wiersz, kolumna;
    while (1)
    {
        cout << "Podaj nr wiersza i kolumny, gdzie chcesz wstawic ";
        if (gracz == 1) cout << "O: " << endl;
        else cout << "X: " << endl;
        cin >> wiersz >> kolumna;

        if (wiersz >= 0 && wiersz <= 2 && kolumna >= 0 && kolumna <= 2 && plansza[wiersz][kolumna] == 0)
            break;
        cout << "Nieprawidłowe pole!" << endl;
    }
    pozycja.push_back(wiersz);
    pozycja.push_back(kolumna);

    return pozycja;

}

int sprawdz_wygrana(vector<vector<int>> plansza)
{
    if (plansza[0][0] != 0)
    {
        if (plansza[0][0] == plansza[1][1] && plansza[0][0] == plansza[2][2])
            return plansza[0][0];
    }

    if (plansza[0][2] != 0)
    {
        if (plansza[0][2] == plansza[1][1] && plansza[0][2] == plansza[2][1])
            return plansza[0][2];
    }
    for (int i = 0; i < 3; i++)
    {
        if (plansza[i][0] != 0)
        {
            if (plansza[i][0] == plansza[i][1] && plansza[i][0] == plansza[i][2])
                return plansza[i][0];
        }

        if (plansza[0][i] != 0)
        {
            if (plansza[0][i] == plansza[1][i] && plansza[0][i] == plansza[2][i])
                return plansza[0][i];
        }
    }
    bool wolne_miejsca = false;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (plansza[i][j] == 0)
            {
                wolne_miejsca = true;
                break;
            }
        }
        if (wolne_miejsca) break;
    }
    if (!wolne_miejsca)
        return -1; //remis

    return 0; //jeśli brak wygranej i są jeszcze wolne miejsca
}


vector<int> ruch_komputera(vector<vector<int>> plansza, int czyj_ruch, int gracz)
{
    vector<vector<int>> tab = plansza; //kopia planszy
    vector<vector<int>> wartosci_alfabeta;
    vector<int> pozycja;
    vector<int> helper;

    int suma = 0;
    int wiersz=-1, kolumna=-1;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            helper.push_back(-10 * czyj_ruch);
            if (tab[i][j] == 0) suma++;
        }
        wartosci_alfabeta.push_back(helper);
        helper.clear();
    }

    if (suma == 0)
    {
        int kto_wygral = sprawdz_wygrana(tab);
        if (sprawdz_wygrana(tab) == gracz)
        {
            pozycja.push_back(3); //ile miejsc zajętych i -1 czyli gracz
            pozycja.push_back(3);
            pozycja.push_back(-1);
            return pozycja;
        }
        else if (sprawdz_wygrana(tab) == gracz % 2 + 1)
        {
            pozycja.push_back(3);
            pozycja.push_back(3);
            pozycja.push_back(1);
            return pozycja;
        }
        else
        {
            pozycja.push_back(3);
            pozycja.push_back(3);
            pozycja.push_back(0); // remis
            return pozycja;
        }
    }
    for (int i = 0; i < 3; i++)
    {
        tab = plansza;
        for (int j = 0; j < 3; j++)
        {
            tab = plansza;
            if (tab[i][j] != 0)
                continue;
            if (czyj_ruch == -1) //ruch gracza
                tab[i][j] = gracz;
            else
                tab[i][j] = gracz % 2 + 1; //ruch komputera
            if (sprawdz_wygrana(tab) == gracz)
            {
                pozycja.push_back(i); //wiersz
                pozycja.push_back(j); //kolumna
                pozycja.push_back(-1);
                return pozycja;
            }
            else if (sprawdz_wygrana(tab) == gracz % 2 + 1)
            {
                pozycja.push_back(i); //wiersz
                pozycja.push_back(j); //kolumna
                pozycja.push_back(1);
                return pozycja;
            }
            wartosci_alfabeta[i][j] = ruch_komputera(tab, czyj_ruch * (-1), gracz)[2];
            if (czyj_ruch == -1 && wartosci_alfabeta[2][2] == -1)
                break;
            if (czyj_ruch == 1 && wartosci_alfabeta[2][2] == 1)
                break;
        }
    }
    int tmp = -10 * czyj_ruch;
    if (czyj_ruch == -1)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (wartosci_alfabeta[i][j] < tmp)
                {
                    tmp = wartosci_alfabeta[i][j];
                    wiersz = i;
                    kolumna = j;
                }
                else if (wartosci_alfabeta[i][j] == tmp && rand() % 2 == 1)
                {
                    tmp = wartosci_alfabeta[i][j];
                    wiersz = i;
                    kolumna = j;
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (wartosci_alfabeta[i][j] > tmp)
                {
                    tmp = wartosci_alfabeta[i][j];
                    wiersz = i;
                    kolumna = j;
                }
                else if (wartosci_alfabeta[i][j] == tmp && rand() % 2 == 1)
                {
                    tmp = wartosci_alfabeta[i][j];
                    wiersz = i;
                    kolumna = j;
                }
            }
        }
    }

    pozycja.clear();
    pozycja.push_back(wiersz);
    pozycja.push_back(kolumna);
    pozycja.push_back(tmp);

    return pozycja;
}


int gra(int gracz)
{
    vector<int> pozycja;
    vector<vector<int>> plansza;
    vector<int> zera;
    int wiersz, kolumna;
    int kto_wygral;
    int liczba_ruchow = 0;

    for (int i = 0; i < 3; i++)
    {
        zera.push_back(0);
    }

    for (int i = 0; i < 3; i++)
    {
        plansza.push_back(zera);
    }

    if (gracz == 2)
    {
        system("cls");
        wyswietl_plansze(plansza);
        pozycja = ruch_komputera(plansza, 1, gracz);
        wiersz = pozycja[0];
        kolumna = pozycja[1];
        plansza[wiersz][kolumna] = 1;
        liczba_ruchow++;
    }

    while (1)
    {
        system("cls");
        wyswietl_plansze(plansza);
        pozycja = ruch_gracza(plansza, gracz);
        wiersz = pozycja[0];
        kolumna = pozycja[1];
        plansza[wiersz][kolumna] = gracz;
        liczba_ruchow++;

        if (liczba_ruchow > 4)
        {
            kto_wygral = sprawdz_wygrana(plansza);
            if (kto_wygral != 0)
            {
                system("cls");
                wyswietl_plansze(plansza);
                break;
            }
              
        }
        

        system("cls");
        wyswietl_plansze(plansza);
        pozycja = ruch_komputera(plansza, 1, gracz);
        wiersz = pozycja[0];
        kolumna = pozycja[1];
        plansza[wiersz][kolumna] = gracz % 2 + 1;
        liczba_ruchow++;
        
        if (liczba_ruchow > 4)
        {
            kto_wygral = sprawdz_wygrana(plansza);
            if (kto_wygral != 0)
            {
                system("cls");
                wyswietl_plansze(plansza);
                break;
            }
        }

    }

    return kto_wygral;
}

void execute()
{
    int gracz; //1- gra jako pierwszy, 2- gra jako drugi
    while (1)
    {
        cout << "Grasz jako O [1] czy X [2]? ";
        cin >> gracz;
        if (gracz == 1 || gracz == 2) break;
        system("cls");
        cout << endl << "Wybierz 1 lub 2!" << endl;
    }

    int kto_wygral = gra(gracz);
    if (kto_wygral == gracz)
    {
        cout << "Wygrales!";
    }
    else if (kto_wygral == gracz % 2 + 1)
    {
        cout << "Przegrales!";
    }
    else
        cout << "Remis!";

}


int main()
{
    execute();

    return 0;
}
