#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#define _USE_MATH_DEFINES
#include <algorithm>
#include <bitset>


using namespace std;

float pi = atan(1) * 4;

string string2binary(string s, int wybor) //1- BigEndian inne- LittleEndian
{
    string bin;

    if (wybor == 1)
    {
        for (char& _char : s)
        {
            bin += bitset<8>(_char).to_string();
        }
        cout << bin << endl;
        return bin;
    }
    else
    {
        reverse(s.begin(), s.end());
        for (char& _char : s)
        {
            bin += bitset<8>(_char).to_string();
        }
        cout << bin << endl;
        return bin;
    }
}


int main()
{
    // zad 1 sygnał CLK

    float f = 10;
    float dlugosc = 1.6; //długość wykresu/czas na wykresie
    float lprobek = 100; //ilość próbek na bit
    float Tb = 1/(2*f); //czas trwania połowy bitu
    float lbitow = 2 * dlugosc * f; //liczba bitów
    float dt = Tb / lprobek; //delta czasu
    float size = lprobek * lbitow; //ilość próbek

    int* CLK = new int[size];
    float* czas = new float[size];
    int n = 0;

    ofstream plik1;
    plik1.open("CLK.csv");

    for (int i = 1; i <= lbitow; i++)
    {
        if (i%2!=0)
        {
            for (int j = 0; j < lprobek; j++)
            {
                CLK[n] = 1;
                czas[n] = dt * n;
                plik1 << " " << czas[n] << "," << " " << CLK[n] << endl;
                n++;
            }
        }
        else
        {
            for (int j = 1; j <= lprobek; j++)
            {
                CLK[n] = 0;
                czas[n] = dt * n;
                plik1 << " " << czas[n] << "," << " " << CLK[n] << endl;
                n++;
            }
        }
    }

    plik1.close();

    // zad 2 
    // generator TTL

    string test = "ab";
    cout << endl << "Strumien wejsciowy:     ";
    string bin = string2binary(test, 1); // strumień binarny: 0110000101100010

    float dlugosc2 = bin.length();
    float Tb2 = 1/f; //czas trwania jednego bitu
    float dt2 = Tb2 / lprobek; //delta czasu
    float size2 = lprobek * dlugosc2; //ilość próbek

    int* TTL = new int[size2];
    float* czas2 = new float[size2];
    n = 0;

    //sygnał informacyjny (m(t))
    ofstream plik2;
    plik2.open("TTL.csv");

    for (int i = 0; i < dlugosc2; i++)
    {
        if (bin[i] == '1')
        {
            for (int j = 0; j < lprobek; j++)
            {
                TTL[n] = 1;
                czas2[n] = dt2 * n;
                plik2 << " " << czas2[n] << "," << " " << TTL[n] << endl;
                n++;
            }
        }
        else if (bin[i] == '0')
        {
            for (int j = 0; j < lprobek; j++)
            {
                TTL[n] = 0;
                czas2[n] = dt2 * n;
                plik2 << " " << czas2[n] << "," << " " << TTL[n] << endl;
                n++;
            }
        }
    }

    plik2.close();

    // kodowanie NRZI

    int* NRZI = new int[size2];
    ofstream plik3;
    plik3.open("NRZI.csv");

    n = 0;
    float tmp = 0.0;


    while (tmp < Tb) //Tb to czas trwania połowy bitu
    {
        NRZI[n] = 0;
        plik3 << " " << czas2[n] << "," << " " << NRZI[n] << endl;
        n++;
        tmp = tmp + dt2;
    }


    bool pierwszazmiana = false;
 
    if (TTL[n] == 0)
    {
        while (tmp < Tb2+Tb)
        {
            NRZI[n] = NRZI[n - 1];
            plik3 << " " << czas2[n] << "," << " " << NRZI[n] << endl;
            n++;
            tmp = tmp + dt2;
        }
    }
    else
    {
        pierwszazmiana = true;
        while (tmp < Tb2+Tb)
        {
            NRZI[n] = -1;
            plik3 << " " << czas2[n] << "," << " " << NRZI[n] << endl;
            n++;
            tmp = tmp + dt2;
        }
    }

    int helper = 0;

    for (int i = 0; i < dlugosc2 - 2; i++)
    {
        if (TTL[n] == 0)
        {
            for (int j = 0; j < lprobek; j++)
            {
                NRZI[n] = NRZI[n - 1];
                plik3 << " " << czas2[n] << "," << " " << NRZI[n] << endl;
                n++;
                tmp = tmp + dt2;
            }
        }
        else
        {
            if (pierwszazmiana == true)
            {
                helper = NRZI[n - 1];
                for (int j = 0; j < lprobek; j++)
                {
                    NRZI[n] = (-1) * helper;
                    plik3 << " " << czas2[n] << "," << " " << NRZI[n] << endl;
                    n++;
                    tmp = tmp + dt2;
                }
            }
            else
            {
                pierwszazmiana = true;
                for (int j = 0; j < lprobek; j++)
                {
                    NRZI[n] = -1;
                    plik3 << " " << czas2[n] << "," << " " << NRZI[n] << endl;
                    n++;
                    tmp = tmp + dt2;
                }
            }
        }
    }

    while (tmp < dlugosc)
    {
        if (TTL[n] == 0)
        {
            NRZI[n] = NRZI[n - 1];
            plik3 << " " << czas2[n] << "," << " " << NRZI[n] << endl;
            n++;
            tmp = tmp + dt2;
        }
        else
        {
            NRZI[n] = -NRZI[n - 1];
            plik3 << " " << czas2[n] << "," << " " << NRZI[n] << endl;
            n++;
            tmp = tmp + dt2;
        }
    }

    plik3.close();


    // kodowanie BAMI

    int* BAMI = new int[size2];

    ofstream plik4;
    plik4.open("BAMI.csv");

    n = 0;
    tmp = 0.0;

    while (tmp < Tb2)
    {
        BAMI[n] = 0;
        plik4 << " " << czas2[n] << "," << " " << BAMI[n] << endl;
        n++;
        tmp = tmp + dt2;
    }

    bool minus = false;
    helper = 0;

    for (int i = 0; i < dlugosc2 - 1; i++)
    {
        if (TTL[n + 1] == 0)
        {
            for (int j = 0; j < lprobek; j++)
            {
                BAMI[n] = 0;
                plik4 << " " << czas2[n] << "," << " " << BAMI[n] << endl;
                n++;
            }
        }
        else
        {
            if (BAMI[n - 1] == 0 && minus == false)
            {
                minus = true;
                for (int j = 0; j < lprobek; j++)
                {
                    BAMI[n] = -1;
                    plik4 << " " << czas2[n] << "," << " " << BAMI[n] << endl;
                    n++;
                }
            }
            else if (BAMI[n - 1] == 0 && minus == true)
            {
                minus = false;
                for (int j = 0; j < lprobek; j++)
                {
                    BAMI[n] = 1;
                    plik4 << " " << czas2[n] << "," << " " << BAMI[n] << endl;
                    n++;
                }          
            }
            else
            {
                minus = !minus;
                helper = BAMI[n - 1];
                for (int j = 0; j < lprobek; j++)
                {
                    BAMI[n] = (-1) * helper;
                    plik4 << " " << czas2[n] << "," << " " << BAMI[n] << endl;
                    n++;
                }
            }
        }
    }

    plik4.close();

    // kodowanie Manchester

    int* MANCHESTER = new int[size2];

    ofstream plik5;
    plik5.open("Manchester.csv");

    n = 0;
    tmp = 0.0;
    helper = 0;


    while (tmp < Tb) //Tb to czas trwania połowy bitu
    {
        MANCHESTER[n] = 0;
        plik5 << " " << czas2[n] << "," << " " << MANCHESTER[n] << endl;
        n++;
        tmp = tmp + dt2;
    }

    for (int i = 0; i < dlugosc2 - 1; i++)
    {
        if (TTL[n] == 0)
        {
            for (int j = 0; j < lprobek / 2; j++)
            {
                MANCHESTER[n] = 1;
                plik5 << " " << czas2[n] << "," << " " << MANCHESTER[n] << endl;
                n++;
                tmp = tmp + dt2;
            }
        }
        else
        {
            for (int j = 0; j < lprobek / 2; j++)
            {
                MANCHESTER[n] = -1;
                plik5 << " " << czas2[n] << "," << " " << MANCHESTER[n] << endl;
                n++;
                tmp = tmp + dt2;
            }
        }

        if (TTL[n - 1] == TTL[n - 2])
        {
            helper = MANCHESTER[n - 1];
            for (int j = 0; j < lprobek / 2; j++)
            {
                MANCHESTER[n] = (-1) * helper;
                plik5 << " " << czas2[n] << "," << " " << MANCHESTER[n] << endl;
                n++;
                tmp = tmp + dt2;
            }
        }
        else
        {
            for (int j = 0; j < lprobek / 2; j++)
            {
                MANCHESTER[n] = MANCHESTER[n - 1];
                plik5 << " " << czas2[n] << "," << " " << MANCHESTER[n] << endl;
                n++;
                tmp = tmp + dt2;
            }
        }
    }


    while (tmp < dlugosc)
    {
        if (TTL[n] == 0)
        {
            MANCHESTER[n] = 1;
            plik5 << " " << czas2[n] << "," << " " << MANCHESTER[n] << endl;
            n++;
            tmp = tmp + dt2;
        }
        else
        {
            MANCHESTER[n] = -1;
            plik5 << " " << czas2[n] << "," << " " << MANCHESTER[n] << endl;
            n++;
            tmp = tmp + dt2;
        }
    }


    plik5.close();



    //------------------DEKODERY-------------------//

    //dekodowanie TTL

    n = 0;

    cout << "Dekodowanie TTL         ";

    while (n < size2)
    {
        cout << TTL[n];
        n = n + lprobek;
    }
    cout << endl;

    //dekodowanie NRZI

    n = lprobek / 2;
    int biezacy = 0;
    int nastepny = 0;

    cout << "Dekodowanie NRZI         ";

    while (n < size2)
    {
        if (NRZI[n] == 0 || NRZI[n] == 1)
            biezacy = 1;
        else
            biezacy = 0;

        n = n + lprobek;

        if (n >= size2) break;

        if (NRZI[n] == 0 || NRZI[n] == 1)
            nastepny = 1;
        else
            nastepny = 0;

        if (biezacy == nastepny)
            cout << 0;
        else
            cout << 1;
    }
    cout << endl;

    //dekodowanie BAMI

    n = lprobek;

    cout << "Dekodowanie BAMI         ";

    while (n < size2)
    {
        if (BAMI[n] == 0)
            cout << 0;
        else
            cout << 1;

        n = n + lprobek;
    }
    cout << endl;

    //dekodowanie Manchester

    n = 0.75 * lprobek;

    cout << "Dekodowanie Manchester  ";

    while (n < size2)
    {
        if (MANCHESTER[n] == -1)
            cout << 1;
        else
            cout << 0;

        n = n + lprobek;
    }
    cout << endl << endl;

    delete[] czas;
    delete[] czas2;
    delete[] CLK;
    delete[] TTL;
    delete[] NRZI;
    delete[] BAMI;
    delete[] MANCHESTER;


    return 0;
}
