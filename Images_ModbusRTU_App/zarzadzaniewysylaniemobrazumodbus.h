#ifndef ZARZADZANIEWYSYLANIEMOBRAZUMODBUS_H
#define ZARZADZANIEWYSYLANIEMOBRAZUMODBUS_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include<QRunnable>

#include"modbusmaster.h"

enum coRobic
{
    wlaczycTrybGraficzny = 0,
    wylaczycTrybGraficzny = 1,
    wysylacObraz = 2,
    trybAutomatycznyWysylania = 3
};

/**
 * \brief Klasa ZarzadzanieWysylaniemObrazuModbus odpowiada za zarzadzanie rozkazami
 */

class ZarzadzanieWysylaniemObrazuModbus : public QObject, public QRunnable
{
    Q_OBJECT

private:
    /** tablica przetrzymujaca aktualnie wysylany obraz */
    bool **tablicaPixeli;
    /** szerokosc aktualnego obrazu */
    int szerokosc;
    /** wysokosc aktualnego obrazu */
    int wysokosc;
    /** adres aktualnie wybranego urzadzenia */
    int adresUrzadzenia;
    /** zmienna przetrzymujaca aktualne zadanie */
    coRobic coRobicAktualnie;
    /** zmienna przetrzymujaca informacje czy przerwac wysylanie */
    bool przerwacWysylanie;
    /** wskaznik do obiektu modbusMaster odpowiedzialnego za wyslanie rozkazow*/
    ModbusMaster *modbusMaster;
    /** zmienna logiczna przetrzymujaca informacje czy trwa transmisja*/
    bool czyWysylam;
    /** zmienna logiczna przetrzymujaca informacje czy trwa transmisja w trybie automatycznym*/
    bool trybAutomatyczny;
    /** zmienna logiczna przetrzymujaca informacje czy przerwac transmisje do aktulnie wybranego urzadzenia*/
    bool przerwacWysylanieDlaAktualnegoUrzadzenia;

signals:
    /**
     * \brief Sygnal emitowan po zakonczeniu transmisji
     * @param adres urzadzenia do ktorego dobywala sie transmisja
     */
    void finished(int adresUrzadzeniaWiadomosci);
    /**
     * \brief Sygnal emitowan przy braku odpowiedzi danego urzadzenia
     * @param adres urzadzenia ktore nie odpowiada
     */
    void brakOdpowiedziSlave(int adresUrzadzenia);
    /**
     * \brief Sygnal emitowan przy braku polaczenia z portem com
     */
    void brakPolaczeniaCom();


public:
    /**
     * \brief Konstruktor klasy ZarzadzanieWysylaniemObrazuModbus
     * @param szerokosc wyswietlacza
     * @param wysokosc wyswietlacza
     */
    explicit ZarzadzanieWysylaniemObrazuModbus(int szerokosc , int wysokosc);
    /**
     * \brief Destruktor klasy ZarzadzanieWysylaniemObrazuModbus
     */
    ~ZarzadzanieWysylaniemObrazuModbus();
    /**
     * \brief Funkcja odpowiedzialna za uzupelnienie stanu logicznego piksela
     * @param wspolrzedna odcieta piksela
     * @param wspolrzedna rzedna piksela
     * @param stan logiczny piksela
     */
    void uzupelnijPixel(int x, int y, bool czySwieci);
    /**
     * \brief Funkcja odpowiedzialna za zwrocenie czesci wiadomosci ze stanami binarnymi diod w danym wierszu
     * @param wspolrzedna rzedna danego wiersza
     */
    unsigned char *stworzWiadomosc(int rzad);
    /**
     * \brief Funkcja odpowiedzialna za ustawienie polaczenia z portem i zwrocenie informacji o powodzeniu
     * @param numer portu
     */
    bool ustawieniaPortModbus(std::string port);
    /**
     * \brief Funkcja odpowiedzialna za ustawienie parametrow polaczenia, zwraca czy ustawienie przbieglo pomyslnie
     * @param predkosc transmisji
     * @param liczba bitow znaku
     * @param parzystosc
     */
    bool ustawieniaPrzesyluModbus(std::string predkoscTransmisji,std::string liczbaBitow, std::string parity);
    /**
     * \brief Funkcja odpowiedzialna za zerwanie poloczenia z portem
     */
    void rozlaczModbus();
    /**
     * \brief Funkcja odpowiedzialna za ustawienie adresu urzadzenia
     * @param adres urzadzenia
     */
    void setAdresUrzadzenia(int value);
    /**
     * \brief Funkcja odpowiedzialna za ustawienie aktualnej czynnosci do wykonania
     * @param czynnosc
     */
    void setCoRobicAktualnie(const coRobic &value);
    /**
     * \brief Funkcja odpowiedzialna za zmiane rozmiaru wyswietlacza
     * @param szerokosc wyswietlacza
     * @param wysokosc wyswietlacza
     */
    void zmianaWielkosciWyswietlacza(int szerokosc,int wysokosc);
    /**
     * \brief Funkcja odpowiedzialna za rozpoczenie watku
     */
    void run() override;
    /**
     * \brief Funkcja odpowiedzialna za ustaweinie zmiennej do przerwania wyslania
     * @param wartosc zmiennej logicznej, 1- przerwac wysylanie
     */
    void setPrzerwacWysylanie(bool value);
    /**
     * \brief Funkcja odpowiedzialna za ustaweinie zmiennej do przerwania wyslania do danego urzadzenia
     * @param wartosc zmiennej logicznej, 1- przerwac wysylanie do danego urzadzenia
     */
    void setPrzerwacWysylanieDlaAktualnegoUrzadzenia(bool value);

private slots:
    /**
     * \brief Funkcja odpowiedzialna za odpowiedz na sygnal o braku odpowiedzi od danego urzadzenia
     * @param adres urzadzenia
     */
    void otrzymanieBrakOdpowiedziSlave(int adresUrzadzenia);




};

#endif // ZARZADZANIEWYSYLANIEMOBRAZUMODBUS_H
