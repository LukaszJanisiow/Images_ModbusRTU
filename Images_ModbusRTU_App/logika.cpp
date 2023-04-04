#include "logika.h"

Logika::Logika(QObject *parent) : QObject(parent)
{

    szerokoscWyswietlacza = 128;
    wysokoscWyswietlacza = 64;
    metodaSkalowania = autorska;
    czyBylaWczytanaGrafika = false;
    czyPolaczenieZModbusem = false;
    dialogOknoOtwarte = false;
    progskaliSzarosci = 127;
    zachowacProporcje = false;
    iloscWlaczonychWatkow = 0;
    maksymalnaIloscWlaczonychWatkow = QThread::idealThreadCount();
    wyswietlaczPixele = new WyswietlaczPixele(szerokoscWyswietlacza,wysokoscWyswietlacza);
    grafikaImage = new GrafikaImage();
    tablicaUrzadzenPracujacych = new coRobiUrzadzenie[256];
    for(int i = 0; i < 256; i++)
    {
        tablicaUrzadzenPracujacych[i] = nic;
    }


}

Logika::~Logika()
{


    delete grafikaImage;
    if(czyBylaWczytanaGrafika)
    {
        delete wyswietlaczGrafika;
    }

    delete wyswietlaczPixele;


    if(czyPolaczenieZModbusem)
    {
        zarzadzanieWysylaniemObrazuModbus->setPrzerwacWysylanie(true);
        QThreadPool::globalInstance()->waitForDone(-1);

        delete zarzadzanieWysylaniemObrazuModbus;
    }
}

bool Logika::wczytajGrafike(QString nazwaPliku)
{
    return grafikaImage->wczytajGrafike(nazwaPliku);
}

void Logika::przyciskPolaczModbus()
{
    dialogOknoOtwarte = true;
    emit otworzOknoPolaczenieModbus();
}

void Logika::polaczModbus(ParametryModbus pM)
{

    this->pM = pM;
    zarzadzanieWysylaniemObrazuModbus = new ZarzadzanieWysylaniemObrazuModbus(szerokoscWyswietlacza,wysokoscWyswietlacza);
    zarzadzanieWysylaniemObrazuModbus->setAutoDelete(false);
    czyPolaczenieZModbusem = true;
    connect(zarzadzanieWysylaniemObrazuModbus,SIGNAL(finished(int)),this,SLOT(watekWysylaniaModbusZakonczony(int)));
    connect(zarzadzanieWysylaniemObrazuModbus,SIGNAL(brakOdpowiedziSlave(int)),this,SLOT(brakOdpowiedziSlave(int)));
    connect(zarzadzanieWysylaniemObrazuModbus,SIGNAL(brakPolaczeniaCom()),this,SLOT(brakPolaczeniaZPortemCom()));
    if(zarzadzanieWysylaniemObrazuModbus->ustawieniaPortModbus(pM.port))
    {
        if(zarzadzanieWysylaniemObrazuModbus->ustawieniaPrzesyluModbus(pM.predkoscTransmisji,pM.liczbaBitow,pM.parity))
        {
            emit sygnalpolaczonolubRozlaczonoZModbusem(true);
        }
        else
        {
            emit sygnalOtworzenieOknaModbusPortProblem(pM.port);
        }

    }
    else
    {
        emit sygnalOtworzenieOknaModbusPortProblem(pM.port);
    }
    emit zamknijOknoDialogowe();
}

void Logika::edytujModbus(ParametryModbus pM)
{
    this->pM.predkoscTransmisji = pM.predkoscTransmisji;
    this->pM.liczbaBitow = pM.liczbaBitow;
    this->pM.parity = pM.parity;
    zarzadzanieWysylaniemObrazuModbus->ustawieniaPrzesyluModbus(pM.predkoscTransmisji,pM.liczbaBitow,pM.parity);
    emit zamknijOknoDialogowe();
}


