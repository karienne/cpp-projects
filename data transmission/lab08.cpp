/* Działanie programu:
1. strumień wejściowy:                 0110|0001 (litera 'a')
2. strumień zakodowany kodem Hamminga: 1100110|1101001
3. kod Hamminga po zepsuciu bitu 7 w pierwszym pakiecie i bitu 2 w drugim: 1100111|1001001
4. zdekodowany kod Hamminga (z naprawieniem wadliwych bitów): 01100001

5. strumień wejściowy zakodowany ulepszonym kodem Hamminga: 11001100|11010010
6. zepsucie bitów 7,6 w pierwszym pakiecie i 4,3 w drugim: 11001010|11100010
7. p4 w pierwszym kroku było zgodne z tym w pakiecie nadawcy
8. wyliczony indeks korekty n=1
9. po negacji bitu 1 ponowna weryfikacja p4, jednak jest ono różne od tego z pakietu nadawcy (komunikat o 2 błędnych bitach)
*/



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
        return bin;
    }
    else
    {
        reverse(s.begin(), s.end());
        for (char& _char : s)
        {
            bin += bitset<8>(_char).to_string();
        }
        return bin;
    }
}

vector<vector<int>> hammingcode(vector<int> bin) //zwraca wektor 7-bitowych wektorów
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

    return wektorpaczek7bit;
}

vector<vector<int>> hammingdecoder(vector<vector<int>> hamming) //przyjmuje wektor wektorów 7 bitowych, zwraca wektor wektorów 4-bitowych
{
    vector<vector<int>> wektorpaczek4bit;
    vector<int> paczka;

    int lpaczek = hamming.size();

    int h1, h2, h3, h4, h5, h6, h7;
    int p1, p2, p3;
    int n; //indeks korekty

    for (int i = 0; i < lpaczek; i++) 
    {
        h1 = hamming[i][0];
        h2 = hamming[i][1];
        h3 = hamming[i][2];
        h4 = hamming[i][3];
        h5 = hamming[i][4];
        h6 = hamming[i][5];
        h7 = hamming[i][6];

        p1 = (h1 + h3 + h5 + h7) % 2;
        p2 = (h2 + h3 + h6 + h7) % 2;
        p3 = (h4 + h5 + h6 + h7) % 2;

        n = p1 + (p2 * 2) + (p3 * 4);

        if (n > 0)
        {
            cout << "Naprawianie bitu " << n << endl;
            if (hamming[i][n - 1] == 0)
                hamming[i][n - 1] = 1;
            else
                hamming[i][n - 1] = 0;
        }
        else
            cout << "Brak bitu do naprawy" << endl;
        
        paczka.push_back(hamming[i][2]);
        paczka.push_back(hamming[i][4]);
        paczka.push_back(hamming[i][5]);
        paczka.push_back(hamming[i][6]);

        wektorpaczek4bit.push_back(paczka);
        paczka.clear();
        
    }

    return wektorpaczek4bit;
}

vector<vector<int>> SECDED(vector<int> bin) //zwraca wektor wektorów 8-bitowych
{
    int lbitow = bin.size();
    int lpaczek = lbitow / 4;
    vector<vector<int>> wektorpaczek4bit, wektorpaczek8bit;
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

    //paczki 8-bitowe
    int p1, p2, p3, p4;
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
        p4 = (p1 + p2 + p3 + d1 + d2 + d3 + d4) % 2;

        paczka.push_back(p1);
        paczka.push_back(p2);
        paczka.push_back(d1);
        paczka.push_back(p3);
        paczka.push_back(d2);
        paczka.push_back(d3);
        paczka.push_back(d4);
        paczka.push_back(p4);

        wektorpaczek8bit.push_back(paczka);
        paczka.clear();
    }

    return wektorpaczek8bit;
}

vector<vector<int>> SECDEDdekoder(vector<vector<int>> secded)
{
    vector<vector<int>> wektorpaczek4bit;
    vector<int> paczka;

    int lpaczek = secded.size();

    int h1, h2, h3, h4, h5, h6, h7, h8;
    int p1, p2, p3, p4;
    int n; //indeks korekty

    for (int i = 0; i < lpaczek; i++)
    {
        h1 = secded[i][0];
        h2 = secded[i][1];
        h3 = secded[i][2];
        h4 = secded[i][3];
        h5 = secded[i][4];
        h6 = secded[i][5];
        h7 = secded[i][6];
        h8 = secded[i][7];

        //weryfikacja p4:
        p4 = (h1 + h2 + h3 + h4 + h5 + h6 + h7) % 2;

        if (p4 != h8)
            cout << "Co najmniej jeden bit zostal zepsuty (50% szans na naprawe pakietu)" << endl;

        p1 = (h1 + h3 + h5 + h7) % 2;
        p2 = (h2 + h3 + h6 + h7) % 2;
        p3 = (h4 + h5 + h6 + h7) % 2;

        n = p1 + (p2 * 2) + (p3 * 4);

        if (n > 0)
        {
            cout << "Naprawianie bitu " << n << endl;
            if (secded[i][n - 1] == 0)
                secded[i][n - 1] = 1;
            else
                secded[i][n - 1] = 0;
        }
        else
            cout << "Brak bitu do naprawy" << endl;

        h1 = secded[i][0];
        h2 = secded[i][1];
        h3 = secded[i][2];
        h4 = secded[i][3];
        h5 = secded[i][4];
        h6 = secded[i][5];
        h7 = secded[i][6];
        h8 = secded[i][7];

        //ponowna weryfikacja p4
        p4 = (h1 + h2 + h3 + h4 + h5 + h6 + h7) % 2;

        if (p4 == h8)
        {
            cout << "W pakiecie byl tylko 1 uszkodzony bit" << endl;
            paczka.push_back(h3);
            paczka.push_back(h5);
            paczka.push_back(h6);
            paczka.push_back(h7);

            wektorpaczek4bit.push_back(paczka);
            paczka.clear();
        }
        else
        {
            cout << "W pakiecie byly co najmniej 2 uszkodzone bity. Prosba o ponowna transmisje" << endl;
            return secded;
        }

    }

    return wektorpaczek4bit;
}


