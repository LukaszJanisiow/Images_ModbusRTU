#ifndef DIALOGPOLACZMODBUS_H
#define DIALOGPOLACZMODBUS_H

#include "qdialogwlasna.h"
#include <QLabel>
#include<QComboBox>
#include <QHBoxLayout>
#include<QPushButton>
#include<QMessageBox>
#include<QSpinBox>



struct ParametryModbus
{
    std::string port;
    std::string predkoscTransmisji;
    std::string liczbaBitow;
    std::string parity;

};

namespace Ui {
class DialogPolaczModbus;
}
/**
 * \brief Klasa DialogPolaczModbus odpowiada za okno dialogowe z edycja parametrow polaczenia
 */
class DialogPolaczModbus : public QDialogWlasna
{
    Q_OBJECT

private:
    /** zmienna przechowujaca informacje czy okno sluzy do edycji parametrow polaczenia - 1 czy tez do wybrania portu - 0 */
    bool edycja;
    Ui::DialogPolaczModbus *ui;
    QComboBox *comboBoxPorty;
    QComboBox *comboBoxPredkoscTransmisji;
    QComboBox *comboBoxLiczbaBitow;
    QComboBox *comboBoxParity;
    QPushButton *pushButtonOK;
    QPushButton *pushButtonAnuluj;
    QHBoxLayout *pierwszy;
    QHBoxLayout * drugi;
    QHBoxLayout * trzeci;
    QHBoxLayout * czwarty;
    QHBoxLayout  *siodmy;
    QLabel *napisPorty;
    QLabel *napisPredkosciTransmisji;
    QLabel *napisLiczbaBitow;
    QLabel *napisParity;

signals:
    /**
     * \brief Sygnal emitowany po zatwierdzniu nacisniecia przycisku ok
     * @param wybrana przez uzytkownika parametry polaczenia
     */
    void przyciskOkKlikniety(ParametryModbus parametryModbus);
    /**
     * \brief Sygnal emitowany po zmknieciu okna dialogowego
     */
    void oknoZamkniete();

public:
    /**
     * \brief Konstruktor klasy DialogOpcjeWyswietlacz, gdy okno sluzy do edycji parametrow polaczenia i do wybrania portu
     */
    explicit DialogPolaczModbus();
    /**
     * \brief Konstruktor klasy DialogOpcjeWyswietlacz, gdy okno sluzy do edycji parametrow polaczenia
     * @param aktulana predkosc transmisji
     * @param aktualna liczba bitow znaku
     * @param przystosc
     */
    DialogPolaczModbus(std::string predkoscTransmisji,std::string liczbaBitow,std::string parity);
    /**
     * \brief Destruktor klasy DialogOpcjeWyswietlacz
     */
    ~DialogPolaczModbus();


private slots:
    /**
     * \brief Funkcja wywolywana po nacisnieu przycisku ok
     */
    void przyciskOKKlik();
    /**
     * \brief Funkcja wywolywana po nacisnieu przycisku anuluj
     */
    void przyciskAnulujKlik();



private:
    /**
     * \brief Funkcja wywolywana w celu dodania elementow sluzacych do edycji parametrow polaczenia
     */
    void dodanieRzeczyEdytowalnych();
    /**
     * \brief Funkcja wywolywana po zmknieciu okna
     */
    void closeEvent( QCloseEvent* event) override;
    /**
     * \brief Funkcja wywolywana po zmknieciu okna przez przycisk w prawym gornym rogu okna
     */
    void reject() override;
};

#endif // DIALOGPOLACZMODBUS_H
