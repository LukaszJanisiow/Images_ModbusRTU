#ifndef DIALOGOPCJEWYSWIETLACZ_H
#define DIALOGOPCJEWYSWIETLACZ_H
#include"qdialogwlasna.h"
#include <QDialog>
#include<QSpinBox>
#include<QLayout>
#include<QLabel>
#include<QPushButton>
#include<QMessageBox>

namespace Ui {
class DialogOpcjeWyswietlacz;
}
/**
 * \brief Klasa DialogOpcjeWyswietlacz odpowiada za okno dialogowe z ustawieniami wyswietlacza
 */
class DialogOpcjeWyswietlacz : public QDialogWlasna
{
    Q_OBJECT

private:
    Ui::DialogOpcjeWyswietlacz *ui;
    QSpinBox *spinBoxSzerokosc;
    QSpinBox *spinBoxWysokosc;
    QPushButton *pushButtonOK;
    QPushButton *pushButtonAnuluj;
    QHBoxLayout *boxLayoutPierwszy;
    QHBoxLayout * boxLayoutDrugi;
    QHBoxLayout *boxLayoutSzosty;
    QLabel *napisSzerokosc;
    QLabel *napisWysokosc;

signals:
    /**
     * \brief Sygnal emitowany po zatwierdzniu nacisniecia przycisku ok
     * @param wybrana przez uzytkownika szerokosc wyswietlacza
     * @param wybrana przez uzytkownika wysokosc wysweitlacza
     */
    void przyciskOkKlikniety(int szerokosc,int wysokosc);
    /**
     * \brief Sygnal emitowany po zmknieciu okna dialogowego
     */
    void oknoZamkniete();

public:
    /**
     * \brief Konstruktor klasy DialogOpcjeWyswietlacz
     * @param aktulana szerokosc wyswietlacza
     * @param aktualna wysokosc wyswietlacza
     * @param informacja czy trwa transmisja, 1 - trwa transmisja
     */
    explicit DialogOpcjeWyswietlacz(int szerokosc, int wysokosc,bool czyTrwaTransmisja);
    /**
     * \brief Destruktor klasy DialogOpcjeWyswietlacz
     */
    ~DialogOpcjeWyswietlacz();

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
     * \brief Funkcja wywolywana po zmknieciu okna
     */
    void closeEvent( QCloseEvent* event) override;
    /**
     * \brief Funkcja wywolywana po zmknieciu okna przez przycisk w prawym gornym rogu okna
     */
    void reject() override;
};

#endif // DIALOGOPCJEWYSWIETLACZ_H
