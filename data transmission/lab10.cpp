/* 
DANE WEJŚCIOWE: 
011000010110110001100001

alpha=0.5
Zdekodowane strumienie binarne:
ASK: 011000011000000000101000 BER = 12/24
FSK: 010100000000000000000000 BER = 14/24
PSK: 101000001101101111001110 BER = 17/24

dla każdego alfa, nawet 0.9 wychodzi bardzo wysoki wskaźnik BER (podobny jw)...

*/


#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#define _USE_MATH_DEFINES
#include <algorithm>
#include <bitset>
#include <sstream>
#include <cstdlib>
#include <ctime>


using namespace std;

float pi = atan(1) * 4;


float dft(float size, int obieg, float* tab)
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

float dftvector(float size, int obieg, vector<float> tab)
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

vector<int> string2binary(string s, int wybor) //1- BigEndian inne- LittleEndian
{
    string bin;

    if (wybor == 1)
    {
        for (char& _char : s)
        {
            bin += bitset<8>(_char).to_string();
        }
    }
    else
    {
        reverse(s.begin(), s.end());
        for (char& _char : s)
        {
            bin += bitset<8>(_char).to_string();
        }
    }

    int lbitow = bin.length();

    vector<int> binary;

    for (int i = 0; i < lbitow; i++)
    {
        if (bin[i] == '0')
            binary.push_back(0);
        else
            binary.push_back(1);
    }

    return binary;

}

string binary2string(string bin)
{
    string text = "";
    stringstream sstream(bin);
    while (sstream.good())
    {
        bitset<8> bits;
        sstream >> bits;
        text += char(bits.to_ulong());
    }
    return text;
}


vector<int> hammingcode(vector<int> bin)
{
    int lbitow = bin.size();
    int lpaczek = lbitow / 4;
    vector<vector<int>> wektorpaczek4bit, wektorpaczek7bit;
    vector<int> paczka;
    int tmp = 0;

    //paczki 4-bitowe
    for (int i = 0; i < lpaczek; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            paczka.push_back(bin[tmp]);
            tmp++;
        }
        wektorpaczek4bit.push_back(paczka);
        paczka.clear();
    }

    //paczki 7-bitowe
    int p1, p2, p3;
    int d1, d2, d3, d4;

    for (int i = 0; i < lpaczek; i++)
    {
        d1 = wektorpaczek4bit[i][0];
        d2 = wektorpaczek4bit[i][1];
        d3 = wektorpaczek4bit[i][2];
        d4 = wektorpaczek4bit[i][3];

        p1 = (d1 + d2 + d4) % 2;
        p2 = (d1 + d3 + d4) % 2;
        p3 = (d2 + d3 + d4) % 2;

        paczka.push_back(p1);
        paczka.push_back(p2);
        paczka.push_back(d1);
        paczka.push_back(p3);
        paczka.push_back(d2);
        paczka.push_back(d3);
        paczka.push_back(d4);

        wektorpaczek7bit.push_back(paczka);
        paczka.clear();
    }

    vector<int> hamming;

    for (int i = 0; i < lpaczek; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            hamming.push_back(wektorpaczek7bit[i][j]);
        }
    }

    return hamming;
}

