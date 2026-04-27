#include <iostream>
#include <complex>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

using Zespolona = complex<double>;
using Macierz = vector<vector<Zespolona>>;

// Konwertuje napis typu "3+2i" lub "4-1i" na liczbe zespolona
Zespolona wczytaj_zespolona(const string& wejscie) {
    string s = wejscie;
    s.erase(remove(s.begin(), s.end(), ' '), s.end());

    size_t i_pos = s.find('i');
    if (i_pos == string::npos) {
        return Zespolona(stod(s), 0.0);
    }

    if (s == "i") return Zespolona(0.0, 1.0);
    if (s == "+i") return Zespolona(0.0, 1.0);
    if (s == "-i") return Zespolona(0.0, -1.0);

    string cz_real = "0";
    string cz_urojona;

    size_t plus = s.find_last_of('+', i_pos - 1);
    size_t minus = s.find_last_of('-', i_pos - 1);

    size_t znak_pos = (plus != string::npos && minus != string::npos)
        ? max(plus, minus)
        : (plus != string::npos ? plus : minus);

    if (znak_pos != string::npos && znak_pos != 0) {
        cz_real = s.substr(0, znak_pos);
        cz_urojona = s.substr(znak_pos, i_pos - znak_pos);
    }
    else {
        cz_urojona = s.substr(0, i_pos);
    }

    double re = stod(cz_real);
    double im = (cz_urojona == "+" || cz_urojona == "") ? 1.0 :
        (cz_urojona == "-") ? -1.0 : stod(cz_urojona);

    return Zespolona(re, im);
}

// Wczytuje macierz od uzytkownika
Macierz wczytaj_macierz(int wiersze, int kolumny, const string& nazwa) {
    Macierz M(wiersze, vector<Zespolona>(kolumny));
    cout << "Podaj elementy macierzy " << nazwa << " (" << wiersze << "x" << kolumny << "), np. 3+2i lub 5:\n";
    for (int i = 0; i < wiersze; ++i)
        for (int j = 0; j < kolumny; ++j) {
            string wejscie;
            cout << nazwa << "[" << i << "][" << j << "] = ";
            cin >> wejscie;
            M[i][j] = wczytaj_zespolona(wejscie);
        }
    return M;
}

// Iloczyn skalarny Frobeniusa
Zespolona iloczyn_frobeniusa(const Macierz& A, const Macierz& B) {
    Zespolona suma = 0.0;
    for (size_t i = 0; i < A.size(); ++i)
        for (size_t j = 0; j < A[0].size(); ++j)
            suma += conj(A[i][j]) * B[i][j];
    return suma;
}

// Norma Frobeniusa
double norma_frobeniusa(const Macierz& A) {
    double suma = 0.0;
    for (const auto& wiersz : A)
        for (const auto& el : wiersz)
            suma += norm(el);
    return sqrt(suma);
}

// Odleglosc Frobeniusa
double odleglosc_frobeniusa(const Macierz& A, const Macierz& B) {
    double suma = 0.0;
    for (size_t i = 0; i < A.size(); ++i)
        for (size_t j = 0; j < A[0].size(); ++j)
            suma += norm(A[i][j] - B[i][j]);
    return sqrt(suma);
}

// Kat miedzy macierzami
double kat_miedzy_macierzami(const Macierz& A, const Macierz& B) {
    Zespolona iloczyn = iloczyn_frobeniusa(A, B);
    double normaA = norma_frobeniusa(A);
    double normaB = norma_frobeniusa(B);
    double cos_theta = real(iloczyn) / (normaA * normaB);
    cos_theta = max(-1.0, min(1.0, cos_theta));
    const double PI = 3.141592653589793;
    return acos(cos_theta) * 180.0 / PI;
}

int main() {
    int wiersze, kolumny;
    cout << "Podaj liczbe wierszy i kolumn (dla obu macierzy): ";
    cin >> wiersze >> kolumny;

    Macierz A = wczytaj_macierz(wiersze, kolumny, "A");
    Macierz B = wczytaj_macierz(wiersze, kolumny, "B");

    cout << "\nIloczyn Frobeniusa (Re,Im): " << iloczyn_frobeniusa(A, B) << endl;
    cout << "Norma Frobeniusa A: " << norma_frobeniusa(A) << endl;
    cout << "Norma Frobeniusa B: " << norma_frobeniusa(B) << endl;
    cout << "Odleglosc Frobeniusa: " << odleglosc_frobeniusa(A, B) << endl;
    cout << "Kat miedzy macierzami (stopnie): " << kat_miedzy_macierzami(A, B) << endl;

    return 0;
}
