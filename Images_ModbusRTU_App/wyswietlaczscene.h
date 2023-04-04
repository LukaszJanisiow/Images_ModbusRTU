#ifndef WYSWIETLACZSCENE_H
#define WYSWIETLACZSCENE_H

#include<QGraphicsScene>
#include<QGraphicsEllipseItem>

/**
 * \brief Klasa WyswietlaczScene odpowiada za wyswietlanie podgladu projektu
 */
class WyswietlaczScene : public QGraphicsScene
{

private:
    /** lista diod na podgladzie*/
    QGraphicsEllipseItem **listaLedow;
    /** szerokosc wyswietlacza na podgladzie*/
    int szerokoscWyswietlacza;
    /** wysokosc wyswietlacza na podgladzie*/
    int wysokoscWyswietlacza;

public:
    /**
     * \brief Konstruktor Klasy WyswietlaczScene
     * @param rodzic obiektu
     * @param szerokosc wyswietlacza na podgladzie
     * @param wysokosc wyswietlacza na podgladzie
     */
    WyswietlaczScene(QObject * parent, int szerokosc, int wysokosc);
    /**
     * \brief Destruktor Klasy WyswietlaczScene
     */
    ~WyswietlaczScene();
    /**
     * \brief Funkcja odpowiedzialna za zmiane stanu diody na podgladzie o zadanych wspolrzednych
     * @param wspolrzedna odcieta diody
     * @param wspolrzedna rzedna diody
     * @param stan diody, 1 - zapalona, 0 - zgaszona
     */
    void zaswiecLed(int x, int y, bool czyZapalic);



};

#endif // WYSWIETLACZSCENE_H