vector<int> hammingdecoder(vector<int> hamming)
{

    int lbitow = hamming.size();
    int lpaczek = lbitow / 7;
    vector<vector<int>> wektorpaczek7bit, wektorpaczek4bit;
    vector<int> paczka;
    int tmp = 0;

    for (int i = 0; i < lpaczek; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            paczka.push_back(hamming[tmp]);
            tmp++;
        }
        wektorpaczek7bit.push_back(paczka);
        paczka.clear();
    }


    int h1, h2, h3, h4, h5, h6, h7;
    int p1, p2, p3;
    int n; //indeks korekty

    for (int i = 0; i < lpaczek; i++)
    {
        h1 = wektorpaczek7bit[i][0];
        h2 = wektorpaczek7bit[i][1];
        h3 = wektorpaczek7bit[i][2];
        h4 = wektorpaczek7bit[i][3];
        h5 = wektorpaczek7bit[i][4];
        h6 = wektorpaczek7bit[i][5];
        h7 = wektorpaczek7bit[i][6];

        p1 = (h1 + h3 + h5 + h7) % 2;
        p2 = (h2 + h3 + h6 + h7) % 2;
        p3 = (h4 + h5 + h6 + h7) % 2;

        n = p1 + (p2 * 2) + (p3 * 4);

        if (n > 0)
        {
            cout << "Naprawianie bitu " << n << endl;
            if (wektorpaczek7bit[i][n - 1] == 0)
                wektorpaczek7bit[i][n - 1] = 1;
            else
                wektorpaczek7bit[i][n - 1] = 0;
        }

        paczka.push_back(wektorpaczek7bit[i][2]);
        paczka.push_back(wektorpaczek7bit[i][4]);
        paczka.push_back(wektorpaczek7bit[i][5]);
        paczka.push_back(wektorpaczek7bit[i][6]);

        wektorpaczek4bit.push_back(paczka);
        paczka.clear();

    }

    vector<int> dekoder;

    for (int i = 0; i < lpaczek; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            dekoder.push_back(wektorpaczek4bit[i][j]);
        }
    }

    return dekoder;
}

vector<float> noise(int q, int size)
{
    vector<float> noise;
    const static float c1 = (1 << q) - 1;
    const static float c2 = ((int)(c1 / 3)) + 1;
    const static float c3 = 1.f / c1;

    float random = 0.f;
    float szum = 0.f;

    for (int i = 0; i < size; i++)
    {
        random = ((float)rand() / (float)(RAND_MAX + 1));
        szum = (2.f * ((random * c2) + (random * c2) + (random * c2)) - 3.f * (c2 - 1.f)) * c3;
        noise.push_back(szum);
    }

    return noise;
}


