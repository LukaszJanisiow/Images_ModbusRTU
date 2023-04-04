#include "wyswietlaczgrafika.h"


WyswietlaczGrafika::WyswietlaczGrafika(int szerokosc, int wysokosc)
{
    this->szerokosc = szerokosc;
    this->wysokosc = wysokosc;

    tablicaPixeli = new RGBS*[szerokosc];
    for (int i = 0; i<szerokosc; i++)
    {
            tablicaPixeli[i] = new RGBS [wysokosc];
    }

}

void WyswietlaczGrafika::uzupelnijKolor(int x, int y, int tablicaRGB[3])
{
    tablicaPixeli[x][y].red = tablicaRGB[0];
    tablicaPixeli[x][y].green = tablicaRGB[1];
    tablicaPixeli[x][y].blue = tablicaRGB[2];
    tablicaPixeli[x][y].skalaSzarosci = 0.299*tablicaPixeli[x][y].red + 0.587*tablicaPixeli[x][y].green + 0.114*tablicaPixeli[x][y].blue;
    tablicaPixeli[x][y].czarnobialy = false;
    delete tablicaRGB;

}

void WyswietlaczGrafika::skalowanie(int naSzerokosc, int naWysokosc, int metoda, int poziomCzarnoBialy, bool zachowanieProporcji)
{
    RGBS **wskaznikpomocniczy;
    double skalaSzerokosc = double(szerokosc)/naSzerokosc;
    double skalaWysokosc = double(wysokosc)/naWysokosc;
    int skalaSrednia = round((skalaSzerokosc + skalaWysokosc)/2);
    if(zachowanieProporcji)
    {
        if(skalaSzerokosc > skalaWysokosc)
        {
            naWysokosc = round(wysokosc/skalaSzerokosc);
            skalaSrednia = skalaSzerokosc;
        }
        else if(skalaSzerokosc < skalaWysokosc)
        {
            naSzerokosc = round(szerokosc/skalaWysokosc);
            skalaSrednia = skalaWysokosc;
        }

    }


    if(metoda == 0)                                                     // metoda najbliższego sąsiada
    {
        wskaznikpomocniczy = skalowanieNajblizszegoSasiada(naSzerokosc,naWysokosc);
    }
    else if( metoda == 1)                                                          // metoda biliniowa (byl problem z ostatnim pikselem na dole i po prawej)
    {
        wskaznikpomocniczy = skalowanieBiliniowe(naSzerokosc,naWysokosc);

    }
    else if(metoda == 2)                                                                           // metoda bicubic
    {
        wskaznikpomocniczy = skalowanieBicubic(naSzerokosc,naWysokosc);
    }
    else if(metoda == 3)
    {
        wskaznikpomocniczy = skalowanieBoxSampling(naSzerokosc,naWysokosc,skalaSrednia);
    }
    else
    {
        if(wysokosc > naWysokosc && szerokosc > naSzerokosc)
        {
            wskaznikpomocniczy = metodaAutorska(naSzerokosc,naWysokosc);
        }
        else
        {
            wskaznikpomocniczy = skalowanieNajblizszegoSasiada(naSzerokosc,naWysokosc);
        }

    }


    for (int i = 0; i<szerokosc; i++)
    {
            delete [] tablicaPixeli[i];
    }
    delete [] tablicaPixeli;
    tablicaPixeli = wskaznikpomocniczy;
    szerokosc = naSzerokosc;
    wysokosc = naWysokosc;

    this->uzupelnijCzarnoBialy(poziomCzarnoBialy);


}

RGBS **WyswietlaczGrafika::skalowanieNajblizszegoSasiada(int naSzerokosc, int naWysokosc)
{
    RGBS **tablicapomocnicza = new RGBS*[naSzerokosc];                                                      //stworzenie tablicy pomocniczej o wielkości obrazu wyjściowego
    for (int i = 0; i<naSzerokosc; i++)
                tablicapomocnicza[i] = new RGBS [naWysokosc];

    for(int i = 0; i<naSzerokosc; i++)                                                                      //przejście po wszsytkich pikselach obrazu wyjściowego
    {
        for(int j = 0; j<naWysokosc; j++)
        {
            int wspolrzednax = round(i*1.0/(naSzerokosc*1.0 - 1.0 )*(szerokosc*1.0 -1.0));                  //przeskalowanie punktu z obrazu wyjściowego na obraz wejściowy i znalezienie jego najbliższego sąsiada
            int wspolrzednay = round(j*1.0/(naWysokosc*1.0 - 1.0 )*(wysokosc*1.0 -1.0));

            tablicapomocnicza[i][j] = tablicaPixeli[wspolrzednax][wspolrzednay];                            //przypisanie wartości odpowiedniego punktu na obrazie wejściowym danemu pikselowi na obrazie wyjściowym
        }
    }

    return tablicapomocnicza;
}

