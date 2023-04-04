#ifndef MODBUSMASTER_H
#define MODBUSMASTER_H

#include <windows.h>
#include<qobject.h>
#include <QObject>
#include<mutex>


enum zwrot
{
    poprawna = 0,
    nFunkcja = 1,                          //Kod funkcji odebrany w zapytaniu nie jest obsługiwany przezj ednostkę podrzędną.
    nAdresDanych = 2,
    nWartoscDanych = 3,
    niepowodzenie = 4,
    potwierdzenie = 5,
    jednostkaPodrzednaZajeta = 6,
    bladParzystosciPamieci = 7,
    brakOdpowiedzilubZla = 8,
    bladPolaczeniaComZwrot = 9
};

enum zwrotWyslaniaRamki
{
    wykonano = 0,
    bladPolaczeniaCom = 1,
    bladPolaczeniaZWyswietlaczem = 2
};

/**
 * \brief Klasa ModbusMaster odpowiada za komunikacje z wyswietlaczami
 */
class ModbusMaster:  public QObject
{
    Q_OBJECT

private:
    /** zmienna przechowujaca informacje o otwartym polaczeniu */
    HANDLE hComm;
    /** zmienna przechowujaca informacje o pramaterach polaczenia */
    DCB dcb;
    /** zmienna przechowujaca informacje o timneoutach */
    COMMTIMEOUTS timeouts;
    /** zmienna przechowujaca numer portu */
    std::string port;
    /** zmienna przechowujaca ostatnia odebrana wiadomosc */
    unsigned char* ostatniaOdebranaWiadomosc;
    /** zmienna przechowujaca dlugosc ostatniej odebranej wiadomosci */
    int dlugoscOstatniejOdebranejWiadomosci;
    /** synchronizator */
    std::mutex m;

public:
    /**
     * \brief Konstuktor klasy ModbusMaster
     */
    ModbusMaster();
    /**
     * \brief Destuktor klasy ModbusMaster
     */
    ~ModbusMaster();
    /**
     * \brief Funkcja odpowiedzialna za ustawienia za rozpoczecie polaczenia, zwraca czy polaczenie nastapilo
     * @param numer portu
     */
    bool ustawieniaPort(std::string port);
    /**
     * \brief Funkcja odpowiedzialna za ustawienie parametrow polaczenia, zwraca czy ustawienie przbieglo pomyslnie
     * @param predkosc transmisji
     * @param liczba bitow znaku
     * @param parzystosc
     */
    bool ustawieniaPrzesylu(std::string predkoscTransmisji,std::string liczbaBitow, std::string parity);
    /**
     * \brief Funkcja odpowiedzialna za zerwanie polaczenia z portem
     */
    void rozlacz();
    /**
     * \brief Funkcja odpowiedzialna za stworzenie i wyslanie ramki wlaczenia lub wylaczenia trybu graficznego, zwraca czy wyslanie przebieglo pomyslnie
     * @param adres urzadzenia
     * @param 1 - wlaczenie, 0 - wylaczenie
     */
    zwrotWyslaniaRamki wlaczTrybGraficzny(int adresUrzadzenia, bool wlacz);
    /**
     * \brief Funkcja odpowiedzialna za stworzenie i wyslanie ramki wiersza obrazu, zwraca czy wyslanie przebieglo pomyslnie
     * @param adres urzadzenia
     * @param numer wiersza
     * @param tablica stanow diod w wierszu
     * @param ilosc diod w wierszu
     */
    zwrotWyslaniaRamki zaswiec(int adresUrzadzenia, int numerWiersza, unsigned char *wiadomosc, int dlugoscWiadomosci);



signals:
    /**
     * \brief Sygnal emitowany przy braku odpowiedzi danego urzadzenia
     * @param adres urzadzenia
     */
    void brakOdpowiedziSlave(int numerSlave);



private:
    /**
     * \brief Funkcja odpowiedzialna za obliczenie sumu kontrolnej uzywana przy wysylaniu i odbieraniu ramek
     * @param wiadomosc
     * @param ilosc bitow wiadomosci
     */
    unsigned char *obliczCRC(unsigned char *pMessage, unsigned int NumberOfBytes);
    /**
     * \brief Funkcja odpowiedzialna za sprawdzenie sumy kontrolnej wiadomosci, ktora zostala odebrana
     * @param wiadomosc
     * @param ilosc bitow wiadomosci
     */
    bool sprawdzCRC(unsigned char *wiadomosc, unsigned int dlugoscWiadomosci);
    /**
     * \brief Funkcja odpowiedzialna za wsylanie ramki
     * @param adres urzadzenia
     * @param ramka rozkazowa
     * @param ilosc bitow wiadomosci
     * @param poprawna odpowiedz urzadzenia podrzednego
     * @param dlugosc odpowiedzi urzadzenia podrzednego
     */
    zwrotWyslaniaRamki wyslijRamke(int adresUrzadzenia,unsigned char *ramkaRTU, unsigned int dlugoscRamki, unsigned char *odpowiedz,int dlugoscOdpowiedzi);
    /**
     * \brief Funkcja odpowiedzialna za czekanie i odbieranie wiadomosci od urzadzenia podrzednego
     * @param poprawna odpowiedz urzadzenia podrzednego
     * @param dlugosc odpowiedzi urzadzenia podrzednego
     */
    zwrot czekanieNaOdpowiedz(unsigned char *odpowiedz,int dlugoscOdpowiedzi);
    /**
     * \brief Funkcja odpowiedzialna za probe ponownego polaczenia z portem
     */
    bool sprobujPolaczycPonownieZPortem();


};

#endif // MODBUSMASTER_H