vector<int> negacjabitu(vector<int> strumien, int n) //n- indeks bitu do negacji
{
    if (n > 0)
    {
        if (strumien[n - 1] == 0)
        {
            strumien[n - 1] = 1;
            cout << "Bit nr " << n << " zanegowany" << endl;
        }

        else if (strumien[n - 1] == 1)
        {
            strumien[n - 1] = 0;
            cout << "Bit nr " << n << " zanegowany" << endl;
        }
        else
            cout << "Nieprawidlowy strumien" << endl;
    }

    else
        cout << "Niepoprawny nr bitu" << endl;

    return strumien;
}

int main()
{
    string test = "a";
    string bin = string2binary(test, 1); 
    int lbitow= bin.length();

    //przepis ze stringa na wektor intów:
    vector<int> binary;

    cout << "Strumien wejsciowy:    ";

    for (int i = 0; i < lbitow; i++)
    {
        if (bin[i] == '0')
            binary.push_back(0);
        else
            binary.push_back(1);
        cout << binary[i];
    }
    cout << endl;

    //Kod Hamminga(7,4)
    vector<vector<int>> kodHamminga = hammingcode(binary);

    cout << "Kod Hamminga:          ";

    for (int i = 0; i < kodHamminga.size(); i++)
    {
        for (int j = 0; j < 7; j++)
        {
            cout << kodHamminga[i][j];
        }
        
    }
    cout << endl;

    //psucie bitu
    int n;

    for (int i = 0; i < kodHamminga.size(); i++)
    {
        n = (rand() % 7) + 1;
        kodHamminga[i] = negacjabitu(kodHamminga[i], n);
    }

    cout << "Kod Hamminga po 'zepsuciu' bitu:  ";

    for (int i = 0; i < kodHamminga.size(); i++)
    {
        for (int j = 0; j < 7; j++)
        {
            cout << kodHamminga[i][j];
        }
    }
    cout << endl;

    vector<vector<int>> dekoderHam = hammingdecoder(kodHamminga);

    cout << "Zdekodowany kod Hamminga:    ";

    for (int i = 0; i < dekoderHam.size(); i++)
    {
        for (int j = 0; j < 4; j++)
        {
            cout << dekoderHam[i][j];
        }
    }
    cout << endl;


    //Kod Hamminga- wersja SECDED
    vector<vector<int>> kodSECDED = SECDED(binary);

    cout << "Kod Hamminga SECDED:          ";

    for (int i = 0; i < kodSECDED.size(); i++)
    {
        for (int j = 0; j < 8; j++)
        {
            cout << kodSECDED[i][j];
        }
    }
    cout << endl;

    //psucie 2 bitów
    int n2;

    for (int i = 0; i < kodSECDED.size(); i++)
    {
        n = (rand() % 7) + 1;
        do{
            n2 = (rand() % 7) + 1;
        } while (n2 == n);
        kodSECDED[i] = negacjabitu(kodSECDED[i], n);
        kodSECDED[i] = negacjabitu(kodSECDED[i], n2);
    }
    cout << "Kod SECDED po negacjach bitow:    ";
    for (int i = 0; i < kodSECDED.size(); i++)
    {
        for (int j = 0; j < 8; j++)
        {
            cout << kodSECDED[i][j];
        }
    }
    cout << endl;


    vector<vector<int>> dekoderSECDED = SECDEDdekoder(kodSECDED);


    /*cout << "Zdekodowany kod Hamminga SECDED:    ";

    for (int i = 0; i < dekoderSECDED.size(); i++)
    {
        for (int j = 0; j < 4; j++)
        {
            cout << dekoderSECDED[i][j];
        }
    }
    cout << endl;*/



    return 0;



}

/*  0-1 p
    1-2 p
    2-3
    3-4 p
    4-5
    5-6
    6-7
    ---
    7-8 p
*/

// dzielimy bajt na dwie paczki 4-bitowe (1-4) o nazwie d jako wektory transponowane
// upychamy 4 bitowe paczki w dwóch strukturach 7-bitowych, gdzie na bitach o indeksach które są potęgami liczby 2 będą
// bity parzystości a na pozostałych bitach będą kolejno upakowane bity danych
// bity parzystości obliczamy jako sumę modulo konkretnych wskazanych przez diagram danych bitów danych
// z dwóch paczek 4-bitowych robimy dwie paczki 7-bitowe (albo przekazujemy cały ciąg który dopiero potem rozdzielamy na struktury 7-bitowe)
// za pomocą bitów parzystości z 7-bitowej paczki obliczamy indeks korekty (syndrom) :
// n= p1*2^0 + p2*s^1 + p3*2^2  jeżeli n=0 tzn że wszystkie bity danych przyszły prawidłowo
// wskazany tym indeksem bit trzeba zanegować, czyli skorygować
// finalny wektor danych (po korekcie): d'=[h3,h5,h6,h7]
