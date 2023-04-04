#ifndef WYSWIETLACZVIEW_H
#define WYSWIETLACZVIEW_H

#include<QGraphicsView>
#include<QtWidgets>

/**
 * \brief Klasa WyswietlaczView odpowiada za widok podgladu wyswietlacza
 */
class WyswietlaczView : public QGraphicsView
{
public:
    /**
     * \brief Konstuktor klasy WyswietlaczView
     */
    WyswietlaczView(QWidget *parent = 0);

protected:
    /**
     * \brief Funkcja odpowiedzialna za przyblizanie i oddalanie widoku podgladu wyswietlacza
     */
    void wheelEvent(QWheelEvent *event) override;

};

#endif // WYSWIETLACZVIEW_H
