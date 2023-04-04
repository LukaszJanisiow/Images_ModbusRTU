#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QMessageBox>
#include "wyswietlaczview.h"
#include "wyswietlaczscene.h"
#include "logika.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


/**
 * \brief Klasa MainWindow odpowiada za glowne okno aplikacji
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    Logika *logika;
    QMenu *menuOpcje;
    QMenu *menuModbus;
    QAction *actionModbusPolacz;
    QAction *actionModbusEdytuj;
    QAction *actionModbusRozlacz;
    QAction *actionWyswietlaczOpcje;
    QAction *actionObrobkaObrazuOpcje;
    WyswietlaczScene *wyswietlaczScene;
    WyswietlaczView *wyswietlaczView;
    QVBoxLayout *vboxPasekBoczny;
    QLabel *labelGrafika;
    QPushButton *pushButtonOtworzObrazek;
    QGroupBox *groupBoxGrafika;
    QVBoxLayout *vboxGrafika;
    QCheckBox *checkboxZachowacProporcje;
    QPushButton *pushButtonWyswietlObrazek;
    QGroupBox *groupBoxProgSkaliSzarosci;
    QVBoxLayout *vboxProgSkaliSzarosci;
    QSlider *sliderGranicaSkaliSzarosci;
    QPushButton *pushButtonAutomatyczneProgowanie;
    QGroupBox *groupBoxModbus;
    QVBoxLayout *vboxModbus;
    QLabel *labelAdresUrzadzenia;
    QSpinBox *spinBoxAdresUrzadzenia;
    QHBoxLayout *adresUrzadzeniaLayout;
    QPushButton *pushButtonWlaczTrybGraficznyModbus;
    QPushButton *pushButtonWylaczTrybGraficznyModbus;
    QHBoxLayout *trybGraficznyLayout;
    QPushButton *pushButtonWyslijObrazModbus;
    QPushButton *pushButtonZatrzymajWysylanieObrazuModbus;
    QHBoxLayout *wyslijZatrzymajLayout;
    QCheckBox *checkboxAutomatyczneWysylanieModbus;
    QLabel *labelIloscTrwajacychTransmisji;
    QLabel *labelLiczbaTrwajacychTransmisji;
    QHBoxLayout *trwajaceTransmisjeLayout;
    QPushButton *pushButtonZakonczWszystkieTransmisjeModbus;
    QDialogWlasna *dialogOkno;

public:
    /**
     * \brief Konstruktor klasy MainWindow
     */
    MainWindow(QWidget *parent = nullptr);
    /**
     * \brief Destruktor klasy MainWindow
     */
    ~MainWindow();

