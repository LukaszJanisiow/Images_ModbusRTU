#ifndef DIALOGOPCJEOBROBKAOBRAZU_H
#define DIALOGOPCJEOBROBKAOBRAZU_H

#include "qdialogwlasna.h"
#include <QLabel>
#include<QComboBox>
#include <QHBoxLayout>
#include<QPushButton>

enum algorytmObrobkiObrazu
{
    najblizszegoSasiada = 0,
    biliniowa = 1,
    bicubic = 2,
    boxSampling = 3,
    autorska = 4
};

namespace Ui {
class DialogOpcjeObrobkaObrazu;
}

/**
 * \brief Klasa DialogOpcjeObrobkaObrazu odpowiada za okno dialogowe z ustawieniami obrobki obrazu
 */
class DialogOpcjeObrobkaObrazu : public QDialogWlasna
{
    Q_OBJECT
private:
    Ui::DialogOpcjeObrobkaObrazu *ui;
    QHBoxLayout *boxLayoutPierwszy;
    QHBoxLayout *boxLayoutDrugi;
    QLabel *napisAlorytmZmianyRozmiaruObrazu;
    QComboBox *comboBoxAlorytmZmianyRozmiaruObrazu;
    QPushButton *pushButtonOK;
    QPushButton *pushButtonAnuluj;

signals:
    /**
     * \brief Sygnal emitowany po zatwierdzniu nacisniecia przycisku ok
     * @param wybrany przez uzytkownika algorytm obrobki obrazu
     */
    void przyciskOkKlikniety(algorytmObrobkiObrazu metodaSkalowania);
    /**
     * \brief Sygnal emitowany po zmknieciu okna dialogowego
     */
    void oknoZamkniete();

public:
    /**
     * \brief Konstruktor klasy DialogOpcjeObrobkaObrazu
     * @param aktulany algoytmobrobkiobrazu
     */
    explicit DialogOpcjeObrobkaObrazu(algorytmObrobkiObrazu metodaSkalowania);
    /**
     * \brief Destruktor klasy DialogOpcjeObrobkaObrazu
     */
    ~DialogOpcjeObrobkaObrazu();

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

#endif // DIALOGOPCJEOBROBKAOBRAZU_H
