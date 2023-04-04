#include "wyswietlaczscene.h"


WyswietlaczScene::WyswietlaczScene(QObject * parent, int szerokosc, int wysokosc) :QGraphicsScene(parent)
{
    this->szerokoscWyswietlacza = szerokosc;
    this->wysokoscWyswietlacza = wysokosc;
    this->setSceneRect(0,0,12*szerokosc + 42, 12*wysokosc + 22);
    listaLedow = new QGraphicsEllipseItem*[szerokosc];
    for (int i = 0; i < szerokosc; i++)
    {
            listaLedow[i] = new QGraphicsEllipseItem[wysokosc];
            for(int j = 0 ; j < wysokosc ; j++)
            {
               listaLedow[i][j].setRect(0,0,10,10);
               listaLedow[i][j].setBrush(QBrush(Qt::gray));
               this->addItem(&listaLedow[i][j]);
               listaLedow[i][j].setPos((21 + 12*i),(11 + 12*j));
            }
    }
}

void WyswietlaczScene::zaswiecLed(int x, int y, bool czyZapalic)
{
    if(czyZapalic)
    {
        listaLedow[x][y].setBrush(QBrush(Qt::red));
    }
    else
    {
        listaLedow[x][y].setBrush(QBrush(Qt::gray));
    }

}

WyswietlaczScene::~WyswietlaczScene()
{
    for (int i = 0; i<szerokoscWyswietlacza; i++)
    {
        delete [] listaLedow[i];
    }
}