private slots:
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o ustawenie grafikia
     * @param obraz
     */
    void ustawGrafike(QImage image);
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o wyswietelnie problemu z wczytywaniem obrazu
     */
    void otworzenieOknaGrafikaWczytanieProblem();
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o nacisnieciu przez uzytkownika przycisku wyswietl obraz
     */
    void wyswietlObrazekKlikniety();
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o nacisnieciu przez uzytkownika przycisku wyslij obraz
     */
    void wyslijObrazModbusKlikniety();
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o nacisnieciu przez uzytkownika przycisku zatrzymaj wysylanie
     */
    void zatrzymajWysylanieObrazModbusKlikniety();
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o nacisnieciu przez uzytkownika przycisku wlaczenie trybu graficznego
     */
    void wlaczTrybGraficznyModbusKlikniety();
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o nacisnieciu przez uzytkownika przycisku wylaczenie trybu graficznego
     */
    void wylaczTrybGraficznyModbusKlikniety();
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o nacisnieciu przez uzytkownika przycisku automatycznego wysylania
     */
    void automatyczneWysylanieModbusKlikniety();
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o braku odpowiedzi danego urzadzenia
     * @param adres urzadzenia
     */
    void brakOdpowiedziSlave(int adresUrzadzenia);
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o otwarcie okna polaczenie
     */
    void otwarcieOknaPolaczeniaModbus();
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o otwarcie okna polaczenia o zadanych parametrach
     * @param aktualna predkosc transmisji
     * @param aktualna liczba bitow znaku
     * @param aktualna parzystosc
     * @param zmienna logiczna okreslajaca czy trwa w danym momencie transmisja
     */
    void otwarcieOknaPolaczeniaModbus(std::string predkoscTransmisji,std::string liczbaBitow, std::string parity, bool czyTrwaTransmisja);
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o poloczeniu lub rozlaczeniu
     * @param 1 - polaczono, 0 - rozlaczono
     */
    void polaczonolubRozlaczonoZModbusem(bool polaczono);
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o otwarcie okna z komunikatem o braku polaczenia z portem com
     * @param numer portu
     */
    void otworzenieOknaModbusPortProblem(std::string port);
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o otwarcie okna edcyja polaczenia
     * @param aktualna predkosc transmisji
     * @param aktualna liczba bitow znaku
     * @param aktualna parzystosc
     * @param zmienna logiczna okreslajaca czy trwa w danym momencie transmisja
     */
    void otwarcueOknoEdycjiModbus(std::string predkoscTransmisji,std::string liczbaBitow, std::string parity, bool czyTrwaTransmisja);
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o otwarcie okna opcji wyswietlacza
     * @param aktualna szerokosc wyswietlacza
     * @param aktualna wysokosc wyswietlacza
     * @param zmienna logiczna okreslajaca czy trwa w danym momencie transmisja
     */
    void otwarcieOknaOpcjiwyswietlacza(int szerokosc, int wysokosc, bool czyTrwaTransmisji);
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o zmianie wymiarow wyswietlacza
     * @param aktualna szerokosc wyswietlacza
     * @param aktualna wysokosc wyswietlacza
     */
    void zmienWymiaryScena(int szerokosc, int wysokosc);
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o otwaarcie opcji obrobki obrazu
     * @param aktualna metoda skalowania
     */
    void otwarcieOknoObrobkiObrazu(algorytmObrobkiObrazu metodaSkalowania);
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o otwarcie okna dialogowego do pobrania obrazu
     */
    void otwarcieoknaDialogoweDoPobieraniaObrazu();
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o zapalanie danej diody na podgladzie
     * @param wspolrzedna rzedna diody
     * @param wspolrzedna odcieta diody
     * @param zmienna logiczna okreslajaca czy dioda ma byc zapalona - 1 czy zgaszona - 0
     */
    void zaswiecLedScena(int x,int y, bool czyZaswiecic);
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o zzmianie wartosci progu binaryzacji
     * @param prog binaryzacji
     */
    void setValueSliderGranicaSkaliSzarosci(int granica);
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o rozlaczeniu polaczenia
     */
    void modbusRozlaczKlikniety();
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o wyswietlenie okna z zapytaniem o zakonczenie wsyzstkich transmisji
     * @param adres urzadzenia
     * @param czynnosc jaka ma zostac wykonana po zarzymaniu transmisji
     */
    void czyWylaczycWszystkieTransmisjeTrybRozgloszeniowy(int adresUrzadzenia, int czynnosc);
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o zmiane stanow przyciskow
     * @param zmienna przetrzymujaca informacje co robi dane urzadzenie
     */
    void zmienStanPrzyciskowModbus(coRobiUrzadzenie coRobi);
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o wyswietlenie okna dialogowego o osiagnietej maksymalnej ilosc transmisji
     */
    void maksymalnaIloscTransmisjiZostalaOsiagnieta();
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o zmianie ilosci trwajacych transmisji
     * @param ilosc transmisji w danym momencie
     */
    void zmienIloscTrwajacychTransmisji(int liczbaTransmisji);
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o trwajacych transmisjach w trakcie otwarcia okna dialogowego
     * @param 1- trwa transmisja, 0 - zakonczono wszystkie transmisje
     */
    void dialogOknoTrwajaTransmisje(bool tak);
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o zakonczenie trwania transmisji do danego urzadzenia
     * @param adres urzadzenia
     */
    void zakonczenieWysylania(int adresUrzadzenia);
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o usuniecie okna dialogowego
     */
    void zamknijOkno();
    /**
     * \brief Funkcja odpowiadajaca na otrzymanie sygnalu o wyswietlenie komunikatu o braku polaczenia z portem
     */
    void brakPolaczeniaCom();

private:
    /**
     * \brief Funkcja odpowiadajaca za dodawanie elementow wczytywania grafiki
     */
    void dodawanieElemetowWczytywaniaGrafiki();
    /**
     * \brief Funkcja odpowiadajaca za dodawanie elementow obslugi modbus
     */
    void dodawanieElementowObslugiModbus();
    /**
     * \brief Funkcja odpowiadajaca za tworzenie menu gornego
     */
    void tworzenieMenuGornego();
    /**
     * \brief Funkcja odpowiadajaca za akcji do menu gornego
     */
    void tworzenieAction();
    /**
     * \brief Funkcja odpowiadajaca za aktywacje przyciskow oblsugi modbus
     * @param 1 - aktywowac, 0 - dezaktywowac
     */
    void przyciskiModbusEnabled(bool czyWlaczyc);


};
#endif // MAINWINDOW_H