void Logika::przyciskRozlaczModbus()
{
    zarzadzanieWysylaniemObrazuModbus->setPrzerwacWysylanie(true);
    QThreadPool::globalInstance()->waitForDone(-1);
    zarzadzanieWysylaniemObrazuModbus->rozlaczModbus();
    delete zarzadzanieWysylaniemObrazuModbus;
    czyPolaczenieZModbusem = false;
    emit sygnalpolaczonolubRozlaczonoZModbusem(false);

}

void Logika::przyciskEdytujModbus()
{

    if(iloscWlaczonychWatkow != 0)
    {
        dialogOknoOtwarte = true;
        emit otworzOknoEdycjiModbus(pM.predkoscTransmisji,pM.liczbaBitow,pM.parity,true);
    }
    else
    {
        dialogOknoOtwarte = true;
        emit otworzOknoEdycjiModbus(pM.predkoscTransmisji,pM.liczbaBitow,pM.parity,false);

    }
    dialogOknoOtwarte = true;

}

void Logika::przyciskWyswietlaczOpcje()
{
    if(iloscWlaczonychWatkow != 0)
    {
        dialogOknoOtwarte = true;
        emit otworzOknoOpcjiWyswietlacza(szerokoscWyswietlacza,wysokoscWyswietlacza,true);
    }
    else
    {
        dialogOknoOtwarte = true;
        emit otworzOknoOpcjiWyswietlacza(szerokoscWyswietlacza,wysokoscWyswietlacza,false);
    }
}

void Logika::zmianaWymiarowWyswietlacz(int szerokosc,int wysokosc)
{
    if(this->szerokoscWyswietlacza != szerokosc || this->wysokoscWyswietlacza != wysokosc)
    {
        if(czyPolaczenieZModbusem)
        {
            zarzadzanieWysylaniemObrazuModbus->setPrzerwacWysylanie(true);
        }
        this->szerokoscWyswietlacza = szerokosc;
        this->wysokoscWyswietlacza = wysokosc;
        emit sygnalzmienWymiaryScena(szerokoscWyswietlacza,wysokoscWyswietlacza);
        delete wyswietlaczPixele;
        wyswietlaczPixele = new WyswietlaczPixele(szerokoscWyswietlacza,wysokoscWyswietlacza);
        if(czyBylaWczytanaGrafika)
        {
            delete wyswietlaczGrafika;
            wyswietlaczGrafika = new WyswietlaczGrafika(grafikaImage->width(),grafikaImage->height());
            for(int i = 0; i < grafikaImage->width(); i++ )
            {
                for(int j = 0; j < grafikaImage->height(); j ++)
                {
                    wyswietlaczGrafika->uzupelnijKolor(i,j,grafikaImage->zwrocTabliceRGB(i,j));
                }
            }
            przyciskWyswietlObrazek(progskaliSzarosci,zachowacProporcje);
        }

        if(czyPolaczenieZModbusem)
        {
            zarzadzanieWysylaniemObrazuModbus->zmianaWielkosciWyswietlacza(szerokosc,wysokosc);
        }
    }
    emit zamknijOknoDialogowe();
}



void Logika::przyciskObrobkaObrazuOpcje()
{
    dialogOknoOtwarte = true;
    emit otworzOknoObrobkiObrazu(metodaSkalowania);

}

void Logika::zmianaAlgorytmuObrobkiObrazu(algorytmObrobkiObrazu metodaSkalowania)
{
    this->metodaSkalowania = metodaSkalowania;
    if(czyBylaWczytanaGrafika)
    {
        delete wyswietlaczGrafika;
        wyswietlaczGrafika = new WyswietlaczGrafika(grafikaImage->width(),grafikaImage->height());
        for(int i = 0; i < grafikaImage->width(); i++ )
        {
            for(int j = 0; j < grafikaImage->height(); j ++)
            {
                wyswietlaczGrafika->uzupelnijKolor(i,j,grafikaImage->zwrocTabliceRGB(i,j));
            }
        }
        przyciskWyswietlObrazek(progskaliSzarosci,zachowacProporcje);
    }
    emit zamknijOknoDialogowe();

}



