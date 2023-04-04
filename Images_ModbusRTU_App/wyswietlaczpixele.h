#ifndef WYSWIETLACZPIXELE_H
#define WYSWIETLACZPIXELE_H

/**
 * \brief Klasa WyswietlaczPixele odpowiada za przetrzymywanie wartosci pikseli projektu
 */
class WyswietlaczPixele
{
private:
    /** szerokosc wyswietlacza */
    int szerokosc;
    /** wysokosc wyswietlacza */
    int wysokosc;
    /** tablica stanow pikseli na podgladzie */
    bool **tablicaPixeli;

public:
    /**
     * \brief Konstruktor klasy WyswietlaczPixele
     * @param szerokosc wyswietlacza
     * @param wysokosc wyswietlacza
     */
    WyswietlaczPixele(int szerokosc, int wysokosc);
    /**
     * \brief Destruktor klasy WyswietlaczPixele
     */
    ~WyswietlaczPixele();
    /**
     * \brief Funkcja odpowiedzialna za uzupelnienia stanu piksela o danych wspolrzendych
     * @param wspolrzedna odcieta piksela
     * @param wspolrzedna rzedna piksela
     * @param stan piksela 1- swiei, 0 - nie swieci
     */
    void uzupelnijPixel(int x, int y, bool czySwieci);
    /**
     * \brief Funkcja odpowiedzialna za zwrocenie stanu piksela o danych wspolrzednych
     * @param wspolrzedna odcieta piksela
     * @param wspolrzedna rzedna piksela
     */
    bool getPixel(int x,int y);


};

#endif // WYSWIETLACZPIXELE_H
