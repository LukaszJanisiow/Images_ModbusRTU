#include "wyswietlaczpixele.h"

WyswietlaczPixele::WyswietlaczPixele(int szerokosc, int wysokosc)
{
    this->szerokosc = szerokosc;
    this->wysokosc = wysokosc;

    tablicaPixeli = new bool*[szerokosc];
    for (int i = 0; i<szerokosc; i++)
    {
            tablicaPixeli[i] = new bool [wysokosc];
            for (int j = 0; j<wysokosc; j++)
            {
                tablicaPixeli[i][j] = 0;
            }
    }
}

WyswietlaczPixele::~WyswietlaczPixele()
{
    for (int i = 0; i<szerokosc; i++)
    {
        delete [] tablicaPixeli[i];
    }
}

void WyswietlaczPixele::uzupelnijPixel(int x, int y, bool czySwieci)
{
    tablicaPixeli[x][y] = czySwieci;
}


bool WyswietlaczPixele::getPixel(int x, int y)
{
    return tablicaPixeli[x][y];
}