void Logika::dialogOknoZamkniete()
{
    dialogOknoOtwarte = false;
}



void Logika::przyciskOtworzObrazek()
{
    dialogOknoOtwarte = true;
    emit otworzoknoDialogoweDoPobieraniaObrazu();
}

void Logika::przyciskWyswietlObrazek(int granicaSkaliSzarosci,bool zachowacProporcje)
{
    progskaliSzarosci = granicaSkaliSzarosci;
    this->zachowacProporcje = zachowacProporcje;
    if(czyBylaWczytanaGrafika)
    {
        delete wyswietlaczGrafika;
    }
    wyswietlaczGrafika = new WyswietlaczGrafika(grafikaImage->width(),grafikaImage->height());
    czyBylaWczytanaGrafika = true;
    for(int i = 0; i < grafikaImage->width(); i++ )
    {
        for(int j = 0; j < grafikaImage->height(); j ++)
        {
            wyswietlaczGrafika->uzupelnijKolor(i,j,grafikaImage->zwrocTabliceRGB(i,j));
        }
    }

    wyswietlaczGrafika->skalowanie(szerokoscWyswietlacza,wysokoscWyswietlacza,metodaSkalowania,granicaSkaliSzarosci, zachowacProporcje);

    for(int i = 0; i < szerokoscWyswietlacza; i++ )
    {
        for(int j = 0; j < wysokoscWyswietlacza; j ++)
        {

             wyswietlaczPixele->uzupelnijPixel(i,j,0);
             emit sygnalZaswiecLedScena(i,j,0);
             if(czyPolaczenieZModbusem)
             {
                 zarzadzanieWysylaniemObrazuModbus->uzupelnijPixel(i  ,j ,wyswietlaczPixele->getPixel(i,j));
             }
        }
    }
    int roznicaSzerokosci = szerokoscWyswietlacza - wyswietlaczGrafika->getSzerokosc();
    int roznicaWysokosci = wysokoscWyswietlacza - wyswietlaczGrafika->getWysokosc();
    for(int i = 0; i < wyswietlaczGrafika->getSzerokosc(); i++ )
    {
        for(int j = 0; j < wyswietlaczGrafika->getWysokosc(); j ++)
        {
             wyswietlaczPixele->uzupelnijPixel(i + floor(roznicaSzerokosci/2) ,j + floor(roznicaWysokosci/2),wyswietlaczGrafika->czyCzarnoBialy(i,j));
             emit sygnalZaswiecLedScena(i + floor(roznicaSzerokosci/2) ,j + floor(roznicaWysokosci/2),wyswietlaczGrafika->czyCzarnoBialy(i,j));
             if(czyPolaczenieZModbusem)
             {
                 zarzadzanieWysylaniemObrazuModbus->uzupelnijPixel(i + floor(roznicaSzerokosci/2) ,j + floor(roznicaWysokosci/2),wyswietlaczPixele->getPixel(i + floor(roznicaSzerokosci/2) ,j + floor(roznicaWysokosci/2)));
             }
        }
    }
}

void Logika::sliderGranicaSkaliSzarosciZmienilWartosc(int granicaSkaliSzarosci)
{
    this->progskaliSzarosci = granicaSkaliSzarosci;
    wyswietlaczGrafika->uzupelnijCzarnoBialy(granicaSkaliSzarosci);
    int roznicaSzerokosci = szerokoscWyswietlacza - wyswietlaczGrafika->getSzerokosc();
    int roznicaWysokosci = wysokoscWyswietlacza - wyswietlaczGrafika->getWysokosc();
    for(int i = 0; i < wyswietlaczGrafika->getSzerokosc(); i++ )
    {
        for(int j = 0; j < wyswietlaczGrafika->getWysokosc(); j ++)
        {
             wyswietlaczPixele->uzupelnijPixel(i + floor(roznicaSzerokosci/2) ,j + floor(roznicaWysokosci/2),wyswietlaczGrafika->czyCzarnoBialy(i,j));
             emit sygnalZaswiecLedScena(i + floor(roznicaSzerokosci/2) ,j + floor(roznicaWysokosci/2),wyswietlaczGrafika->czyCzarnoBialy(i,j));
             if(czyPolaczenieZModbusem)
             {
                 zarzadzanieWysylaniemObrazuModbus->uzupelnijPixel(i + floor(roznicaSzerokosci/2) ,j + floor(roznicaWysokosci/2),wyswietlaczPixele->getPixel(i + floor(roznicaSzerokosci/2) ,j + floor(roznicaWysokosci/2)));
             }
        }
    }
}