int main()
{
    string test = "ala";
    vector<int> bin = string2binary(test, 1);

    cout << endl << "1. Dane wejsciowe: ";
    cout << test << endl;

    for (int i = 0; i < bin.size(); i++)
    {
        cout << bin[i];
    }

    cout << endl << endl;

    bin = hammingcode(bin);
    int dlugosc = bin.size();

    cout << "2. Dane zabezpieczone kodem Hamming(7,4):     " << endl;

    for (int i = 0; i < dlugosc; i++)
    {
        cout << bin[i];
    }
    cout << endl;

    float lprobek = 200; //ilość próbek na bit
    float Tb = 0.1; //czas trwania jednego bitu
    float dt = Tb / lprobek; //delta czasu
    float fs = 1 / dt;
    float size = lprobek * dlugosc; //ilość próbek

    int* probki = new int[size];
    float* czas = new float[size];
    int n = 0;

    ofstream plik1;
    plik1.open("TTL.csv");

    for (int i = 0; i < dlugosc; i++)
    {
        if (bin[i] == 1)
        {
            for (int j = 0; j < lprobek; j++)
            {
                probki[n] = 1;
                czas[n] = dt * n;
                plik1 << " " << czas[n] << "," << " " << probki[n] << endl;
                n++;
            }
        }
        else if (bin[i] == 0)
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
    float A1 = 0.0;
    float A2 = 1.0;
    float f = N / Tb;

    //ASK
    float* ASK = new float[size];
    float* sASK = new float[size]; //sygnał nośny dla ASK
    float* xASK = new float[size]; //przemnożony sygnał zmodulowany przez sygnał nośny
    float* pASK = new float[size]; //funkcja całkowa dla ASK
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

        sASK[i] = A2 * sin(2 * pi * f * czas[i]);

        xASK[i] = ASK[i] * sASK[i];
    }

    plik2.close();


    // WIDMO ASK
    //delta czasu - 0.0005
    //fs = 1/0.0005 = 2000

    vector<float> czestotliwosc;
    n = 0;
    float fk = 0.0; //skala częstotliwości
    vector<float> widmoASK;
    ofstream widmo1;
    widmo1.open("widmoASK.csv");
    while (n < size)
    {
        widmoASK.push_back(dft(size, n, ASK));
        if (n <= (size / 2))
            widmo1 << " " << fk << "," << " " << widmoASK[n] << endl;
        n++;
        czestotliwosc.push_back(fk);
        fk = n * fs / size;
    }

    widmo1.close();


    pASK[0] = 0.0;

    n = 1;

    for (int i = 0; i < dlugosc; i++)
    {
        for (int j = 1; j < lprobek; j++)
        {
            pASK[n] = pASK[n - 1] + xASK[n];
            n++;
        }
        if (n == size) break;
        pASK[n] = 0.0;
        n++;
    }


    //FSK
    float f1 = N / (2 * Tb);
    float f2 = N / Tb;
    float* FSK = new float[size];
    float* sFSK1 = new float[size]; //sygnał nośny dla FSK zera binarne
    float* sFSK2 = new float[size]; //sygnał nośny dla FSK jedynki binarne
    float* xFSK1 = new float[size]; //przemnożony sygnał nośny przez zmodulowany zera
    float* xFSK2 = new float[size]; //przemnożony jedynki
    float* pFSK1 = new float[size]; //funkcja całkowa dla FSK zera
    float* pFSK2 = new float[size]; //funkcja całkowa dla FSK jedynki
    float* pFSK = new float[size]; //funkcja całkowa ostateczna

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
        sFSK1[i] = A * sin(2 * pi * f1 * czas[i]);

        sFSK2[i] = A * sin(2 * pi * f2 * czas[i]);

        xFSK1[i] = FSK[i] * sFSK1[i];

        xFSK2[i] = FSK[i] * sFSK2[i];
    }

    plik3.close();

    // WIDMO FSK

    n = 0;
    vector<float> widmoFSK;
    ofstream widmo2;
    widmo2.open("widmoFSK.csv");
    while (n < size)
    {
        widmoFSK.push_back(dft(size, n, FSK));
        if (n <= (size / 2))
            widmo2 << " " << czestotliwosc[n] << "," << " " << widmoFSK[n] << endl;
        n++;
    }

    widmo2.close();


    pFSK1[0] = 0.0;
    pFSK2[0] = 0.0;
    pFSK[0] = 0.0;

    n = 1;

    for (int i = 0; i < dlugosc; i++)
    {
        for (int j = 1; j < lprobek; j++)
        {
            pFSK1[n] = pFSK1[n - 1] + xFSK1[n];
            pFSK2[n] = pFSK2[n - 1] + xFSK2[n];
            pFSK[n] = pFSK2[n] - pFSK1[n];
            n++;
        }
        if (n == size) break;
        pFSK1[n] = 0.0;
        pFSK2[n] = 0.0;
        pFSK[n] = 0.0;

        n++;
    }


    //PSK
    float fi1 = 0.0;
    float fi2 = pi;
    float* PSK = new float[size];
    float* sPSK = new float[size]; //sygnał nośny dla PSK
    float* xPSK = new float[size]; //przemnożony
    float* pPSK = new float[size]; //funkcja całkowa dla PSK
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
        sPSK[i] = A * sin(2 * pi * f * czas[i] + fi2);
        xPSK[i] = PSK[i] * sPSK[i];
    }

    plik4.close();

    // WIDMO PSK

    n = 0;
    vector<float> widmoPSK;
    ofstream widmo3;
    widmo3.open("widmoPSK.csv");
    while (n < size)
    {
        widmoPSK.push_back(dft(size, n, PSK));
        if (n <= (size / 2))
            widmo3 << " " << czestotliwosc[n] << "," << " " << widmoPSK[n] << endl;
        n++;
    }

    widmo3.close();


    pPSK[0] = 0.0;

    n = 1;

    for (int i = 0; i < dlugosc; i++)
    {
        for (int j = 1; j < lprobek; j++)
        {
            pPSK[n] = pPSK[n - 1] + xPSK[n];
            n++;
        }
        if (n == size) break;
        pPSK[n] = 0.0;
        n++;
    }


    // szum

    float alfa = 0.5;
    int q = 15;
    vector<float> szum = noise(q, size);
    vector<float> zaszumionyASK, zaszumionyFSK, zaszumionyPSK;
    ofstream szum1, szum2, szum3;
    szum1.open("szumASK.csv");
    szum2.open("szumFSK.csv");
    szum3.open("szumPSK.csv");

    for (int i = 0; i < size; i++)
    {
        zaszumionyASK.push_back((ASK[i] * alfa) + (szum[i] * (1.0 - alfa)));
        zaszumionyFSK.push_back((FSK[i] * alfa) + (szum[i] * (1.0 - alfa)));
        zaszumionyPSK.push_back((PSK[i] * alfa) + (szum[i] * (1.0 - alfa)));
        szum1 << " " << czas[i] << "," << " " << zaszumionyASK[i] << endl;
        szum2 << " " << czas[i] << "," << " " << zaszumionyFSK[i] << endl;
        szum3 << " " << czas[i] << "," << " " << zaszumionyPSK[i] << endl;
    }

    szum1.close();
    szum2.close();
    szum3.close();

    // widmo zaszumionych sygnałów

    n = 0;
    vector<float> widmoASKposzumie, widmoFSKposzumie, widmoPSKposzumie;
    ofstream widmo4, widmo5, widmo6;
    widmo4.open("widmoASKposzumie.csv");
    widmo5.open("widmoFSKposzumie.csv");
    widmo6.open("widmoPSKposzumie.csv");

    while (n < size)
    {
        widmoASKposzumie.push_back(dftvector(size, n, zaszumionyASK));
        widmoFSKposzumie.push_back(dftvector(size, n, zaszumionyFSK));
        widmoPSKposzumie.push_back(dftvector(size, n, zaszumionyPSK));
        if (n <= (size / 2))
        {
            widmo4 << " " << czestotliwosc[n] << "," << " " << widmoASKposzumie[n] << endl;
            widmo5 << " " << czestotliwosc[n] << "," << " " << widmoFSKposzumie[n] << endl;
            widmo6 << " " << czestotliwosc[n] << "," << " " << widmoPSKposzumie[n] << endl;
        }

        n++;
    }

    widmo4.close();
    widmo5.close();
    widmo6.close();


    //demodulacja

    //ASK
    vector<int> mASK; //zdemodulowany sygnał
    float h = 0.04;

    for (int i = 0; i < size; i++)
    {
        if (zaszumionyASK[i] >= h)
            mASK.push_back(1);
        else
            mASK.push_back(0);
    }

    // FSK
    vector<int> mFSK; //zdemodulowany sygnał
    h = 0.3;

    for (int i = 0; i < size; i++)
    {
        if (zaszumionyFSK[i] >= h)
            mFSK.push_back(1);
        else
            mFSK.push_back(0);
    }

    //PSK
    vector<int> mPSK; //zdemodulowany sygnał
    h = 0.04;

    for (int i = 0; i < size; i++)
    {
        if (zaszumionyPSK[i] >= h)
            mPSK.push_back(1);
        else
            mPSK.push_back(0);
    }



    vector<int> demodulacjaASK, demodulacjaFSK, demodulacjaPSK;

    n = 4;
    for (int i = 0; i < size; i++)
    {
        if (mASK[n] == 0)
        {
            demodulacjaASK.push_back(0);
            n = n + lprobek;
        }
        else if (mASK[n] == 1)
        {
            demodulacjaASK.push_back(1);
            n = n + lprobek;
        }
        if (n > size) break;
    }

    n = 150;

    for (int i = 0; i < size; i++)
    {
        if (mFSK[n] == 0)
        {
            demodulacjaFSK.push_back(0);
            n = n + lprobek;
        }
        else if (mFSK[n] == 1)
        {
            demodulacjaFSK.push_back(1);
            n = n + lprobek;
        }
        if (n > size) break;
    }

    n = 50;

    for (int i = 0; i < size; i++)
    {
        if (mPSK[n] == 0)
        {
            demodulacjaPSK.push_back(0);
            n = n + lprobek;
        }
        else if (mPSK[n] == 1)
        {
            demodulacjaPSK.push_back(1);
            n = n + lprobek;
        }
        if (n > size) break;
    }


    cout << endl << "3. Modulacja ASK/FSK/PSK (wykresy) + szum" << endl;
    cout << endl << "4. Zdemodulowany sygnal ASK zabezpieczony kodem kanalowym:" << endl;

    for (int i = 0; i < demodulacjaASK.size(); i++)
        cout << demodulacjaASK[i];

    cout << endl;
    cout << endl << "Zdemodulowany sygnal FSK zabezpieczony kodem kanalowym:" << endl;

    for (int i = 0; i < demodulacjaFSK.size(); i++)
        cout << demodulacjaFSK[i];

    cout << endl << endl << "Zdemodulowany sygnal PSK zabezpieczony kodem kanalowym:" << endl;

    for (int i = 0; i < demodulacjaPSK.size(); i++)
        cout << demodulacjaPSK[i];

    cout << endl << endl;


    vector<int> dekoder1 = hammingdecoder(demodulacjaASK);
    vector<int> dekoder2 = hammingdecoder(demodulacjaFSK);
    vector<int> dekoder3 = hammingdecoder(demodulacjaPSK);

    cout << endl << endl << "5. Zdekodowany strumien binarny (ASK):" << endl;

    int licznik = 0;
    for (int i = 0; i < dekoder1.size(); i++)
    {
        if (bin[i] != dekoder1[i])
            licznik++;
        
        cout << dekoder1[i];
    }
    //wskaźnik BER
    cout << endl << "Wskaznik BER: " << licznik << "/" << dekoder1.size()  << endl;


    string binaryWord1 = "";

    for (int i = 0; i < dekoder1.size(); i++)
    {
        if (dekoder1[i] == 0)
            binaryWord1.push_back('0');
        else if (dekoder1[i] == 1)
            binaryWord1.push_back('1');
    }

    string zdanie1 = binary2string(binaryWord1);

    cout << endl;
    cout << zdanie1 << endl;

    cout << endl << "Zdekodowany strumien binarny (FSK):" << endl;

    licznik = 0;
    for (int i = 0; i < dekoder2.size(); i++)
    {
        if (bin[i] != dekoder2[i])
            licznik++;
        cout << dekoder2[i];
    }

    cout << endl << "Wskaznik BER: " << licznik << "/" << dekoder2.size()  << endl;

    string binaryWord2 = "";

    for (int i = 0; i < dekoder2.size(); i++)
    {
        if (dekoder2[i] == 0)
            binaryWord2.push_back('0');
        else if (dekoder2[i] == 1)
            binaryWord2.push_back('1');
    }

    string zdanie2 = binary2string(binaryWord2);

    cout << endl;
    cout << zdanie2 << endl;


    cout << endl << "Zdekodowany strumien binarny (PSK):" << endl;

    licznik = 0;
    for (int i = 0; i < dekoder3.size(); i++)
    {
        if (bin[i] != dekoder3[i])
            licznik++;
        cout << dekoder3[i];
    }
    cout << endl << "Wskaznik BER: " << licznik << "/" << dekoder3.size()  << endl;

    string binaryWord3 = "";

    for (int i = 0; i < dekoder3.size(); i++)
    {
        if (dekoder3[i] == 0)
            binaryWord3.push_back('0');
        else if (dekoder3[i] == 1)
            binaryWord3.push_back('1');
    }

    string zdanie3 = binary2string(binaryWord3);

    cout << endl;
    cout << zdanie3 << endl << endl;


    // BER - różnica bitów w sygnale wejściowym i zdekodowanym, ile bitów zepsutych, i podzielić przez
    // całkowitą liczbę odzyskanych bitów w kroku 5
    // trzeba zdemodulować zaszumione sygnały


    delete[] probki;
    delete[] czas;

    delete[] ASK;
    delete[] sASK;
    delete[] xASK;
    delete[] pASK;
   // delete[] mASK;

    delete[] FSK;
    delete[] sFSK1;
    delete[] sFSK2;
    delete[] xFSK1;
    delete[] xFSK2;
    delete[] pFSK1;
    delete[] pFSK2;
    delete[] pFSK;


    delete[] PSK;
    delete[] sPSK;
    delete[] xPSK;
    delete[] pPSK;
   // delete[] mPSK;

    return 0;
}
