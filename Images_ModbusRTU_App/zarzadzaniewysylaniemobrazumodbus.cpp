#include "zarzadzaniewysylaniemobrazumodbus.h"


ZarzadzanieWysylaniemObrazuModbus::ZarzadzanieWysylaniemObrazuModbus(int szerokosc, int wysokosc)
{
    this->szerokosc = szerokosc;
    this->wysokosc = wysokosc;
    modbusMaster = new ModbusMaster();
    przerwacWysylanie = false;
    przerwacWysylanieDlaAktualnegoUrzadzenia = false;
    czyWysylam = false;
    connect(modbusMaster,SIGNAL(brakOdpowiedziSlave(int)),this,SLOT(otrzymanieBrakOdpowiedziSlave(int)));
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

ZarzadzanieWysylaniemObrazuModbus::~ZarzadzanieWysylaniemObrazuModbus()
{
    for (int i = 0; i<szerokosc; i++)
    {
        delete [] tablicaPixeli[i];
    }
    delete modbusMaster;
}

void ZarzadzanieWysylaniemObrazuModbus::uzupelnijPixel(int x, int y, bool czySwieci)
{
    tablicaPixeli[x][y] = czySwieci;
}

unsigned char *ZarzadzanieWysylaniemObrazuModbus::stworzWiadomosc(int rzad)
{
    unsigned char *wiadomosc = new unsigned char [szerokosc/8];
    for( int g = 0; g < szerokosc/8; g ++)
    {
        wiadomosc[g] = 0;
        for(int k = 0; k <8; k ++)
        {
            unsigned char d = tablicaPixeli[g*8 + k][rzad];
            d<<=(7-k);
            wiadomosc[g] = wiadomosc[g]|d;
        }
    }
    return wiadomosc;
}

bool ZarzadzanieWysylaniemObrazuModbus::ustawieniaPortModbus(std::string port)
{
    return modbusMaster->ustawieniaPort(port);
}

bool ZarzadzanieWysylaniemObrazuModbus::ustawieniaPrzesyluModbus(std::string predkoscTransmisji, std::string liczbaBitow, std::string parity)
{
    return modbusMaster->ustawieniaPrzesylu(predkoscTransmisji,liczbaBitow,parity);
}

void ZarzadzanieWysylaniemObrazuModbus::rozlaczModbus()
{

    modbusMaster->rozlacz();
}



void ZarzadzanieWysylaniemObrazuModbus::otrzymanieBrakOdpowiedziSlave(int adresUrzadzenia)
{
    przerwacWysylanie = true;
    emit brakOdpowiedziSlave(adresUrzadzenia);
}

void ZarzadzanieWysylaniemObrazuModbus::setPrzerwacWysylanieDlaAktualnegoUrzadzenia(bool value)
{
    przerwacWysylanieDlaAktualnegoUrzadzenia = value;
}

void ZarzadzanieWysylaniemObrazuModbus::setPrzerwacWysylanie(bool value)
{
    przerwacWysylanie = value;
}

void ZarzadzanieWysylaniemObrazuModbus::setCoRobicAktualnie(const coRobic &value)
{
    coRobicAktualnie = value;
    przerwacWysylanie = false;
    przerwacWysylanieDlaAktualnegoUrzadzenia = false;
}

void ZarzadzanieWysylaniemObrazuModbus::zmianaWielkosciWyswietlacza(int szerokosc, int wysokosc)
{
    for (int i = 0; i<this->szerokosc; i++)
    {
        delete [] tablicaPixeli[i];
    }
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

void ZarzadzanieWysylaniemObrazuModbus::setAdresUrzadzenia(int value)
{
    adresUrzadzenia = value;
}

void ZarzadzanieWysylaniemObrazuModbus::run()
{
    int adresUrzadeniaWiadomosci = adresUrzadzenia;
    if(adresUrzadeniaWiadomosci == 0)
    {
        QThread::currentThread()->msleep(20);
    }
    czyWysylam = true;
    int wysokoscWyswietlacza = wysokosc;

    if(coRobicAktualnie == wlaczycTrybGraficzny)
    {
        zwrotWyslaniaRamki zwrot;
        int licznik = 0;
        do
        {
            zwrot = modbusMaster->wlaczTrybGraficzny(adresUrzadeniaWiadomosci,1);
            licznik++;
            if(zwrot != wykonano && licznik < 3 && !(przerwacWysylanie || (przerwacWysylanieDlaAktualnegoUrzadzenia && adresUrzadeniaWiadomosci == adresUrzadzenia)))
            {
                QThread::currentThread()->msleep(50);
            }
        }while(zwrot == bladPolaczeniaZWyswietlaczem && licznik < 3 && !(przerwacWysylanie || (przerwacWysylanieDlaAktualnegoUrzadzenia && adresUrzadeniaWiadomosci == adresUrzadzenia)));

        if(zwrot == bladPolaczeniaZWyswietlaczem)
        {
            emit brakOdpowiedziSlave(adresUrzadeniaWiadomosci);
        }
        else if(zwrot == bladPolaczeniaCom)
        {
            emit brakPolaczeniaCom();
        }
    }

    else if(coRobicAktualnie == wylaczycTrybGraficzny)
    {
        zwrotWyslaniaRamki zwrot;
        int licznik = 0;
        do
        {
            zwrot = modbusMaster->wlaczTrybGraficzny(adresUrzadeniaWiadomosci,0);
            licznik++;
            if(zwrot != wykonano && licznik < 3 && !(przerwacWysylanie || (przerwacWysylanieDlaAktualnegoUrzadzenia && adresUrzadeniaWiadomosci == adresUrzadzenia)))
            {
                QThread::currentThread()->msleep(50);
            }
        }while(zwrot == bladPolaczeniaZWyswietlaczem && licznik < 3 && !(przerwacWysylanie || (przerwacWysylanieDlaAktualnegoUrzadzenia && adresUrzadeniaWiadomosci == adresUrzadzenia)));

        if(zwrot == bladPolaczeniaZWyswietlaczem)
        {
            emit brakOdpowiedziSlave(adresUrzadeniaWiadomosci);
        }
        else if(zwrot == bladPolaczeniaCom)
        {
            emit brakPolaczeniaCom();
        }
    }

    else if(coRobicAktualnie == wysylacObraz)
    {
        unsigned char **tablicaWiadomosci = new unsigned char*[wysokoscWyswietlacza];
        for (int i = 0; i<wysokoscWyswietlacza; i++)
        {
                tablicaWiadomosci[i] = stworzWiadomosc(i);
        }
        for(int i = 0; i < wysokoscWyswietlacza; i++)
        {
            zwrotWyslaniaRamki zwrot;
            int licznik = 0;
            do
            {
                zwrot = modbusMaster->zaswiec(adresUrzadeniaWiadomosci,i,tablicaWiadomosci[i],szerokosc/8);
                licznik++;
                if(zwrot != wykonano && licznik < 3 && !(przerwacWysylanie || (przerwacWysylanieDlaAktualnegoUrzadzenia && adresUrzadeniaWiadomosci == adresUrzadzenia)))
                {
                    QThread::currentThread()->msleep(50);
                }
            }while(zwrot == bladPolaczeniaZWyswietlaczem && licznik < 3 && !(przerwacWysylanie || (przerwacWysylanieDlaAktualnegoUrzadzenia && adresUrzadeniaWiadomosci == adresUrzadzenia)));

            if(zwrot == bladPolaczeniaZWyswietlaczem)
            {
                emit brakOdpowiedziSlave(adresUrzadeniaWiadomosci);
                break;
            }
            else if(zwrot == bladPolaczeniaCom)
            {
                emit brakPolaczeniaCom();
                break;
            }
            if(przerwacWysylanie || (przerwacWysylanieDlaAktualnegoUrzadzenia && adresUrzadeniaWiadomosci == adresUrzadzenia))
            {
                break;
            }
            if(adresUrzadeniaWiadomosci != 0)
            {
                QThread::currentThread()->msleep(10);
            }
            else
            {
                QThread::currentThread()->msleep(40);
            }

        }
        for (int i = 0; i <wysokoscWyswietlacza; i++)
        {
              delete [] tablicaWiadomosci[i];
        }
    }
    else
    {
        bool czyPierwszeWyswietlenie = true;

        unsigned char **tablicaPoprzednichWiadomosci = new unsigned char*[wysokoscWyswietlacza];
        for (int i = 0; i<wysokoscWyswietlacza; i++)
        {
                tablicaPoprzednichWiadomosci[i] = new unsigned char[szerokosc/8];
        }
        zwrotWyslaniaRamki zwrot = wykonano;
        while(!(przerwacWysylanie || (przerwacWysylanieDlaAktualnegoUrzadzenia && adresUrzadeniaWiadomosci == adresUrzadzenia) || zwrot != wykonano))
        {
            QThread::currentThread()->msleep(40);
            for(int i = 0; i < wysokoscWyswietlacza; i++)
            {
                bool wyslac = false;
                unsigned char *wiadomosc = stworzWiadomosc(i);
                for(int h = 0; h < szerokosc/8; h ++)
                {
                    if(wiadomosc[h] != tablicaPoprzednichWiadomosci[i][h])
                    {
                        wyslac = true;
                    }
                }
                if(wyslac == true || czyPierwszeWyswietlenie ==true)
                {
                    int licznik = 0;
                    do
                    {
                        zwrot = modbusMaster->zaswiec(adresUrzadeniaWiadomosci,i,wiadomosc,szerokosc/8);
                        licznik++;
                        if(zwrot != wykonano && licznik < 3 && !(przerwacWysylanie || (przerwacWysylanieDlaAktualnegoUrzadzenia && adresUrzadeniaWiadomosci == adresUrzadzenia)))
                        {
                            QThread::currentThread()->msleep(50);
                        }
                    }while(zwrot == bladPolaczeniaZWyswietlaczem && licznik < 3 && !(przerwacWysylanie || (przerwacWysylanieDlaAktualnegoUrzadzenia && adresUrzadeniaWiadomosci == adresUrzadzenia)));
                    for(int h = 0; h < szerokosc/8; h ++)
                    {
                        tablicaPoprzednichWiadomosci[i][h] = wiadomosc[h];
                    }   
                }
                delete []wiadomosc;
                if(przerwacWysylanie || (przerwacWysylanieDlaAktualnegoUrzadzenia && adresUrzadeniaWiadomosci == adresUrzadzenia))
                {
                    break;
                }
                else if(zwrot == bladPolaczeniaZWyswietlaczem)
                {
                    emit brakOdpowiedziSlave(adresUrzadeniaWiadomosci);
                    break;
                }
                else if(zwrot == bladPolaczeniaCom)
                {
                    emit brakPolaczeniaCom();
                    break;
                }  
                if(adresUrzadeniaWiadomosci != 0)
                {
                    QThread::currentThread()->msleep(10);
                }
                else
                {
                    QThread::currentThread()->msleep(40);
                }
            }
            czyPierwszeWyswietlenie = false;

        }
        for (int i = 0; i <wysokoscWyswietlacza; i++)
        {
            delete [] tablicaPoprzednichWiadomosci[i];
        }
    }
    emit finished(adresUrzadeniaWiadomosci);
    czyWysylam = false;
}