void Logika::przyciskAutomatyczneProgowanie()
{
    emit sygnalValueSliderGranicaSkaliSzarosci(wyswietlaczGrafika->uzupelnienieCzarnoBialyAutomatycznyProg());
    int roznicaSzerokosci = szerokoscWyswietlacza - wyswietlaczGrafika->getSzerokosc();
    int roznicaWysokosci = wysokoscWyswietlacza - wyswietlaczGrafika->getWysokosc();
    for(int i = 0; i < wyswietlaczGrafika->getSzerokosc(); i++ )
    {
        for(int j = 0; j < wyswietlaczGrafika->getWysokosc(); j ++)
        {
             wyswietlaczPixele->uzupelnijPixel(i + floor(roznicaSzerokosci/2) ,j + floor(roznicaWysokosci/2),wyswietlaczGrafika->czyCzarnoBialy(i,j));
             emit sygnalZaswiecLedScena(i + floor(roznicaSzerokosci/2) ,j + floor(roznicaWysokosci/2),wyswietlaczGrafika->czyCzarnoBialy(i,j));
             if(czyPolaczenieZModbusem)
             {
                 zarzadzanieWysylaniemObrazuModbus->uzupelnijPixel(i + floor(roznicaSzerokosci/2) ,j + floor(roznicaWysokosci/2),wyswietlaczPixele->getPixel(i + floor(roznicaSzerokosci/2) ,j + floor(roznicaWysokosci/2)));
             }
        }
    }
}

void Logika::przyciskWyslijObrazModbus(int adresUrzadzenia)
{
    if(adresUrzadzenia == 0 && iloscWlaczonychWatkow != 0)
    {
        emit sygnalczyWylaczycWszystkieTransmisjeTrybRozgloszeniowy(adresUrzadzenia,1);

    }
    else if(iloscWlaczonychWatkow >= maksymalnaIloscWlaczonychWatkow - 1)
    {
        emit sygnalzmienStanPrzyciskowModbus(tablicaUrzadzenPracujacych[adresUrzadzenia]);
        emit sygnalmaksymalnaIloscTransmisjiZostalaOsiagnieta();
    }
    else
    {
        wyslijObrazModbus(adresUrzadzenia);
    }

}

void Logika::przyciskZatrzymajWysylanieObrazModbus(int adresUrzadzenia)
{
    zarzadzanieWysylaniemObrazuModbus->setAdresUrzadzenia(adresUrzadzenia);
    zarzadzanieWysylaniemObrazuModbus->setPrzerwacWysylanieDlaAktualnegoUrzadzenia(true);
}

void Logika::przyciskWlaczTrybGraficznyModbus(bool wlacz, int adresUrzadzenia)
{

    if(adresUrzadzenia == 0 && iloscWlaczonychWatkow != 0)
    {
        if(wlacz)
        {
            emit sygnalczyWylaczycWszystkieTransmisjeTrybRozgloszeniowy(adresUrzadzenia,2);
        }
        else
        {
            emit sygnalczyWylaczycWszystkieTransmisjeTrybRozgloszeniowy(adresUrzadzenia,3);
        }

    }

    else if(iloscWlaczonychWatkow >= maksymalnaIloscWlaczonychWatkow - 1)
    {
        emit sygnalzmienStanPrzyciskowModbus(tablicaUrzadzenPracujacych[adresUrzadzenia]);
        emit sygnalmaksymalnaIloscTransmisjiZostalaOsiagnieta();
    }
    else
    {
        wlaczTrybGraficznyModbus(wlacz,adresUrzadzenia);
    }

}