RGBS **WyswietlaczGrafika::skalowanieBiliniowe(int naSzerokosc, int naWysokosc)
{
    RGBS **tablicapomocnicza = new RGBS*[naSzerokosc];
    for (int i = 0; i<naSzerokosc; i++)
                tablicapomocnicza[i] = new RGBS [naWysokosc];

    for(int i = 0; i<naSzerokosc; i++)
    {
        for(int j = 0; j<naWysokosc; j++)
        {
            double wspolrzednaX = i*1.0/(naSzerokosc*1.0 - 1.0 )*(szerokosc*1.0 -1.0);
            double wspolrzednaY = j*1.0/(naWysokosc*1.0 - 1.0 )*(wysokosc*1.0 -1.0);

            double wspolczynnikpoziomy =  wspolrzednaX - floor(wspolrzednaX);
            double wspolczynnikpionowy =  wspolrzednaY - floor(wspolrzednaY);

            int wspolrzednaXCalkowita = floor(wspolrzednaX);
            int wspolrzednaYCalkowita = floor(wspolrzednaY);


            RGBS wartosc00 = pobierzWartoscKolorow(wspolrzednaXCalkowita, wspolrzednaYCalkowita);
            RGBS wartosc10 = pobierzWartoscKolorow(wspolrzednaXCalkowita + 1, wspolrzednaYCalkowita);
            RGBS wartosc01 = pobierzWartoscKolorow(wspolrzednaXCalkowita, wspolrzednaYCalkowita + 1);
            RGBS wartosc11 = pobierzWartoscKolorow(wspolrzednaXCalkowita + 1, wspolrzednaYCalkowita + 1);


            RGBS interpolacjaPionowa = interpolacjaBiliniowa(interpolacjaBiliniowa(wartosc00,wartosc10,wspolczynnikpoziomy),interpolacjaBiliniowa(wartosc01,wartosc11,wspolczynnikpoziomy),wspolczynnikpionowy);

            tablicapomocnicza[i][j] = interpolacjaPionowa;
        }
    }
    return tablicapomocnicza;
}

RGBS WyswietlaczGrafika::interpolacjaBiliniowa(RGBS wartosc1, RGBS wartosc2, double wspolczynnik)
{
    RGBS punkt;
    punkt.red = round((1 - wspolczynnik)*wartosc1.red + wspolczynnik*wartosc2.red);
    punkt.green = round((1 - wspolczynnik)*wartosc1.green + wspolczynnik*wartosc2.green);
    punkt.blue = round((1 - wspolczynnik)*wartosc1.blue + wspolczynnik*wartosc2.blue);
    punkt.skalaSzarosci = round((1 - wspolczynnik)*wartosc1.skalaSzarosci + wspolczynnik*wartosc2.skalaSzarosci);

    return  punkt;
}

RGBS **WyswietlaczGrafika::skalowanieBicubic(int naSzerokosc, int naWysokosc)
{
    RGBS **tablicapomocnicza = new RGBS*[naSzerokosc];
    for (int i = 0; i<naSzerokosc; i++)
                tablicapomocnicza[i] = new RGBS [naWysokosc];

    for(int i = 0; i<naSzerokosc; i++)
    {
        for(int j = 0; j<naWysokosc; j++)
        {
            double wspolrzednaX = i*1.0/(naSzerokosc*1.0 - 1.0 )*(szerokosc*1.0 -1.0);
            double wspolrzednaY = j*1.0/(naWysokosc*1.0 - 1.0 )*(wysokosc*1.0 -1.0);

            int wspolrzednaXCalkowita = floor(wspolrzednaX);
            int wspolrzednaYCalkowita = floor(wspolrzednaY);

            RGBS **tablicaKolorow;

            tablicaKolorow = pobieranieWartosciKolorowBicubic(wspolrzednaXCalkowita - 1,wspolrzednaYCalkowita- 1);

            RGBS tablicaInterpolacji[4];
            for(int a = 0; a < 4; a++)
            {
                tablicaInterpolacji[a] = interpolacjaBicubic(tablicaKolorow[a],wspolrzednaX - wspolrzednaXCalkowita );
            }

            tablicapomocnicza[i][j] = interpolacjaBicubic(tablicaInterpolacji,wspolrzednaY - wspolrzednaYCalkowita );

            for(int a = 0; a < 4; a++)
            {
                delete []tablicaKolorow[a];
            }

        }
    }


    return tablicapomocnicza;
}


