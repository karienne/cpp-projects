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


float dft(float size, int obieg, float tab[])
{
    float wyn = 0;
    float re = 0;
    float im = 0;
    for (int i = 0; i < size; i++)
    {
        re = re + tab[i] * cos((-2 * obieg * pi * i) / size);
        im = im - tab[i] * sin((-2 * obieg * pi * i) / size);
    }

    wyn = sqrt(pow(re, 2) + pow(im, 2));
    return wyn;
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

    int N = 1; //częstotliwość na bit
    float A = 1.0;
    float A1 = 0.5;
    float A2 = 0.25;
    float f = N / Tb;

    //ASK
    float* ASK = new float[size];
    ofstream plik2;
    plik2.open("ASK.csv");

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
    }
    
    plik2.close();

    //FSK
    float f1 = (N + 1) / Tb;
    float f2 = (N + 2) / Tb;
    float* FSK = new float[size];
    ofstream plik3;
    plik3.open("FSK.csv");

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
    }

    plik3.close();

    //PSK
    float fi1 = 0.0;
    float fi2 = pi;
    float* PSK = new float[size];
    ofstream plik4;
    plik4.open("PSK.csv");

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
    }

    plik4.close();


    //widma amplitudowe
    vector<float> czestotliwosc;
    n = 0;
    float fk = 0.0; //skala częstotliwości
    vector<float> widmoam;
    ofstream widmo;
    widmo.open("widmo.csv");
    while (n < size)
    {
        widmoam.push_back(dft(size, n, PSK));
        if (n <= (size / 2))
            widmo << " " << fk << "," << " " << widmoam[n] << endl;
        n++;
        czestotliwosc.push_back(fk);
        fk = n * fs / size;
    }

    widmo.close();


    //skala decybelowa
    ofstream decybel;
    decybel.open("widmodecybel.csv");
    n = 0;
    while (n <= size / 2)
    {
        if (widmoam[n] < (*max_element(widmoam.begin(), widmoam.end()) / 10000))
            widmoam[n] = 0;
        else
            widmoam[n] = 10 * log10(widmoam[n]);
        decybel << " " << czestotliwosc[n] << "," <<" " << widmoam[n] << endl;
        n++;
    }
    decybel.close();


    //obliczanie szerokości pasma - na podstawie widma w skali decybelowej

    vector<float> pasmo; //sygnal, gdzie jest powyżej -3dB
    
    for (int i = 0; i < size; i++)
    {
        if (widmoam[i] >= -3)
            pasmo.push_back(czestotliwosc[i]);
    }

    float szer; //szerokość pasma

    szer = abs(*max_element(pasmo.begin(), pasmo.end()) - *min_element(pasmo.begin(), pasmo.end()));
    cout << szer;

    // dla ASK szerokość pasma sygnału = 243.125
    // dla FSK szerokość pasma sygnału = 496.875
    // dla PSK szerokość pasma sygnału = 459.375


    delete[] probki;
    delete[] czas;
    delete[] ASK;
    delete[] FSK;
    delete[] PSK;


	return 0;
}

