#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#define _USE_MATH_DEFINES
#include <algorithm>


using namespace std;


float pi = atan(1) * 4;


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

float ton_prosty(float a, float fm, float t) //sygnał informacyjny
{
    return a * sin(2 * pi * fm * t);
}


int main()
{
    float fs = 1000;
    float am = 1.0;
    float f = 1.0;
    float tn = 0;
    float dt = 1.0 / fs;
    int n = 0;
    float T = 3; //okres
    float lprobek = T * fs;
    float* tab = new float[lprobek];
    float* czas = new float[lprobek];

    ofstream wyk1;
    wyk1.open("wykres_ton_prosty.csv");


    while (tn < T)
    {
        tab[n] = ton_prosty(am, f, tn);
        wyk1 << " " << tn << "," << " " << tab[n] << endl;
        n++;
        czas[n] = tn;
        tn = n * dt;
    }

    wyk1.close();

    //Modulacja amplitudy

    float* mod_am = new float[lprobek];
    float fn = 10;
    float ka = 0.5;
    tn = 0;
    n = 0;

    ofstream wyk2;
    wyk2.open("mod_am.csv");

    while(tn < T)
    {
        mod_am[n] = (ka * tab[n] + 1) * cos(2 * pi * fn * tn);
        wyk2 << " " << tn << "," << " " << mod_am[n] << endl;
        n++;
        tn = n * dt;
    }

    wyk2.close();

    //Modulacja fazy

    float* mod_fazy = new float[lprobek];
    tn = 0;
    n = 0;
    float kp = 1;
    ofstream wyk3;
    wyk3.open("modulacja_fazy.csv");

    while (tn < T)
    {
        mod_fazy[n] = cos(2 * pi * fn * tn + kp * tab[n]);
        wyk3 << " " << tn << "," << " " << mod_fazy[n] << endl;
        n++;
        tn = n * dt;
    }

    wyk3.close();


    //widma amplitudowe

    vector<float> czestotliwosc;
    n = 0;
    float fk = 0.0; //skala częstotliwości
    vector<float> widmoam;

    ofstream widmo;
    widmo.open("widmo.csv");

    while (n < lprobek)
    {
        widmoam.push_back(dft(lprobek, n, mod_fazy)*2/lprobek);
        if (n <= (lprobek / 2))
            widmo << " " << fk << "," << " " << widmoam[n] << endl;
        n++;
        czestotliwosc.push_back(fk);
        fk = n * fs / lprobek;
    }

    widmo.close();

    //skala decybelowa

    ofstream decybel;
    decybel.open("widmodecybel.csv");
    n = 0;
    while (n <= lprobek / 2)
    {
        if (widmoam[n] < (*max_element(widmoam.begin(), widmoam.end()) / 10000))
            widmoam[n] = 0;
        else
            widmoam[n] = 10 * log10(widmoam[n]);
        decybel << " " << czestotliwosc[n] << "," << " " << widmoam[n] << endl;
        n++;
    }
    decybel.close();


    //obliczanie szerokości pasma - na podstawie widma w skali decybelowej

    vector<float> pasmo; //sygnał, gdzie jest poniżej -3dB

    for (int i = 0; i < lprobek; i++)
    {
        if (widmoam[i] >= -3 && widmoam[i]!=0)
            pasmo.push_back(czestotliwosc[i]);
    }

    float szer; //szerokość pasma

    szer = abs(*max_element(pasmo.begin(), pasmo.end()) - *min_element(pasmo.begin(), pasmo.end()));
    cout << szer;

    //a) szerokość pasma widma dla modulacji amplitudowej- 989.667
    //   dla modulacji fazy- 


    return 0;
}