RGBS **WyswietlaczGrafika::pobieranieWartosciKolorowBicubic(int x, int y)
{
    RGBS **tablicapomocnicza = new RGBS*[4];
    for (int i = 0; i<4; i++)
                tablicapomocnicza[i] = new RGBS [4];
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            tablicapomocnicza[i][j] = pobierzWartoscKolorow(x + i, y + j);
        }
    }
    return tablicapomocnicza;
}

RGBS WyswietlaczGrafika::interpolacjaBicubic(RGBS *tablicaKolorow, double wspolrzedna)
{
    RGBS punkt;
    punkt.red = (int)((-1.0/2.0*tablicaKolorow[0].red+3.0/2.0*tablicaKolorow[1].red-3.0/2.0*tablicaKolorow[2].red+1.0/2.0*tablicaKolorow[3].red)* wspolrzedna*wspolrzedna*wspolrzedna + (tablicaKolorow[0].red - 5.0/2.0 * tablicaKolorow[1].red + 2.0 * tablicaKolorow[2].red - 1.0/2.0 *tablicaKolorow[3].red)*wspolrzedna*wspolrzedna + ((tablicaKolorow[2].red - tablicaKolorow[0].red)/2.0)*wspolrzedna + tablicaKolorow[1].red);
    punkt.green = (int)((-1.0/2.0*tablicaKolorow[0].green +3.0/2.0*tablicaKolorow[1].green-3.0/2.0*tablicaKolorow[2].green+1.0/2.0*tablicaKolorow[3].green)* wspolrzedna*wspolrzedna*wspolrzedna + (tablicaKolorow[0].green - 5.0/2.0 * tablicaKolorow[1].green + 2.0 * tablicaKolorow[2].green - 1.0/2.0 *tablicaKolorow[3].green)*wspolrzedna*wspolrzedna + ((tablicaKolorow[2].green - tablicaKolorow[0].green)/2.0)*wspolrzedna + tablicaKolorow[1].green);
    punkt.blue = (int)((-1.0/2.0*tablicaKolorow[0].blue+3.0/2.0*tablicaKolorow[1].blue-3.0/2.0*tablicaKolorow[2].blue+1.0/2.0*tablicaKolorow[3].blue)* wspolrzedna*wspolrzedna*wspolrzedna + (tablicaKolorow[0].blue - 5.0/2.0 * tablicaKolorow[1].blue + 2.0 * tablicaKolorow[2].blue - 1.0/2.0 *tablicaKolorow[3].blue)*wspolrzedna*wspolrzedna + ((tablicaKolorow[2].blue - tablicaKolorow[0].blue)/2.0)*wspolrzedna + tablicaKolorow[1].blue);
    punkt.skalaSzarosci = (int)((-1.0/2.0*tablicaKolorow[0].skalaSzarosci+3.0/2.0*tablicaKolorow[1].skalaSzarosci-3.0/2.0*tablicaKolorow[2].skalaSzarosci+1.0/2.0*tablicaKolorow[3].skalaSzarosci)* wspolrzedna*wspolrzedna*wspolrzedna + (tablicaKolorow[0].skalaSzarosci - 5.0/2.0 * tablicaKolorow[1].skalaSzarosci + 2.0 * tablicaKolorow[2].skalaSzarosci - 1.0/2.0 *tablicaKolorow[3].skalaSzarosci)*wspolrzedna*wspolrzedna + ((tablicaKolorow[2].skalaSzarosci - tablicaKolorow[0].skalaSzarosci)/2.0)*wspolrzedna + tablicaKolorow[1].skalaSzarosci);

    if(punkt.skalaSzarosci > 255)
    {
        punkt.skalaSzarosci = 255;
    }
    else if(punkt.skalaSzarosci < 0)
    {
        punkt.skalaSzarosci = 0;
    }
    if(punkt.red > 255)
    {
        punkt.red = 255;
    }
    else if(punkt.red < 0)
    {
        punkt.red = 0;
    }

    if(punkt.green > 255)
    {
        punkt.green = 255;
    }
    else if(punkt.green < 0)
    {
        punkt.green = 0;
    }

    if(punkt.blue > 255)
    {
        punkt.blue = 255;
    }
    else if(punkt.blue < 0)
    {
        punkt.blue = 0;
    }
    return punkt;
}



