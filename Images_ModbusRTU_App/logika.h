#ifndef LOGIKA_H
#define LOGIKA_H

#include<QThreadPool>
#include "modbusmaster.h"
#include "grafikaimage.h"
#include "wyswietlaczgrafika.h"
#include "wyswietlaczpixele.h"
#include "dialogpolaczmodbus.h"
#include "dialogopcjewyswietlacz.h"
#include"dialogopcjeobrobkaobrazu.h"

#include"zarzadzaniewysylaniemobrazumodbus.h"

enum coRobiUrzadzenie
{
    nic = 0,
    trybManualny = 1,               // wlacz/wylacz tryb graficzny , wysyla obraz
    trybAuto = 2,
    wlaczonyTrybRozgloszeniowy = 3
};


/**
 * \brief Klasa Logika odpowiada za logike aplikacji
 */
class Logika: public QObject{

    Q_OBJECT


private:
    /** zmienna przechowujaca wybrane przez uzytkownika parametry polaczenia */
    ParametryModbus pM;
    /** wskaznik do obiketu grafikaImage, który przetrzymuje oryginalny obraz  */
    GrafikaImage *grafikaImage;
    /** wskaznik do obiketu wyswietlaczGrafika, który przetrzymuje obraz przeskalowany i zbinaryzowany  */
    WyswietlaczGrafika *wyswietlaczGrafika;
    /** zmienna przetrzymujaca szerokosc wyswietlacza  */
    int szerokoscWyswietlacza;
    /** zmienna przetrzymujaca wysokosc wyswietlacza  */
    int wysokoscWyswietlacza;
    /** zmienna przetrzymujaca metodę skalowania  */
    algorytmObrobkiObrazu metodaSkalowania;
    /** zmienna przetrzymujaca informacje czy byl juz wczytany obraz */
    bool czyBylaWczytanaGrafika;
    /** zmienna przetrzymujaca informacje czy bylo juz rozpoczete polaczenie*/
    bool czyPolaczenieZModbusem;
    /** zmienna przetrzymujaca informacje czy okno dialogowe jest otwarte*/
    bool dialogOknoOtwarte;
    /** zmienna przetrzymujaca informacje o wybranym przez uzytkownika progi binaryzacji*/
    int progskaliSzarosci;
    /** zmienna przetrzymujaca informacje o wybranym przez uzytkownika progi binaryzacji*/
    bool zachowacProporcje;
    /** tablica przetrzymujaca informacje o tym co robi urzadzenie o danym adresie */
    coRobiUrzadzenie *tablicaUrzadzenPracujacych;
    /** zmienna przetrzymujaca informacje o ilosci wlaczonej watkow */
    int iloscWlaczonychWatkow;
    /** zmienna przetrzymujaca informacje o maksymalnej ilosci watkow */
    int maksymalnaIloscWlaczonychWatkow;
    /** wskaznika do obiektu wyswietlaczPixele, który odpowiada za przetrzymywanie informacji wartościach diod w wyswietlaczu*/
    WyswietlaczPixele *wyswietlaczPixele;
    /** wskaznika do obiektu zarzadzanieWysylaniemObrazuModbus, który odpowiada za zarzadzanie komunikacja z wysweitlaczami*/
    ZarzadzanieWysylaniemObrazuModbus *zarzadzanieWysylaniemObrazuModbus;

signals:
    /**
     * \brief Emisja sygnalu o otworzenie okna polaczenia
     */
    void otworzOknoPolaczenieModbus();
    /**
     * \brief Emisja sygnalu o braku odpowiedzi dnaego urzadzenia
     * @param adres uradzenia
     */
    void sygnalBrakOdpowiedziSlave(int adresUrzadzenia);
    /**
     * \brief Emisja sygnalu o polaczeniu lub rozlaczeniu
     * @param zmienna logiczna okreslajaca czy polaczono - 1 czy rozloaczono - 0
     */
    void sygnalpolaczonolubRozlaczonoZModbusem(bool polaczono);
    /**
     * \brief Emisja sygnalu o otworzenie okna wyswietlajacego komunikat o zerwaniu polaczenia
     * @param numer portu
     */
    void sygnalOtworzenieOknaModbusPortProblem(std::string port);
    /**
     * \brief Emisja sygnalu o otworzenie okna edycji parametrow polaczenia
     * @param aktualna predkosc transmisji
     * @param aktualna liczba bitow znaku
     * @param aktualna parzystosc
     * @param zmienna logiczna okreslajaca czy trwa w danym momencie transmisja
     */
    void otworzOknoEdycjiModbus(std::string predkoscTransmisji,std::string liczbaBitow, std::string parity, bool czyTrwaTransmisja);
    /**
     * \brief Emisja sygnalu o otworzenie okna opcji wyswietlacza
     * @param aktualna szerokosc wyswietlacza
     * @param aktualna wysokosc wyswietlacza
     * @param zmienna logiczna okreslajaca czy trwa w danym momencie transmisja
     */
    void otworzOknoOpcjiWyswietlacza(int szerokosc, int wysokosc, bool czyTrwaTransmisja);
    /**
     * \brief Emisja sygnalu o zmiane wymiarow wyswietlacza w GUI
     * @param aktualna szerokosc wyswietlacza
     * @param aktualna wysokosc wyswietlacza
     */
    void sygnalzmienWymiaryScena(int szerokoscWyswietlacza,int wysokoscWyswietlacza);
    /**
     * \brief Emisja sygnalu o wyswietlenie okna wyboru sposobu obrobki obrazu
     * @param aktualna metoda skalowania obrazu
     */
    void otworzOknoObrobkiObrazu(algorytmObrobkiObrazu metodaSkalowania);
    /**
     * \brief Emisja sygnalu o wyswietlenie okna dialogowego do wybiernia obrazu
     */
    void otworzoknoDialogoweDoPobieraniaObrazu();
    /**
     * \brief Emisja sygnalu o zaswiecenie danej diody w GUI
     * @param wspolrzedna rzedna diody
     * @param wspolrzedna odcieta diody
     * @param zmienna logiczna okreslajaca czy dioda ma byc zapalona - 1 czy zgaszona - 0
     */
    void sygnalZaswiecLedScena(int x,int y, bool czyZaswiec);
    /**
     * \brief Emisja sygnalu o zmiane progu binaryzacji
     * @param prog binaryzacji
     */
    void sygnalValueSliderGranicaSkaliSzarosci(int prog);
    /**
     * \brief Emisja sygnalu o zamkniecie okna dialogowego
     */
    void zamknijOknoDialogowe();
    /**
     * \brief Emisja sygnalu o wyswietlenie okna zapytania do uzytkownika o zakonczeniu wszystkich transmisji
     * @param adres urzadzenia
     * @param czynnosc jaka ma zostac wykonana po zakonczeniu transmisji
     */
    void sygnalczyWylaczycWszystkieTransmisjeTrybRozgloszeniowy(int adresUrzadzenia, int czynnosc);
    /**
     * \brief Emisja sygnalu o zmiane stanow przyciskow
     * @param co robi dane urzadzenie
     */
    void sygnalzmienStanPrzyciskowModbus(coRobiUrzadzenie corobiurzadzenie);
    /**
     * \brief Emisja sygnalu o wyswietlenie okna o osiagnietej maksymalnej ilosci transmisji w danym momencie
     */
    void sygnalmaksymalnaIloscTransmisjiZostalaOsiagnieta();
    /**
     * \brief Emisja sygnalu o wyswietlanej aktualnie ilosci trwajacych transmisji
     * @param ilosc transmisji
     */
    void sygnalzmienIloscTrwajacychTransmisji(int iloscWlaczonychWatkow);
    /**
     * \brief Emisja sygnalu o rozpoczeciu lub zakonczeniu transmisji podczas otwarcia okna dialogowego
     * @param 1 - trwa transmisja, 0 - zakonczono wszystkie transmisje
     */
    void sygnaldialogOknoTrwajaTransmisje(bool tak);
    /**
     * \brief Emisja sygnalu o zakonczeniu wysylania do danego urzadzenia
     * @param adres urzadzenia
     */
    void sygnalzakonczenieWysylania(int adresUrzadzenia);
    /**
     * \brief Emisja sygnalu o braku polaczenia z portem com
     */
    void sygnalbrakPolaczeniaCom();

public:
    /**
     * \brief Konstruktor klasy Logika
     */
    explicit Logika(QObject *parent = nullptr);
    /**
     * \brief Destruktor klasy Logika
     */
    ~Logika();
    /**
     * \brief Funkcja odpowiadajaca za wczytanie obrazu
     * @param zmienna przechowujaca nazwe pliku
     */
    bool wczytajGrafike(QString nazwaPliku);

public slots:
    /**
     * \brief Funkcja odpowiadająca za odpowiedz logiki na przycisniecie przycisku polacz Modbus
     */
    void przyciskPolaczModbus();
    /**
     * \brief Funkcja odpowiadajaca za polaczenie oraz zapisanie parametrow polaczenie
     * @param zmienna przechowujaca parametry polaczenia
     */
    void polaczModbus(ParametryModbus pM);
    /**
     * \brief Funkcja odpowiadajaca za edycje parametrow polaczenie
     * @param zmienna przechowujaca parametry polaczenia
     */
    void edytujModbus(ParametryModbus pM);
    /**
     * \brief Funkcja odpowiadająca za zerwanie polaczenia
     */
    void przyciskRozlaczModbus();
    /**
     * \brief Funkcja odpowiadajaca za odpowiedz logiki na przycisniecie przycisku edytuj Modbus
     */
    void przyciskEdytujModbus();
    /**
     * \brief Funkcja odpowiadajaca za odpowiedz logiki na przycisniecie przycisku wyswietlacz Opcje
     */
    void przyciskWyswietlaczOpcje();
    /**
     * \brief Funkcja odpowiadajaca za zmiane wymiarow wyswietlacza
     * @param zmienna przechowujaca wybrana przez uzytkownika szerokosc wyswietlacza
     * @param zmienna przechowujaca wybrana przez uzytkownika wyswokosc wyswietlacza
     */
    void zmianaWymiarowWyswietlacz(int szerokosc,int wysokosc);
    /**
     * \brief Funkcja odpowiadajaca za odpowiedz logiki na przycisniecie przycisku obrobka obrazu Opcje
     */
    void przyciskObrobkaObrazuOpcje();
    /**
     * \brief Funkcja odpowiadajaca za zmiane algorytmu skalowania
     * @param zmienna przechowujaca wybrana przez uzytkownika metoda skalowania
     */
    void zmianaAlgorytmuObrobkiObrazu(algorytmObrobkiObrazu metodaSkalowania);
    /**
     * \brief Funkcja odpowiadajaca za odpowiedz systemu na zamkniecie okna dialogowego
     */
    void dialogOknoZamkniete();
    /**
     * \brief Funkcja odpowiadajaca za odpowiedz logiki na przycisniecie przycisku wczytaj obraz
     */
    void przyciskOtworzObrazek();
    /**
     * \brief Funkcja odpowiadajaca za odpowiedz logiki na przycisniecie przycisku wyswietl obrazek
     * @param wybrany przez uzytkownika aktualnie prog binaryzacji
     * @param zmienna logiczna okreslajaca czy obrazek ma zostac wyswietlony zachowujac proporcje
     */
    void przyciskWyswietlObrazek(int granicaSkaliSzarosci,bool zachowacProporcje);
    /**
     * \brief Funkcja odpowiadajaca za zapis aktulanej wartosci wybranego przez uzytkownika progu binaryzacji
     * @param wybrany przez uzytkownika aktualnie prog binaryzacji
     */
    void sliderGranicaSkaliSzarosciZmienilWartosc(int granicaSkaliSzarosci);
    /**
     * \brief Funkcja odpowiadajaca za odpowiedz logiki na przycisniecie przycisku automatyczne progowanie
     */
    void przyciskAutomatyczneProgowanie();
    /**
     * \brief Funkcja odpowiadajaca za odpowiedz logiki na przycisniecie przycisku wyslij obraz
     * @param wybrany przez uzytkownika adres urzadzenia
     */
    void przyciskWyslijObrazModbus(int adresUrzadzenia);
    /**
     * \brief Funkcja odpowiadajaca za odpowiedz logiki na przycisniecie przycisku zatrzymaj
     * @param wybrany przez uzytkownika adres urzadzenia
     */
    void przyciskZatrzymajWysylanieObrazModbus(int adresUrzadzenia);
    /**
     * \brief Funkcja odpowiadajaca za odpowiedz logiki na przycisniecie przycisku wlacz tryb graficzny
     * @param wybrany przez uzytkownika adres urzadzenia
     */
    void przyciskWlaczTrybGraficznyModbus(bool wlacz,int adresUrzadzenia);
    /**
     * \brief Funkcja odpowiadajaca za odpowiedz logiki na zmiane stanu checkboxa wlaczenie trybu autoamtycznego
     * @param wybrany przez uzytkownika adres urzadzenia
     * @param zmienna logiczna okreslajaca wlaczenie czy wylaczenie trybu automatycznego
     */
    void przyciskWlaczTrybAutomatycznyModbus(int adresUrzadzenia, bool wlacz);
    /**
     * \brief Funkcja odpowiadajaca za odpowiedz logiki na zmiane wybranego adresu uzytkownika
     * @param wybrany przez uzytkownika adres urzadzenia
     */
    void pokazOdpowiedniePrzyciskiModbus(int adresUrzadzenia);
    /**
     * \brief Funkcja odpowiadajaca za odpowiedz logiki na brak polaczenia z portem com
     */
    void brakPolaczeniaZPortemCom();
    /**
     * \brief Funkcja odpowiadajaca za odpowiedz logiki na przycisniecie przycisku zakoncz wszystkie transmisje
     */
    void przyciskZakonczWszystkieTransmisjeModbusModbus();
    /**
     * \brief Funkcja odpowiadajaca za odpowiedz logiki na wybranie przez uzytkownika opcji zakonczenia wszystkich transmisji i transmisje w trybie rozgloszeniowym
     * @param wybrany przez uzytkownika adres urzadzenia
     */
    void zakonczWyslijDoUrzadzenia(int adresUrzadzenia);
    /**
     * \brief Funkcja odpowiadajaca za odpowiedz logiki na wybranie przez uzytkownika opcji zakonczenia wszystkich transmisji i wlaczenie trybu graficznego
     * @param wybrany przez uzytkownika adres urzadzenia
     * @param zmienna logiczna okreslajaca wlaczenie czy wylaczenie trybu graficznego
     */
    void zakonczWlaczTrybGraficzny(int adresUrzadzenia, bool czyWlaczyc);
    /**
     * \brief Funkcja odpowiadajaca za odpowiedz logiki na wybranie przez uzytkownika opcji zakonczenia wszystkich transmisji i wlaczenie trybu automatycznego
     * @param wybrany przez uzytkownika adres urzadzenia
     * @param zmienna logiczna okreslajaca wlaczenie czy wylaczenie trybu automatycznego
     */
    void zakonczAutomatyczneWysylanie(int adresUrzadzenia,bool wlacz);
    /**
     * \brief Funkcja odpowiadajaca za zwrocenie informacji co robi urzadzenie o danym adresie
     * @param wybrany przez uzytkownika adres urzadzenia
     */
    coRobiUrzadzenie gettablicaUrzadzenPracujacych(int adresUrzadzenia);

private slots:
    /**
     * \brief Funkcja odpowiadajaca za odpowiedz logiki na zakonczenie transmisji do danego urzadzenia
     * @param adres urzadzenia
     */
    void watekWysylaniaModbusZakonczony(int adresUrzadzenia);
    /**
     * \brief Funkcja odpowiadajaca za odpowiedz logiki na brak odpowiedzi danego urzadzenia podrzednego
     * @param adres urzadzenia
     */
    void brakOdpowiedziSlave(int adresUrzadzenia);




private:
    /**
     * \brief Funkcja odpowiadajaca za wysylanie obrazu
     * @param adres urzadzenia
     */
    void wyslijObrazModbus(int adresUrzadzenia);
    /**
     * \brief Funkcja odpowiadajaca za wlaczanie trybu graficznego
     * @param zmienna logiczna czy wlaczac tryb graficzny
     * @param adres urzadzenia
     */
    void wlaczTrybGraficznyModbus(bool wlacz,int adresUrzadzenia);
    /**
     * \brief Funkcja odpowiadajaca za wlaczenie trybu automatycznej transmisji do danego urzadzenia
     * @param adres urzadzenia
     * @param zmienna logiczna czy wlaczac automatyczna transmisje
     */
    void wlaczTrybAutomatycznyModbus(int adresUrzadzenia, bool wlacz);




};

#endif // LOGIKA_H
