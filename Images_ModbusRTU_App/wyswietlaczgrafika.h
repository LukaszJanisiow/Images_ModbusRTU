#ifndef WYSWIETLACZGRAFIKA_H
#define WYSWIETLACZGRAFIKA_H

#include<cmath>

struct RGBS                                    // RGB, skala szarosci i czarno biały
{
    int red;
    int green;
    int blue;
    int skalaSzarosci;
    bool czarnobialy;
};


/**
 * \brief Klasa WyswietlaczGrafika odpowiada za operacje na obrazie
 */

class WyswietlaczGrafika
{
private:
    /** szerokosc aktualnego obrazu */
    int szerokosc;
    /** wysokosc aktualnego obrazu */
    int wysokosc;
    /** tablica wartosci pikseli obrazu */
    RGBS **tablicaPixeli;

public:
    /**
     * \brief Konstruktor klasy WyswietlaczGrafika
     * @param szerokosc obrazu
     * @param wysokosc obrazu
     */
    WyswietlaczGrafika(int szerokosc, int wysokosc);
    /**
     * \brief Destruktor klasy WyswietlaczGrafika
     */
    ~WyswietlaczGrafika();
    /**
     * \brief Funkcja odpowiedzialna za uzupelnienie wartosci kolorow danego piksela obrazu
     * @param wspolrzedna odcieta piksela
     * @param wspolrzedna rzedna piksela
     * @param tablica wartosci kanalow RGB
     */
    void uzupelnijKolor(int x, int y, int tablicaRGB[3]);
    /**
     * \brief Funkcja odpowiedzialna za skalowanie obrazu
     * @param szerokosc na jak ma zostac przeskalowany obraz
     * @param wysokosc na jak ma zostac przeskalowany obraz
     * @param metoda jaka ma zostac uzyta
     * @param poziom progu binaryzacji
     * @param zmienna logiczna czy zachowac porporcje obrazu wejsciowego 1 - zachowac
     */
    void skalowanie(int naSzerokosc, int naWysokosc, int metoda, int poziomCzarnoBialy, bool zachowanieProporcji);        //metoda 0- najbliższego sasiada   , 1 - Interpolacja dwuliniowa   2 - Bicubic, 3- box sampling    4 - autorska
    /**
     * \brief Funkcja odpowiedzialna za uzupelnienie stanow pikseli obrazu binarnego
     * @param poziom progu binaryzacji
     */
    void uzupelnijCzarnoBialy(int poziom);
    /**
     * \brief Funkcja zracajaca wartosc binarna piksela o podanych wspolrzednych
     * @param wspolrzedna odcieta piksela
     * @param wspolrzedna rzedna piksela
     */
    bool czyCzarnoBialy(int x, int y);
    /**
     * \brief Funkcja odpowiedzialna za uzupelnie wartosci obrazu binarnego za pomoca progu wyznaczonego metoda Otsu i zwrocenie tego progu
     */
    int uzupelnienieCzarnoBialyAutomatycznyProg();
    /**
     * \brief Funkcja zwracajaca szerokosc obrazu
     */
    int getSzerokosc() const;
    /**
     * \brief Funkcja zwracajaca wysokosc obrazu
     */
    int getWysokosc() const;
    /**
     * \brief Funkcja zwracajaca porg binaryzacji wyznaczony metoda Otsu
     */
    int metodaOtsu();

private:
    /**
     * \brief Funkcja zwracajaca tablice obrazu przeskalowanego za pomoca metodu najblizszego sasiada
     * @param szerokosc na jak ma zostac przeskalowany obraz
     * @param wysokosc na jak ma zostac przeskalowany obraz
     */
    RGBS **skalowanieNajblizszegoSasiada(int naSzerokosc, int naWysokosc);
    /**
     * \brief Funkcja zwracajaca tablice obrazu przeskalowanego za pomoca metodu biliniowa
     * @param szerokosc na jak ma zostac przeskalowany obraz
     * @param wysokosc na jak ma zostac przeskalowany obraz
     */
    RGBS **skalowanieBiliniowe(int naSzerokosc, int naWysokosc);
    /**
     * \brief Funkcja zwracajaca interpolacje wartosci za pomoca interpolacji liniowej
     * @param wartosc kolorw piksela
     * @param wartosc kolorw drugiego piksela
     * @param wspolczynnik odleglosci
     */
    RGBS interpolacjaBiliniowa(RGBS wartosc1, RGBS wartosc2,double wspolczynnik);
    /**
     * \brief Funkcja zwracajaca tablice obrazu przeskalowanego za pomoca metodu bicubic
     * @param szerokosc na jak ma zostac przeskalowany obraz
     * @param wysokosc na jak ma zostac przeskalowany obraz
     */
    RGBS **skalowanieBicubic(int naSzerokosc, int naWysokosc);
    /**
     * \brief Funkcja zwracajaca wartosci pikseli w oknie pikseli metody bicubic
     * @param wspolrzedna odcieta piksela interpolowanego
     * @param wspolrzedna rzedna piksela interpolowanego
     */
    RGBS **pobieranieWartosciKolorowBicubic(int x, int y);
    /**
     * \brief Funkcja zwracajaca interpolacje wartosci pikseli w oknie pikseli metody bicubic
     * @param tablica wartosci kolorw pikseli interpolowanych
     * @param wspolrzedna piksela interpolowanego
     */
    RGBS interpolacjaBicubic(RGBS *tablicaKolorow, double wspolrzedna);
    /**
     * \brief Funkcja zwracajaca tablice obrazu przeskalowanego za pomoca metodu autorskiej
     * @param szerokosc na jak ma zostac przeskalowany obraz
     * @param wysokosc na jak ma zostac przeskalowany obraz
     */
    RGBS **metodaAutorska(int naSzerokosc, int naWysokosc);
    /**
     * \brief Funkcja zwracajaca tablice obrazu przeskalowanego za pomoca metodu boxsampling
     * @param szerokosc na jak ma zostac przeskalowany obraz
     * @param wysokosc na jak ma zostac przeskalowany obraz
     */
    RGBS **skalowanieBoxSampling(int naSzerokosc, int naWysokosc,int skalaSrednia);
    /**
     * \brief Funkcja zwracajaca wartosci kolorow piksela o danych wspolrzednych
     * @param wspolrzedna odcieta piksela
     * @param wspolrzedna rzedna piksela
     */
    RGBS pobierzWartoscKolorow(int x,int y);
};

#endif // WYSWIETLACZGRAFIKA_H
