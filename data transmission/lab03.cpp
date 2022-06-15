#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm> 
#include <complex>

using namespace std;
typedef complex<float> liczba_zesp;

int A = 3;
int B = 1;
int C = 3;
float pi = atan(1) * 4;

float ton_prosty(float a, float f, float fi, float t)
{
	return a * sin(2*pi*f*t+fi);
}

float dft(vector<float> tab, int n, int obieg) //n- liczba próbek
{
	float re = 0;
	float im = 0;
	float wyn = 0;
	
	for (int i = 0; i < n; i++)
	{
		re = re + (tab[i] * cos((-2 * pi * i * obieg) / n));
		im = im + (tab[i] * sin((-2 * pi * i * obieg) / n));
	}
	
	wyn = sqrt(pow(re, 2) + pow(im, 2)); //widmo amplitudowe
	return wyn;
}

liczba_zesp dft2(vector<float> tab, int n, int obieg)
{
	float re = 0;
	float im = 0;
	liczba_zesp wyn;

	for (int i = 0; i < n; i++)
	{
		re = re + (tab[i] * cos((-2 * pi * i * obieg) / n));
		im = im + (tab[i] * sin((-2 * pi * i * obieg) / n));	
	}

	wyn = liczba_zesp(re, im);

	return wyn;
}


float idft(vector<liczba_zesp> tab, int n, int obieg) //n-liczba próbek
{
	float re = 0;
	float im = 0;
	float wyn = 0;
	for (int i = 0; i < n; i++)
	{
		re = re + (tab[i].real() * cos((2 * pi * i * obieg) / n));
		im = im - (tab[i].imag() * sin((2 * pi * i * obieg) / n));
	}

	if (re >= im)
		wyn = sqrt(pow(re - im, 2)) / n;
	else
		wyn = -sqrt(pow(re - im, 2)) / n;

	return wyn;
}

float fun1(float t)
{
	return A * pow(t, 2) + B * t + C;
}

float fun2(float t)
{
	return 2 * pow(fun1(t), 2) + 12 * cos(t);
}

float fun3(float t)
{
	return sin(2 * pi * 7 * t) * fun1(t) - 0.2 * log10(abs(fun2(t) + pi));
}

float fun4(float t)
{
	return sqrt(abs(fun2(t) * fun2(t) * fun3(t))) - 1.8 * sin(0.4 * t * fun3(t) * fun2(t));
}

float fun5(float t)
{
	if (t < 0.22 && t >= 0)
		return (1 - 7 * t) * sin((2 * pi * t * 10) / (t + 0.04));

	if (t >= 0.22 && t < 0.7)
		return 0.63 * t * sin(125 * t);

	if (t <= 1.0 && t >= 0.7)
		return pow(1 / t, 0.662) + 0.77 * sin(8 * t);
}

float fun6(float t)
{
	float wyn1, wyn2 = 0.0;
	for (int i = 1; i <= (10*A*B); i++)
	{
		wyn1 = (cos(12 * t * pow(i, 2)) + cos(16 * t * i)) / pow(i, 2);
		wyn2 = wyn2 + wyn1;
	}
	return wyn2;
}


