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


class Board {
public:
    int priority;
    vector<int> queens;
    Board(vector<int>& queens, int priority)
    {
        this->queens = queens;
        this->priority = priority;
    }
    void print()
    {
        for (int i = 0; i < queens.size(); i++)
        {
            std::cout << "(" << i + 1 << ", " << queens[i] << ")";
        }
        std::cout << std::endl;
    }
};

bool operator< (Board a, Board b)
{
    return a.priority > b.priority;
}


vector<vector<int>> BruteForce(int n, vector<int> tab)
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


vector<vector<int>> Ulepszona(int n, vector<int> tab)
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


int h2(int n, vector<int> potomek)
{
    int size = potomek.size(); //ile hetmanów jest wstawionych
    int next = size + 1; //nr następnej kolumny do wstawienia hetmana
    int x1, x2, y1, y2;
    int bicia = 0;

    for (int i = next; i <= n; i++)
    {
        x1 = i;
        for (int j = 1; j <= n; j++)
        {
            y1 = j;
            for (int k = 0; k < size; k++)
            {
                x2 = k + 1;
                y2 = potomek[k];
                if (y1 == y2 || abs(x1 - x2) == abs(y1 - y2))
                {
                    bicia++;
                    break;
                }
            }
        }
    }

    return bicia;
}

vector<Board> BestFirstSearch(int n, int podejscie, int h, int opcja)
{
    priority_queue<Board> myqueue;

    vector<int> currentvector;
    Board currentboard(currentvector, 0);

    vector<Board> rozw; //wektor wszystkich rozwiązań
    vector<vector<int>> potomkowie;

    myqueue.push(currentboard);

    if (podejscie == 1)
    {
        int x1, x2, y1, y2; //kolumny i wiersze
        bool warunek = true; //czy hetmany się nie biją

        while (!myqueue.empty())
        {
            currentboard = myqueue.top();
            myqueue.pop();

            if (currentboard.queens.size() == n) //sprawdzanie warunku (2 hetmany nie mogą się bić)
            {
                sprawdzone++;
                for (int i = 0; i < n; i++)
                {
                    x1 = i + 1;
                    y1 = currentboard.queens[i];

                    for (int j = x1; j < n; j++)
                    {
                        x2 = j + 1;
                        y2 = currentboard.queens[j];

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
                    rozw.push_back(currentboard);
                    if (opcja == 1) return rozw;
                }
            }

            else if (currentboard.queens.size() < n)
            {
                potomkowie = BruteForce(n, currentboard.queens); // wektor potomków

                auto dodawanypotomek = Board(currentvector, 0);
                int wartosc; 

                if (h == 1) 
                {
                    for (int i = 0; i < potomkowie.size(); i++)
                    {
                        wartosc = 0;
                        for (int j = 0; j < potomkowie[i].size(); j++)
                        {
                            if (potomkowie[i][j] < (n / 2))
                                wartosc = wartosc + n - potomkowie[i][j] + 1;
                            else
                                wartosc = wartosc + potomkowie[i][j];
                        }

                        dodawanypotomek.queens = potomkowie[i];
                        dodawanypotomek.priority = wartosc;
                        myqueue.push(dodawanypotomek);
                    }

                }
                else if (h == 2)
                {
                    for (int i = 0; i < potomkowie.size(); i++)
                    {
                        wartosc = h2(n, potomkowie[i]);
                        dodawanypotomek.queens = potomkowie[i];
                        dodawanypotomek.priority = wartosc;
                        myqueue.push(dodawanypotomek);
                    }
                }
                else if (h == 3)
                {
                    int S = (n * n - n) / 2;
                    int hamming;

                    for (int i = 0; i < potomkowie.size(); i++)
                    {
                        hamming = 0;
                        for (int j = 0; j < potomkowie[i].size()-1; j++)
                        {
                            if (potomkowie[i][j] != potomkowie[i][j + 1])
                            {
                                hamming++;
                            }
                        }
                        wartosc = S - hamming;
                        dodawanypotomek.queens = potomkowie[i];
                        dodawanypotomek.priority = wartosc;
                        myqueue.push(dodawanypotomek);
                    }
                }
            }

            warunek = true;
        }
    }
    else if (podejscie == 2)
    {
        while (!myqueue.empty())
        {
            currentboard = myqueue.top();
            myqueue.pop();

            if (currentboard.queens.size() == n)
            {
                sprawdzone++;
                rozw.push_back(currentboard);
                if (opcja == 1) return rozw;
            }
            else if (currentboard.queens.size() < n)
            {
                potomkowie = Ulepszona(n, currentboard.queens);
                auto dodawanypotomek = Board(currentvector, 0);
                int wartosc; 

                if (h == 1) 
                {
                    for (int i = 0; i < potomkowie.size(); i++)
                    {
                        wartosc = 0;
                        for (int j = 0; j < potomkowie[i].size(); j++)
                        {
                            if (potomkowie[i][j] < (n / 2))
                                wartosc = wartosc + n - potomkowie[i][j] + 1;
                            else
                                wartosc = wartosc + potomkowie[i][j];
                        }

                        //Board dodawanypotomek(potomkowie[i], wartosc);
                        dodawanypotomek.queens = potomkowie[i];
                        dodawanypotomek.priority = wartosc;
                        myqueue.push(dodawanypotomek);
                    }

                }
                else if (h == 2)
                {
                    for (int i = 0; i < potomkowie.size(); i++)
                    {
                        wartosc = h2(n, potomkowie[i]);
                        dodawanypotomek.queens = potomkowie[i];
                        dodawanypotomek.priority = wartosc;
                        myqueue.push(dodawanypotomek);
                    }
                }
                else if (h == 3)
                {
                    int S = (n * n - n) / 2;
                    int hamming;

                    for (int i = 0; i < potomkowie.size(); i++)
                    {
                        hamming = 0;
                        for (int j = 0; j < potomkowie[i].size() - 1; j++)
                        {
                            if (potomkowie[i][j] != potomkowie[i][j + 1])
                            {
                                hamming++;
                            }
                        }
                        wartosc = S - hamming;
                        dodawanypotomek.queens = potomkowie[i];
                        dodawanypotomek.priority = wartosc;
                        myqueue.push(dodawanypotomek);
                    }
                }
            }
        }
    }
    return rozw;

}

void Execute()
{
    int n;
    int metoda;
    int h; //heurystyka
    int ilerozw;
    clock_t start, stop;
    float czas;

    vector<Board> rozwiazania;


    cout << "Podaj N: ";
    cin >> n;

    cout << endl << "Jakiej metody uzyc?  1) Brute Force  2) Ulepszona: ";
    cin >> metoda;

    cout << endl << "Jaka heurystyka?  1) h1  2) h2  3) h3: ";
    cin >> h;

    cout << endl << "Ilu rozwiazan szukac?  1) Jedno  2) Wszystkie: ";
    cin >> ilerozw;

    start = clock();
    rozwiazania = BestFirstSearch(n, metoda, h, ilerozw);
    stop = clock();

    czas = (float)(stop - start) / CLOCKS_PER_SEC;

    cout << endl << endl << "Znaleziono nastepujace rozwiazania: " << endl << endl;


    int size = rozwiazania.size();
    int x, y;
    for (int i = 0; i < size; i++)
    {
        cout << "Rozwiazanie " << i + 1 << endl;
        for (int j = 0; j < n; j++)
        {
            x = j + 1;
            y = rozwiazania[i].queens[j];
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

