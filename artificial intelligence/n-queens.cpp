#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include<math.h>
#include<queue>
#include <stack>


using namespace std;

int wygenerowane = 0;
int sprawdzone = 0;

vector<vector<int>> BruteForce(int n, vector<int> tab) //zwraca wektor potomków danego stanu tab
{
    vector<int> tab2 = tab; //wektor pomocniczy do dodawania potomków
    vector<vector<int>> potomkowie; //wektor potomków danego stanu

    for (int i = 1; i <= n; i++)
    {
        tab2.push_back(i);
        potomkowie.push_back(tab2);
        wygenerowane++;
        tab2.pop_back();
    }

    return potomkowie;
}

vector<vector<int>> Ulepszona(int n, vector<int> tab) //zwraca wektor potomków danego stanu tab (tylko dozwolone)
{
    vector<int> tab2 = tab; //wektor pomocniczy
    vector<vector<int>> potomkowie;
    int size = tab.size();

    if (size == 0)
    {
        for (int i = 1; i <= n; i++)
        {
            tab2.push_back(i);
            potomkowie.push_back(tab2);
            wygenerowane++;
            tab2.pop_back();
        }
    }
    else
    { 
        int x1, y1, y2;
        int x2 = size + 1; 
        bool warunek;

        for (int i = 1; i <= n; i++)
        {
            warunek = true;
            y2 = i;

            for (int j = 1; j <= size; j++)
            {
                x1 = j;
                y1 = tab[j - 1];
                if (y1 == y2 || abs(x1 - x2) == abs(y1 - y2))
                {
                    warunek = false;
                    break;
                }               
            }
            if (warunek == true)
            {
                tab2.push_back(i);
                potomkowie.push_back(tab2);
                wygenerowane++;
                tab2.pop_back();
            }
        }
    }
    return potomkowie;   
}


vector<vector<int>> BFS(int n, int podejscie, int opcja) 
/*n- wymiary tablicy i liczba hetmanów do ustawienia, 
  podejscie- 1- BruteForce, 2- Ulepszone
  opcja - 1- czy chcemy jedno rozwiązanie, 2- czy chcemy wszystkie rozwiązania
  */
{
    queue<vector<int>> myqueue;

    vector<int> currentvector; //sprawdzany wektor
    vector<vector<int>> rozw; //wektor wszystkich rozwiązań
    vector<vector<int>> potomkowie;

    myqueue.push(currentvector);

    if (podejscie == 1)
    {
        int x1, x2, y1, y2; //kolumny i wiersze
        bool warunek; //czy hetmany się nie biją

        while (!myqueue.empty())
        {
            warunek = true;
            currentvector = myqueue.front();
            myqueue.pop();

            if (currentvector.size() == n) //sprawdzanie warunku (2 hetmany nie mogą się bić)
            {
                sprawdzone++;
                for (int i = 0; i < n; i++)
                {
                    x1 = i + 1;
                    y1 = currentvector[i];

                    for (int j = x1; j < n; j++)
                    {
                        x2 = j + 1;
                        y2 = currentvector[j];

                        if (y1 == y2 || abs(x1 - x2) == abs(y1 - y2))
                        {
                            warunek = false;
                            break;
                        }
                    }
                    if (warunek == false)
                        break;
                }
                if (warunek == true)
                {
                    rozw.push_back(currentvector);
                    if (opcja == 1) return rozw;
                }
            }
            else if(currentvector.size() < n)
            {
                potomkowie = BruteForce(n, currentvector);

                for (int i = 0; i < potomkowie.size(); i++)
                    myqueue.push(potomkowie[i]);
            }

            /*if (opcja == 1 && !rozw.empty())
                return rozw;*/

            //currentvector = vector<int>();   
        }
    }
    else if (podejscie == 2)
    {
        while (!myqueue.empty())
        {
            currentvector = myqueue.front();
            myqueue.pop();

            if (currentvector.size() == n)
            {
                sprawdzone++;
                rozw.push_back(currentvector);
                if (opcja == 1) return rozw;
            }
            else if(currentvector.size() < n)
            {
                potomkowie = Ulepszona(n, currentvector);

                for (int i = 0; i < potomkowie.size(); i++)
                    myqueue.push(potomkowie[i]);
            }
        }
    }
    return rozw;
}


