#ifndef GRAFIKAIMAGE_H
#define GRAFIKAIMAGE_H

#include<QImage>

class GrafikaImage : public QImage
{
public:
    GrafikaImage();
    bool wczytajGrafike(QString nazwaPliku);

    int *zwrocTabliceRGB(int x, int y);

};

#endif // GRAFIKAIMAGE_H