int main(int argc, char** argv)
{
	//Użyj funkcji z poprzednich zajęć i wyznacz dyskretny sygnał tonu prostego;
	//Wygeneruj wykres dla n należącego do <0; ABC>

	float max = 100 * A + 10 * B + C;
	float a = 1.0;
	float f = B;
	float fi = C * pi;
	float fs = 400;
	float t0 = 0;
	float dt = 1.0 / fs;
	float n = 0;
	float tn = t0;
	vector<float> tprosty;

	ofstream plikk;
	plikk.open("wykres1.csv"); //wykres tonu prostego

	while (n < max)
	{
		tprosty.push_back(ton_prosty(a, f, fi, tn));
		plikk << " " << tn << "," << " " << tprosty[n] << endl;
		n++;
		tn = t0 + n * dt;
	}
	plikk.close();


	//Oblicz widmo amplitudowe 

	float fk = 0; //skala częstotliwości
	vector<float> amplitudowe;

	ofstream plikk2;
	plikk2.open("wykres2.csv"); //wykres widma amplitudowego

	for (int i = 0; i <= tprosty.size()/2+1; i++) //skala w dft jest symetryczna, dlatego podajemy połowę próbek
	{
		fk = i * fs / tprosty.size(); //size=313
		amplitudowe.push_back(dft(tprosty, tprosty.size(), i));
		plikk2 << fk << " " << "," << " " << amplitudowe[i] << endl;
	}

	plikk2.close();

	//Wartość amplitudy przedstawić w skali decybelowej

	ofstream plik3;
	plik3.open("wykres3.csv"); //wykres widma w skali decybelowej

	fk = 0;
	float tresh = (*max_element(amplitudowe.begin(), amplitudowe.end())) / 10000;

	for (int i = 0; i < tprosty.size()/2+1; i++)
	{
		fk = i * fs / tprosty.size();
		if (amplitudowe[i] < tresh)
			amplitudowe[i] = 0;
		else
			amplitudowe[i] = 10 * log10(amplitudowe[i]);
		plik3 << " " << fk << "," << " " << amplitudowe[i] << endl;
	}


	//Dla sygnałów uzyskanych na pierwszych laboratoriach obliczyć widma amplitudowe


	float fs = 200;
	float dt = 1.0 / fs;
	float tn = 0;
	float n = 0;
	vector<float> vec1, vec2, vec3, vec4, vec5, vec6;
	while (tn <= 1)
	{
		vec1.push_back(fun1(tn));
		vec2.push_back(fun2(tn));
		vec3.push_back(fun3(tn));
		vec4.push_back(fun4(tn));
		vec5.push_back(fun5(tn));
		vec6.push_back(fun6(tn));
		n++;
		tn = n * dt;
	}

	n = 0;
	float fk = 0; //skala częstotliwości
	vector<float> amplitudowe1, amplitudowe2, amplitudowe3, amplitudowe4, amplitudowe5, amplitudowe6;

	ofstream plik1, plik2, plik3, plik4, plik5, plik6;
	plik1.open("wykresfun1.csv");
	plik2.open("wykresfun2.csv");
	plik3.open("wykresfun3.csv");
	plik4.open("wykresfun4.csv");
	plik5.open("wykresfun5.csv");
	plik6.open("wykresfun6.csv");

	for (int i = 0; i <= vec1.size() / 2 + 1; i++) //skala w dft jest symetryczna, dlatego podajemy połowę próbek
	{
		fk = i * fs / vec1.size(); 
		amplitudowe1.push_back(dft(vec1, vec1.size(), i));
		amplitudowe2.push_back(dft(vec2, vec1.size(), i));
		amplitudowe3.push_back(dft(vec3, vec1.size(), i));
		amplitudowe4.push_back(dft(vec4, vec1.size(), i));
		amplitudowe5.push_back(dft(vec5, vec1.size(), i));
		amplitudowe6.push_back(dft(vec6, vec1.size(), i));
		plik1 << fk << " " << "," << " " << amplitudowe1[i] << endl;
		plik2 << fk << " " << "," << " " << amplitudowe2[i] << endl;
		plik3 << fk << " " << "," << " " << amplitudowe3[i] << endl;
		plik4 << fk << " " << "," << " " << amplitudowe4[i] << endl;
		plik5 << fk << " " << "," << " " << amplitudowe5[i] << endl;
		plik6 << fk << " " << "," << " " << amplitudowe6[i] << endl;
	}

	plik1.close();
	plik2.close();
	plik3.close();
	plik4.close();
	plik5.close();
	plik6.close();
	


	// Napisz funkcję realizującą Odwrotną Dyskretną Transformatę Fouriera.
	//Zweryfikuj poprawność jej działania odwracając sygnał z dziedziny 
	//częstotliwości do dziedziny czasu(wykorzystaj sygnał użyty w zadaniu drugim).


	vector<liczba_zesp> drugiedft;

	for (int i = 0; i <= tprosty.size() / 2 + 1; i++)
		drugiedft.push_back(dft2(tprosty, tprosty.size(), i));
		
	fk = 0;

	vector<float> odwrocone;

	ofstream plikkk;
	plikkk.open("idft.csv");

	for (int i = 0; i <= tprosty.size() / 2 + 1; i++) 
	{
		fk = i * fs / tprosty.size(); 
		odwrocone.push_back(idft(drugiedft, drugiedft.size(), i));
		plikkk << fk << " " << "," << " " << odwrocone[i] << endl;
	}

	plikkk.close();


	return 0;
}