vector<vector<int>> DFS(int n, int podejscie, int opcja)
{
    stack<vector<int>> mystack;

    vector<int> currentvector; //sprawdzany wektor
    vector<vector<int>> rozw; //wektor wszystkich rozwiązań
    vector<vector<int>> potomkowie;

    mystack.push(currentvector);

    if (podejscie == 1)
    {
        int x1, x2, y1, y2; //kolumny i wiersze
        bool warunek = true; //czy hetmany się nie biją

        while (!mystack.empty())
        {
            currentvector = mystack.top();
            mystack.pop();

            if (currentvector.size() == n) //sprawdzanie warunku (2 hetmany nie mogą się bić)
            {
                sprawdzone++;
                for (int i = 0; i < n; i++)
                {
                    x1 = i + 1;
                    y1 = currentvector[i];

                    for (int j = x1; j < n; j++)
                    {
                        x2 = j + 1;
                        y2 = currentvector[j];

                        if (y1 == y2 || abs(x1 - x2) == abs(y1 - y2))
                        {
                            warunek = false;
                            break;
                        }
                    }
                    if (warunek == false)
                        break;
                }
                if (warunek == true)
                {
                    rozw.push_back(currentvector);
                    if (opcja == 1) return rozw;
                }
            }

            else if (currentvector.size() < n)
            {
                potomkowie = BruteForce(n, currentvector);

                for (int i = 0; i < potomkowie.size(); i++)
                    mystack.push(potomkowie[i]);
            }

            /*if (opcja == 1 && !rozw.empty())
                return rozw;*/

            currentvector = vector<int>();
            warunek = true;
        }
    }
    else if (podejscie == 2)
    {
        while (!mystack.empty())
        {
            currentvector = mystack.top();
            mystack.pop();

            if (currentvector.size() == n)
            {
                sprawdzone++;
                rozw.push_back(currentvector);
                if (opcja == 1) return rozw;
            }
            else if (currentvector.size() < n)
            {
                potomkowie = Ulepszona(n, currentvector);

                for (int i = 0; i < potomkowie.size(); i++)
                    mystack.push(potomkowie[i]);
            }
        }
    }
    return rozw;

}

void Execute()
{
    int n;
    int alg;
    int metoda;
    int ilerozw;
    clock_t start, stop;
    float czas;

    vector<vector<int>> rozwiazania;


    cout << "Podaj N: ";
    cin >> n;

    cout << endl << "Jakiego algorytmu uzyc?  1) BFS  2) DFS: ";
    cin >> alg;

    cout << endl << "Jakiej metody uzyc?  1) Brute Force  2) Ulepszona: ";
    cin >> metoda;

    cout << endl << "Ilu rozwiazan szukac?  1) Jedno  2) Wszystkie: ";
    cin >> ilerozw;

    if (alg == 1)
    {
        start = clock();
        rozwiazania = BFS(n, metoda, ilerozw);
        stop = clock();
    }
    else if (alg == 2)
    {
        start = clock();
        rozwiazania = DFS(n, metoda, ilerozw);
        stop = clock();
    }
        
    czas = (float)(stop - start) / CLOCKS_PER_SEC;

    cout << endl << endl << "Znaleziono nastepujace rozwiazania: " << endl << endl;

    //PozycjeHetmanow(n, rozwiazania);

    int size = rozwiazania.size();
    int x, y;
    for (int i = 0; i < size; i++)
    {
        cout << "Rozwiazanie " << i + 1 << endl;
        for (int j = 0; j < n; j++)
        {
            x = j + 1;
            y = rozwiazania[i][j];
            cout << "(" << x << "," << y << ")";
        }
        cout << endl;
    }


    cout << endl << "Czas znalezienia: " << czas << endl;
    cout << "Liczba stanow wygenerowanych: " << wygenerowane << endl;
    cout << "Liczba stanow sprawdzonych: " << sprawdzone << endl;

}


int main()
{

    Execute();

    return 0;
}

