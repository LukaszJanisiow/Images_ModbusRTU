#include "grafikaimage.h"

GrafikaImage::GrafikaImage()
{

}

bool GrafikaImage::wczytajGrafike(QString nazwaPliku)
{
    if(this->load(nazwaPliku))
    {
        return true;
    }
    else
    {
        return false;
    }

}


int *GrafikaImage::zwrocTabliceRGB(int x, int y)
{
        QColor aktcolor = this->pixel(x, y);

        int *tablica = new int[3];

        tablica[0] = aktcolor.red();
        tablica[1] = aktcolor.green();
        tablica[2] = aktcolor.blue();

        return tablica;
}