void Logika::przyciskWlaczTrybAutomatycznyModbus(int adresUrzadzenia, bool wlacz)
{
    if(wlacz == false)
    {
        wlaczTrybAutomatycznyModbus(adresUrzadzenia,wlacz);
    }
    else if(adresUrzadzenia == 0 && iloscWlaczonychWatkow != 0)
    {
        if(wlacz)
        {
            emit sygnalczyWylaczycWszystkieTransmisjeTrybRozgloszeniowy(adresUrzadzenia,4);
        }
        else
        {
            emit sygnalczyWylaczycWszystkieTransmisjeTrybRozgloszeniowy(adresUrzadzenia,5);
        }

    }
    else if(iloscWlaczonychWatkow >= maksymalnaIloscWlaczonychWatkow - 1)
    {
        emit sygnalzmienStanPrzyciskowModbus(tablicaUrzadzenPracujacych[adresUrzadzenia]);
        emit sygnalmaksymalnaIloscTransmisjiZostalaOsiagnieta();
    }
    else
    {
        wlaczTrybAutomatycznyModbus(adresUrzadzenia,wlacz);
    }

}

void Logika::watekWysylaniaModbusZakonczony(int adresUrzadzenia)
{
    tablicaUrzadzenPracujacych[adresUrzadzenia] = nic;
    iloscWlaczonychWatkow--;
    emit sygnalzmienIloscTrwajacychTransmisji(iloscWlaczonychWatkow);
    if(dialogOknoOtwarte && iloscWlaczonychWatkow == 0)
    {
        emit sygnaldialogOknoTrwajaTransmisje(false);
    }
    qDebug() <<"Ilosc wlaczonych watkow" <<iloscWlaczonychWatkow;
    emit sygnalzakonczenieWysylania(adresUrzadzenia);

}

void Logika::brakOdpowiedziSlave(int adresUrzadzenia)
{
    emit sygnalBrakOdpowiedziSlave(adresUrzadzenia);
}

void Logika::pokazOdpowiedniePrzyciskiModbus(int adresUrzadzenia)
{

    if(adresUrzadzenia == 0)
    {
        emit sygnalzmienStanPrzyciskowModbus(tablicaUrzadzenPracujacych[adresUrzadzenia]);
    }
    else
    {
        if(tablicaUrzadzenPracujacych[0] == nic)
        {
            emit sygnalzmienStanPrzyciskowModbus(tablicaUrzadzenPracujacych[adresUrzadzenia]);
        }
        else
        {
            emit sygnalzmienStanPrzyciskowModbus(wlaczonyTrybRozgloszeniowy);
        }
    }

}

void Logika::brakPolaczeniaZPortemCom()
{
    if(czyPolaczenieZModbusem)
    {
        przyciskRozlaczModbus();
        emit sygnalbrakPolaczeniaCom();
    }
}

void Logika::przyciskZakonczWszystkieTransmisjeModbusModbus()
{
    zarzadzanieWysylaniemObrazuModbus->setPrzerwacWysylanie(true);
    QThreadPool::globalInstance()->waitForDone(-1);
}



void Logika::zakonczWyslijDoUrzadzenia(int adresUrzadzenia)
{
    zarzadzanieWysylaniemObrazuModbus->setPrzerwacWysylanie(true);
    QThreadPool::globalInstance()->waitForDone(-1);
    wyslijObrazModbus(adresUrzadzenia);
}

void Logika::zakonczWlaczTrybGraficzny(int adresUrzadzenia, bool czyWlaczyc)
{
    zarzadzanieWysylaniemObrazuModbus->setPrzerwacWysylanie(true);
    QThreadPool::globalInstance()->waitForDone(-1);
    wlaczTrybGraficznyModbus(czyWlaczyc,adresUrzadzenia);
}

void Logika::zakonczAutomatyczneWysylanie(int adresUrzadzenia, bool wlacz)
{
    zarzadzanieWysylaniemObrazuModbus->setPrzerwacWysylanie(true);
    QThreadPool::globalInstance()->waitForDone(-1);
    wlaczTrybAutomatycznyModbus(adresUrzadzenia,wlacz);
}

