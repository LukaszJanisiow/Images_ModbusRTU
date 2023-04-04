#ifndef QDIALOGWLASNA_H
#define QDIALOGWLASNA_H

#include <QDialog>
/**
 * \brief Klasa QDialogWlasna jest klasa dziedziczaca po QDialog
 */
class QDialogWlasna : public QDialog
{

protected:
    /** zmienna przechowujaca informacje czy trwa w danym momencie transmisja  1- trwa transmisja, 0 - brak transmisji*/
    bool czyTrwaTransmisja;

public:
    /**
     * \brief Konstruktor klasy QDialogWlasna
     */
    QDialogWlasna();
    /**
     * \brief Funkcja odpowiedzialna za ustawienie zmiennej czy trwa w danym momencie transmisja
     * @param 1- trwa transmisja, 0 - brak transmisji
     */
    void setCzyTrwaTransmisja(bool value);


};

#endif // QDIALOGWLASNA_H
