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
    string test = "ab";
    string bin = string2binary(test, 1); // strumień binarny: 0110000101100010

    float dlugosc = bin.length();
    float lprobek = 100; //ilość próbek na bit
    float Tb = 0.1; //czas trwania jednego bitu
    float dt = Tb / lprobek; //delta czasu
    float fs = 1 / dt; //częstotliwość próbkowania
    float size = lprobek * dlugosc; //ilość próbek

    int* probki = new int[size];
    float* czas = new float[size];
    int n = 0;

    //sygnał informacyjny (m(t))
    ofstream plik1;
    plik1.open("sygnal_informacyjny.csv");

    for (int i = 0; i < dlugosc; i++)
    {
        if (bin[i] == '1')
        {
            for (int j = 0; j < lprobek; j++)
            {
                probki[n] = 1;
                czas[n] = dt * n;
                plik1 << " " << czas[n] << "," << " " << probki[n] << endl;
                n++;
            }
        }
        else if (bin[i] == '0')
        {
            for (int j = 0; j < lprobek; j++)
            {
                probki[n] = 0;
                czas[n] = dt * n;
                plik1 << " " << czas[n] << "," << " " << probki[n] << endl;
                n++;
            }
        }
    }

    plik1.close();


    int N = 2; //częstotliwość na bit
    float A = 1.0;
    float A1 = 0;
    float A2 = 1.0;
    float f = N / Tb;

    //ASK
    float* ASK = new float[size];
    float* sASK = new float[size]; //sygnał nośny dla ASK
    float* xASK = new float[size]; //przemnożony sygnał zmodulowany przez sygnał nośny
    float* pASK = new float[size]; //funkcja całkowa dla ASK
    ofstream plik2, plikk, plik5;
    plik2.open("ASK.csv");
    plikk.open("sygnał_nośnyASK.csv");
    plik5.open("xASK.csv");
    

    for (int i = 0; i < size; i++)
    {
        if (probki[i] == 0)
        {
            ASK[i] = A1 * sin(2 * pi * f * czas[i]);
            plik2 << " " << czas[i] << "," << " " << ASK[i] << endl;
        }
        else if (probki[i] == 1)
        {
            ASK[i] = A2 * sin(2 * pi * f * czas[i]);
            plik2 << " " << czas[i] << "," << " " << ASK[i] << endl;
        }

        sASK[i] = A2 * sin(2 * pi * f * czas[i]);
        plikk << " " << czas[i] << "," << " " << sASK[i] << endl;

        xASK[i] = ASK[i] * sASK[i];
        plik5 << " " << czas[i] << "," << " " << xASK[i] << endl;
    }

    plikk.close();
    plik2.close();
    plik5.close();

    ofstream plik6;
    plik6.open("całkowanieASK.csv");
    pASK[0] = 0.0;
    plik6 << " " << czas[0] << "," << " " << pASK[0] << endl;

    n = 1;

    for (int i = 0; i < dlugosc; i++)
    {
        for (int j = 1; j < lprobek; j++)
        {
            pASK[n] = pASK[n-1] + xASK[n];
            plik6 << " " << czas[n] << "," << " " << pASK[n] << endl;
            n++;
        }
        if (n == size) break;
        pASK[n] = 0.0;
        plik6 << " " << czas[n] << "," << " " << pASK[n] << endl;
        n++;  
    }

    plik6.close();

    //komparator

    float* mASK = new float[size]; //zdemodulowany sygnał
    float h = 0.08;
    
    ofstream plik11;
    plik11.open("mASK.csv");

    for (int i = 0; i < size; i++)
    {
        if (pASK[i] >= h)
            mASK[i] = 1;
        else
            mASK[i] = 0;
        plik11 << " " << czas[i] << "," << " " << mASK[i] << endl;
    }

    plik11.close();




    //FSK
    float f1 = (N + 1) / Tb;
    float f2 = (N + 2) / Tb;
    float* FSK = new float[size];
    float* sFSK1 = new float[size]; //sygnał nośny dla FSK zera binarne
    float* sFSK2 = new float[size]; //sygnał nośny dla FSK jedynki binarne
    float* xFSK1 = new float[size]; //przemnożony sygnał nośny przez zmodulowany zera
    float* xFSK2 = new float[size]; //przemnożony jedynki
    float* pFSK1 = new float[size]; //funkcja całkowa dla FSK zera
    float* pFSK2 = new float[size]; //funkcja całkowa dla FSK jedynki
    float* pFSK = new float[size]; //funkcja całkowa ostateczna

    ofstream plik3, plikk2, plik7, plik17;
    plik3.open("FSK.csv");
    plikk2.open("sygnał_nośnyFSK.csv");
    plik7.open("xFSK1.csv");
    plik17.open("xFSK2.csv");

    for (int i = 0; i < size; i++)
    {
        if (probki[i] == 0)
        {
            FSK[i] = A * sin(2 * pi * f1 * czas[i]);
            plik3 << " " << czas[i] << "," << " " << FSK[i] << endl;
        }
        else if (probki[i] == 1)
        {
            FSK[i] = A * sin(2 * pi * f2 * czas[i]);
            plik3 << " " << czas[i] << "," << " " << FSK[i] << endl;
        }
        sFSK1[i] = A * sin(2 * pi * f1 * czas[i]);
        plikk2 << " " << czas[i] << "," << " " << sFSK1[i] << endl;

        sFSK2[i] = A * sin(2 * pi * f2 * czas[i]);

        xFSK1[i] = FSK[i] * sFSK1[i];
        plik7 << " " << czas[i] << "," << " " << xFSK1[i] << endl;

        xFSK2[i] = FSK[i] * sFSK2[i];
        plik17 << " " << czas[i] << "," << " " << xFSK2[i] << endl;
    }

    plik3.close();
    plikk2.close();
    plik7.close();
    plik17.close();

    ofstream plik13, plik14, plik15;
    plik13.open("całkowanieFSK1.csv");
    plik14.open("całkowanieFSK2.csv");
    plik15.open("całkowanieFSK.csv");

    pFSK1[0] = 0.0;
    pFSK2[0] = 0.0;
    pFSK[0] = 0.0;
    plik13 << " " << czas[0] << "," << " " << pFSK1[0] << endl;
    plik14 << " " << czas[0] << "," << " " << pFSK2[0] << endl;
    plik15 << " " << czas[0] << "," << " " << pFSK[0] << endl;

    n = 1;

    for (int i = 0; i < dlugosc; i++)
    {
        for (int j = 1; j < lprobek; j++)
        {
            pFSK1[n] = pFSK1[n - 1] + xFSK1[n];
            plik13 << " " << czas[n] << "," << " " << pFSK1[n] << endl;

            pFSK2[n] = pFSK2[n - 1] + xFSK2[n];
            plik14 << " " << czas[n] << "," << " " << pFSK2[n] << endl;

            pFSK[n] = pFSK2[n] - pFSK1[n];
            plik15 << " " << czas[n] << "," << " " << pFSK[n] << endl;

            n++;
        }
        if (n == size) break;
        pFSK1[n] = 0.0;
        pFSK2[n] = 0.0;
        pFSK[n] = 0.0;
        plik13 << " " << czas[n] << "," << " " << pFSK1[n] << endl;
        plik14 << " " << czas[n] << "," << " " << pFSK2[n] << endl;
        plik15 << " " << czas[n] << "," << " " << pFSK[n] << endl;

        n++;
    }

    plik13.close();
    plik14.close();
    plik15.close();

    //komparator

    float* mFSK = new float[size]; //zdemodulowany sygnał
    h = 0.9;

    ofstream plik16;
    plik16.open("mFSK.csv");

    for (int i = 0; i < size; i++)
    {
        if (pFSK[i] >= h)
            mFSK[i] = 1;
        else
            mFSK[i] = 0;
        plik16 << " " << czas[i] << "," << " " << mFSK[i] << endl;
    }

    plik16.close();



    //PSK
    float fi1 = 0.0;
    float fi2 = pi;
    float* PSK = new float[size];
    float* sPSK = new float[size]; //sygnał nośny dla PSK
    float* xPSK = new float[size]; //przemnożony
    float* pPSK = new float[size]; //funkcja całkowa dla PSK
    ofstream plik4, plikk3, plik9;
    plik4.open("PSK.csv");
    plikk3.open("sygnał_nośnyPSK.csv");
    plik9.open("xPSK.csv");

    for (int i = 0; i < size; i++)
    {
        if (probki[i] == 0)
        {
            PSK[i] = A * sin(2 * pi * f * czas[i] + fi1);
            plik4 << " " << czas[i] << "," << " " << PSK[i] << endl;
        }
        else if (probki[i] == 1)
        {
            PSK[i] = A * sin(2 * pi * f * czas[i] + fi2);
            plik4 << " " << czas[i] << "," << " " << PSK[i] << endl;
        }
        sPSK[i] = A * sin(2 * pi * f * czas[i] + fi2);
        plikk3 << " " << czas[i] << "," << " " << sPSK[i] << endl;

        xPSK[i] = PSK[i] * sPSK[i];
        plik9 << " " << czas[i] << "," << " " << xPSK[i] << endl;
    }

    plik4.close();
    plikk3.close();
    plik9.close();

    ofstream plik10;
    plik10.open("całkowaniePSK.csv");
    pPSK[0] = 0.0;
    plik10 << " " << czas[0] << "," << " " << pPSK[0] << endl;

    n = 1;

    for (int i = 0; i < dlugosc; i++)
    {
        for (int j = 1; j < lprobek; j++)
        {
            pPSK[n] = pPSK[n - 1] + xPSK[n];
            plik10 << " " << czas[n] << "," << " " << pPSK[n] << endl;
            n++;
        }
        if (n == size) break;
        pPSK[n] = 0.0;
        plik10 << " " << czas[n] << "," << " " << pPSK[n] << endl;
        n++;
    }

    plik10.close();

    //komparator

    float* mPSK = new float[size]; //zdemodulowany sygnał
    h = 0.04;

    ofstream plik12;
    plik12.open("mPSK.csv");

    for (int i = 0; i < size; i++)
    {
        if (pPSK[i] >= h)
            mPSK[i] = 1;
        else
            mPSK[i] = 0;
        plik12 << " " << czas[i] << "," << " " << mPSK[i] << endl;
    }

    plik12.close();



    delete[] probki;
    delete[] czas;

    delete[] ASK;
    delete[] sASK;
    delete[] xASK;
    delete[] pASK;
    delete[] mASK;

    delete[] FSK;
    delete[] sFSK1;
    delete[] sFSK2;
    delete[] xFSK1;
    delete[] xFSK2;
    delete[] pFSK1;
    delete[] pFSK2;
    delete[] pFSK;
    delete[] mFSK;


    delete[] PSK;
    delete[] sPSK;
    delete[] xPSK;
    delete[] pPSK;
    delete[] mPSK;
    


    return 0;
}

// sygnał x(t) poddajemy całkowaniu w czasie; wraz z upływem czasu wartość funkcji
// podcałkowej przyrasta o tyle, o ile przyrasta jej pole pod funkcją
// wartość funkcji będzie równa sumie wartości bieżącej x(t) i wartości poprzednich
// całkujemy od zera do Tb, gdy dojedziemy do Tb znowu zerujemy sobie licznik, znowu
// całkujemy od zera (co Tb sekundy się zeruje)

//demodulacja FSK- od sygnału całkowania dla jedynek binarnych odejmujemy sygnał całkowania dla zer binarnych