coRobiUrzadzenie Logika::gettablicaUrzadzenPracujacych(int adresUrzadzenia)
{
    return tablicaUrzadzenPracujacych[adresUrzadzenia];
}

void Logika::wyslijObrazModbus(int adresUrzadzenia)
{
    zarzadzanieWysylaniemObrazuModbus->setAdresUrzadzenia(adresUrzadzenia);
    zarzadzanieWysylaniemObrazuModbus->setCoRobicAktualnie(wysylacObraz);
    zarzadzanieWysylaniemObrazuModbus->setAutoDelete(false);
    for(int i = 0; i < szerokoscWyswietlacza; i++ )
    {
        for(int j = 0; j < wysokoscWyswietlacza; j ++)
        {
            zarzadzanieWysylaniemObrazuModbus->uzupelnijPixel(i,j,wyswietlaczPixele->getPixel(i,j));
        }
    }
    tablicaUrzadzenPracujacych[adresUrzadzenia] = trybManualny;
    iloscWlaczonychWatkow++;
    emit sygnalzmienIloscTrwajacychTransmisji(iloscWlaczonychWatkow);

    if(dialogOknoOtwarte)
    {
        emit sygnaldialogOknoTrwajaTransmisje(true);
    }
    QThreadPool::globalInstance()->start(zarzadzanieWysylaniemObrazuModbus);
}

void Logika::wlaczTrybGraficznyModbus(bool wlacz, int adresUrzadzenia)
{
    zarzadzanieWysylaniemObrazuModbus->setAdresUrzadzenia(adresUrzadzenia);
    if(wlacz)
    {
        zarzadzanieWysylaniemObrazuModbus->setCoRobicAktualnie(wlaczycTrybGraficzny);
    }
    else
    {
        zarzadzanieWysylaniemObrazuModbus->setCoRobicAktualnie(wylaczycTrybGraficzny);
    }
    tablicaUrzadzenPracujacych[adresUrzadzenia] = trybManualny;
    zarzadzanieWysylaniemObrazuModbus->setAutoDelete(false);
    iloscWlaczonychWatkow++;
    emit sygnalzmienIloscTrwajacychTransmisji(iloscWlaczonychWatkow);
    if(dialogOknoOtwarte)
    {
        emit sygnaldialogOknoTrwajaTransmisje(true);
    }
    QThreadPool::globalInstance()->start(zarzadzanieWysylaniemObrazuModbus);
}

void Logika::wlaczTrybAutomatycznyModbus(int adresUrzadzenia, bool wlacz)
{
    if(wlacz)
    {
        zarzadzanieWysylaniemObrazuModbus->setAdresUrzadzenia(adresUrzadzenia);
        zarzadzanieWysylaniemObrazuModbus->setCoRobicAktualnie(trybAutomatycznyWysylania);
        for(int i = 0; i < szerokoscWyswietlacza; i++ )
        {
            for(int j = 0; j < wysokoscWyswietlacza; j ++)
            {
                zarzadzanieWysylaniemObrazuModbus->uzupelnijPixel(i,j,wyswietlaczPixele->getPixel(i,j));
            }
        }
        tablicaUrzadzenPracujacych[adresUrzadzenia] = trybAuto;
        iloscWlaczonychWatkow++;
        emit sygnalzmienIloscTrwajacychTransmisji(iloscWlaczonychWatkow);
        if(dialogOknoOtwarte)
        {
            emit sygnaldialogOknoTrwajaTransmisje(true);
        }
        QThreadPool::globalInstance()->start(zarzadzanieWysylaniemObrazuModbus);
    }
    else
    {
        zarzadzanieWysylaniemObrazuModbus->setAdresUrzadzenia(adresUrzadzenia);
        zarzadzanieWysylaniemObrazuModbus->setPrzerwacWysylanieDlaAktualnegoUrzadzenia(true);
    }
}