RGBS **WyswietlaczGrafika::metodaAutorska(int naSzerokosc, int naWysokosc)
{
    int tablicapomocnicza2[naSzerokosc][naWysokosc];

    RGBS **tablicapomocnicza = new RGBS*[naSzerokosc];
    for (int i = 0; i<naSzerokosc; i++)
    {
        tablicapomocnicza[i] = new RGBS [naWysokosc];
        for(int j = 0; j<naWysokosc; j++)
        {
            tablicapomocnicza2[i][j] = 0;
        }
    }

    for(int i = 0; i<szerokosc; i++)
    {
        for(int j = 0; j<wysokosc; j++)
        {

            double wspolrzednaX = i*1.0/(szerokosc*1.0 - 1.0 )*(naSzerokosc*1.0 - 1.0);
            double wspolrzednaY = j*1.0/(wysokosc*1.0 - 1.0 )*(naWysokosc*1.0 - 1.0);

            int wspolrzednaXCalkowita = round(wspolrzednaX);
            int wspolrzednaYCalkowita = round(wspolrzednaY);

            tablicapomocnicza[wspolrzednaXCalkowita][wspolrzednaYCalkowita].red = (tablicaPixeli[i][j].red + tablicapomocnicza[wspolrzednaXCalkowita][wspolrzednaYCalkowita].red*tablicapomocnicza2[wspolrzednaXCalkowita][wspolrzednaYCalkowita])/(tablicapomocnicza2[wspolrzednaXCalkowita][wspolrzednaYCalkowita] + 1);
            tablicapomocnicza[wspolrzednaXCalkowita][wspolrzednaYCalkowita].green = (tablicaPixeli[i][j].green + tablicapomocnicza[wspolrzednaXCalkowita][wspolrzednaYCalkowita].green*tablicapomocnicza2[wspolrzednaXCalkowita][wspolrzednaYCalkowita])/(tablicapomocnicza2[wspolrzednaXCalkowita][wspolrzednaYCalkowita] + 1);
            tablicapomocnicza[wspolrzednaXCalkowita][wspolrzednaYCalkowita].blue = (tablicaPixeli[i][j].blue + tablicapomocnicza[wspolrzednaXCalkowita][wspolrzednaYCalkowita].blue*tablicapomocnicza2[wspolrzednaXCalkowita][wspolrzednaYCalkowita])/(tablicapomocnicza2[wspolrzednaXCalkowita][wspolrzednaYCalkowita] + 1);
            tablicapomocnicza[wspolrzednaXCalkowita][wspolrzednaYCalkowita].skalaSzarosci = (tablicaPixeli[i][j].skalaSzarosci + tablicapomocnicza[wspolrzednaXCalkowita][wspolrzednaYCalkowita].skalaSzarosci*tablicapomocnicza2[wspolrzednaXCalkowita][wspolrzednaYCalkowita])/(tablicapomocnicza2[wspolrzednaXCalkowita][wspolrzednaYCalkowita] + 1);
            tablicapomocnicza2[wspolrzednaXCalkowita][wspolrzednaYCalkowita]++;
        }
    }
    return tablicapomocnicza;
}

RGBS **WyswietlaczGrafika::skalowanieBoxSampling(int naSzerokosc, int naWysokosc,int skalaSrednia)
{
    int dwaR = 1;
    if(skalaSrednia > 1)
    {
       skalaSrednia = skalaSrednia/2;
       dwaR = skalaSrednia*2;
    }
    RGBS **tablicapomocnicza = new RGBS*[naSzerokosc];
    for (int i = 0; i<naSzerokosc; i++)
                tablicapomocnicza[i] = new RGBS [naWysokosc];

    for(int i = 0; i<naSzerokosc; i++)
    {
        for(int j = 0; j<naWysokosc; j++)
        {
            int wspolrzednax = round(i*1.0/(naSzerokosc*1.0 - 1.0 )*(szerokosc*1.0 -1.0));
            int wspolrzednay = round(j*1.0/(naWysokosc*1.0 - 1.0 )*(wysokosc*1.0 -1.0));

            RGBS *tablicawartosci = new RGBS[dwaR*dwaR];
            for(int a = 0; a < dwaR; a ++ )
            {
                for(int b = 0; b < dwaR; b ++ )
                {
                    tablicawartosci[a*dwaR+b] = pobierzWartoscKolorow(wspolrzednax + a - dwaR/2 +1,wspolrzednay +b - dwaR/2 + 1);

                }
            }
            RGBS kolor;
            for(int a = 0; a < dwaR*dwaR; a ++ )
            {
                if(a == 0)
                {
                    kolor.red = tablicawartosci[a].red;
                    kolor.green = tablicawartosci[a].green;
                    kolor.blue = tablicawartosci[a].blue;
                    kolor.skalaSzarosci = tablicawartosci[a].skalaSzarosci;
                }
                else
                {
                    kolor.red = (kolor.red*a + tablicawartosci[a].red)/(a+1);
                    kolor.green = (kolor.green*a + tablicawartosci[a].green)/(a+1);
                    kolor.blue = (kolor.blue*a + tablicawartosci[a].blue)/(a+1);
                    kolor.skalaSzarosci = (kolor.skalaSzarosci*a + tablicawartosci[a].skalaSzarosci)/(a+1);
                }
            }
            tablicapomocnicza[i][j].red = kolor.red;
            tablicapomocnicza[i][j].green = kolor.green;
            tablicapomocnicza[i][j].blue = kolor.blue;
            tablicapomocnicza[i][j].skalaSzarosci = kolor.skalaSzarosci;

            delete [] tablicawartosci;
        }
    }

    return tablicapomocnicza;
}

