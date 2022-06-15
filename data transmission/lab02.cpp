#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

const int A = 3;
const int B = 1;
const int C = 3;
float pi = atan(1) * 4;


float ton_prosty(float a, float f, float fi, float t) //A- amplituda, f- częstotliwość, fi- kąt przesunięcia w fazie, t- czas w danej chwili
{
    float w = 2 * pi * f;
    return a * sin(w * t + fi);
}

void quantize(float tab[], float t0, float dt)
{
    const int q = 8;
    const int el = pow(2, q);
    float delta = 2.0 / el;
    float* tab2 = new float[el];
    tab2[0] = -1;

    for (int i = 1; i < el; i++)
    {
        tab2[i] = -1 + i * delta;
    }
    
    float tn = t0;
    int n = 1;
    ofstream wyk2;
    wyk2.open("kwantyzacja2.csv");

    while (tn < A)
    {
        if (tab[n - 1] >= tab2[el - 1])
            tab[n - 1] = el - 1;
        else
        {
            for (int j = 1; j < el; j++)
            {
                if (tab[n - 1] < tab2[j])
                {
                    tab[n - 1] = j - 1;
                    break;
                }

            }

        }
        wyk2 << " " << tn << "," << " " << tab[n - 1] << endl;
        tn = t0 + n * dt;
        n++;

    }

    wyk2.close();

}

int main()
{
    //1

    const int fs = 12;
    float t0 = 0;
    float tn = t0;
    float dt = 1.0 / fs;
    int n = 0;
    float tab[A * fs];

    ofstream wyk1;
    wyk1.open("wykres_ton_prosty.csv");


    while (tn < A)
    {
        tab[n] = ton_prosty(1.0, B, C * pi, tn);
        wyk1 << " " << tn << "," << " " << tab[n] << endl;
        n++;
        tn = t0 + n * dt;
    }

    wyk1.close();

    //2

    quantize(tab, t0, dt);

    return 0;
    
}

//uwzględnić przypadek że nie znaleziono
//notatki z tablicy do kwantyzacji:
// int kwantyzuj(float s)
// {
//   for(int i=0; i<2^q; i++)
//     zamieniam skalę od -1 do 1 na od 0 do 2^16
// dzielimy wykres pionowymi kreskami na równe części np na 60
// -1 + 2/2^q
// -1 + 2 * 2/2^q
// -1 + 3 * 2/2^q
// -1 + 4 * 2/2^q



