#include <iostream>
#include <fstream>
#define _USE_MATH_DEFINES
#define M_PI 
#include <cmath>

using namespace std;

int A = 3;
int B = 1;
int C = 3;
float pi = atan(1) * 4;

//zad1

void f_kwadratowa()
{
    float delta;
    float t1, t2;
    delta = B ^ 2 - 4 * A * C;
    if (delta < 0) cout << "Brak miejsc zerowych" << endl;
    else if (delta == 0)
    {
        t1 = -(B / 2 * A);
        cout << "Jedno miejsce zerowe: " <<t1<< endl;
    }
    else
    {
        t1 = (-B + pow(delta, 0.5)) / (2 * A);
        t2 = (-B - pow(delta, 0.5)) / (2 * A);
        cout << "Dwa miejsca zerowe: " << t1 << " i "<<t2<< endl;
      
    }
}

float wykres_fkwadratowa(float t)
{
    return A * pow(t,2) + B * t + C;
}

//zad2

float funkcja1(float t)
{
    return 2 * pow(wykres_fkwadratowa(t), 2) + 12 * cos(t);
}

float funkcja2(float t)
{
    return sin(2 * pi * 7 * t) * wykres_fkwadratowa(t) - 0.2 * log10(abs(funkcja1(t) + pi));
}

float funkcja3(float t)
{
    return sqrt(abs(funkcja1(t) * funkcja1(t) * funkcja2(t))) - 1.8 * sin(0.4 * t * funkcja2(t) * funkcja1(t));
}

float funkcja4(float t)
{
    if (t < 0.22 && t >= 0)
        return (1 - 7 * t) * sin((2 * pi * t * 10) / (t + 0.04));
    
    if (t >= 0.22 && t < 0.7)
        return 0.63 * t * sin(125 * t);
    
    if (t <= 1.0 && t >= 0.7)
        return pow(1/t, 0.662) + 0.77 * sin(8 * t);
}

float funkcja5(float t)
{
    float wyn1, wyn2 = 0.0;
    for (int i = 1; i <= 2; i++)
    {
        wyn1 = (cos(12 * t * pow(i, 2)) + cos(16 * t * i)) / pow(i, 2);
        wyn2 = wyn2 + wyn1;
    }
    return wyn2;
}

float funkcja6(float t)
{
    float wyn1, wyn2 = 0.0;
    for (int i = 1; i <= 4; i++)
    {
        wyn1 = (cos(12 * t * pow(i, 2)) + cos(16 * t * i)) / pow(i, 2);
        wyn2 = wyn2 + wyn1;
    }
    return wyn2;
}

float funkcja7(float t)
{
    float wyn1, wyn2 = 0.0;
    for (int i = 1; i <= 31; i++)
    {
        wyn1 = (cos(12 * t * pow(i, 2)) + cos(16 * t * i)) / pow(i, 2);
        wyn2 = wyn2 + wyn1;
    }
    return wyn2;
}

int main()
{
   // zad1
    f_kwadratowa();

    float t0 = -10;
    float tn = t0;
    float dt = 1.0 / 100;
    int n = 1;

    ofstream wyk1;
    wyk1.open("wykres.csv");


    while (tn <= 10)
    {
        wyk1 <<" "<<tn<<","<< " "<<wykres_fkwadratowa(tn) << endl;
        tn = t0 + n * dt;
        n++;
    }

    wyk1.close();


    // zad2

    // y(t)
    t0 = 0;
    tn = t0;
    dt = 1.0 / 22050;
    n = 1;

    ofstream wyk2;
    wyk2.open("wykres1.csv");


    while (tn <= 1)
    {
        wyk2 << " " << tn << "," << " " << funkcja1(tn) << endl;
        tn = t0 + n * dt;
        n++;
    }

    wyk2.close();

    // z(t)
    t0 = 0;
    tn = t0;
    dt = 1.0 / 22050;
    n = 1;

    wyk2.open("wykres2.csv");


    while (tn <= 1)
    {
        wyk2 << " " << tn << "," << " " << funkcja2(tn) << endl;
        tn = t0 + n * dt;
        n++;
    }

    wyk2.close();

    // u(t)
    t0 = 0;
    tn = t0;
    dt = 1.0 / 22050;
    n = 1;

    wyk2.open("wykres3.csv");


    while (tn <= 1)
    {
        wyk2 << " " << tn << "," << " " << funkcja3(tn) << endl;
        tn = t0 + n * dt;
        n++;
    }

    wyk2.close();

    // v(t)
    t0 = 0;
    tn = t0;
    dt = 1.0 / 22050;
    n = 1;

    wyk2.open("wykres4.csv");


    while (tn <= 1)
    {
        wyk2 << " " << tn << "," << " " << funkcja4(tn) << endl;
        tn = t0 + n * dt;
        n++;
    }

    wyk2.close();

    // p(t)

    //1
    t0 = 0;
    tn = t0;
    dt = 1.0 / 22050;
    n = 1;

    wyk2.open("wykres5.csv");


    while (tn <= 1)
    {
        wyk2 << " " << tn << "," << " " << funkcja5(tn) << endl;
        tn = t0 + n * dt;
        n++;
    }

    wyk2.close();

    // 2
    t0 = 0;
    tn = t0;
    dt = 1.0 / 22050;
    n = 1;

    wyk2.open("wykres6.csv");


    while (tn <= 1)
    {
        wyk2 << " " << tn << "," << " " << funkcja6(tn) << endl;
        tn = t0 + n * dt;
        n++;
    }

    wyk2.close();

    // 3
    t0 = 0;
    tn = t0;
    dt = 1.0 / 22050;
    n = 1;

   
    wyk2.open("wykres7.csv");


    while (tn <= 1)
    {
        wyk2 << " " << tn << "," << " " << funkcja7(tn) << endl;
        tn = t0 + n * dt;
        n++;
    }

    wyk2.close();

    return 0;
}