RGBS WyswietlaczGrafika::pobierzWartoscKolorow(int x, int y)
{
    int wspX;
    int wspY;
    if( x - 1 < 0)
    {
        wspX = 0;
    }
    else if(x - 1 > szerokosc - 1)
    {
        wspX = szerokosc -1;
    }
    else
    {
        wspX = x - 1;
    }

    if( y - 1 < 0)
    {
        wspY = 0;
    }
    else if(y - 1  > wysokosc - 1)
    {
        wspY = wysokosc -1;
    }
    else
    {
        wspY = y - 1;
    }
    return tablicaPixeli[wspX][wspY];
}



void WyswietlaczGrafika::uzupelnijCzarnoBialy(int poziom)
{
    for(int i = 0; i<szerokosc; i++)
    {
        for(int j = 0; j<wysokosc; j++)
        {
            if(tablicaPixeli[i][j].skalaSzarosci <= poziom)
            {
                tablicaPixeli[i][j].czarnobialy = true;
            }
            else
            {
                tablicaPixeli[i][j].czarnobialy = false;
            }
        }
    }
}

bool WyswietlaczGrafika::czyCzarnoBialy(int x, int y)
{
    return tablicaPixeli[x][y].czarnobialy;
}

int WyswietlaczGrafika::uzupelnienieCzarnoBialyAutomatycznyProg()
{
    int prog = metodaOtsu();
    uzupelnijCzarnoBialy(prog);
    return prog;
}

WyswietlaczGrafika::~WyswietlaczGrafika()
{
        for (int i = 0; i<szerokosc; i++)
        {
            delete [] tablicaPixeli[i];
        }
}

int WyswietlaczGrafika::getSzerokosc() const
{
    return szerokosc;
}

int WyswietlaczGrafika::getWysokosc() const
{
    return wysokosc;
}

int WyswietlaczGrafika::metodaOtsu()
{
    long double histogram[256];
    for(int i = 0; i < 256; i++)
    {
        histogram[i] = 0;
    }
    float suma = 0;
    for(int i = 0; i < szerokosc; i++)
    {
        for(int j = 0; j < wysokosc; j++)
        {
            histogram[tablicaPixeli[i][j].skalaSzarosci]++;
            suma = suma + tablicaPixeli[i][j].skalaSzarosci;
        }
    }

    double iloscCalkowitaProbek = wysokosc * szerokosc;
    float iloscProbekPierwszegoPlanu = 0;
    float iloscProbekTla = 0;
    float sumaWartosciProbekPP = 0;
    float sredniaWartoscProbekPP = 0;
    float sredniaWartoscProbekTla =0;
    float wariancjaMiedzyKlasowa = 0;

    float maxWariancja = 0;
    float prog = 0;
    for(int i = 0; i < 256; i++)
    {
        iloscProbekPierwszegoPlanu = iloscProbekPierwszegoPlanu + histogram[i];
        if(iloscProbekPierwszegoPlanu == 0)
        {
            continue;
        }
        iloscProbekTla = iloscCalkowitaProbek -iloscProbekPierwszegoPlanu;

        sumaWartosciProbekPP = sumaWartosciProbekPP + i*histogram[i];

        sredniaWartoscProbekPP = sumaWartosciProbekPP/iloscProbekPierwszegoPlanu;
        sredniaWartoscProbekTla=(suma - sumaWartosciProbekPP)/iloscProbekTla;

        wariancjaMiedzyKlasowa = iloscProbekPierwszegoPlanu*iloscProbekTla*(sredniaWartoscProbekPP-sredniaWartoscProbekTla)*(sredniaWartoscProbekPP-sredniaWartoscProbekTla);
        if(wariancjaMiedzyKlasowa > maxWariancja)
        {
            maxWariancja = wariancjaMiedzyKlasowa;
            prog = i;
        }
    }
    return prog;

}
