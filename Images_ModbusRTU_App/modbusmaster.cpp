#include "modbusmaster.h"

ModbusMaster::ModbusMaster()
{
    dlugoscOstatniejOdebranejWiadomosci = 0;

}

ModbusMaster::~ModbusMaster()
{
    if(dlugoscOstatniejOdebranejWiadomosci != 0)
    {
        delete ostatniaOdebranaWiadomosc;
    }
}

bool ModbusMaster::ustawieniaPort(std::string port)
{
    this->port = port;
    hComm = CreateFileA(port.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0);

    if (hComm == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    else
    {
        return true;
    }

}

bool ModbusMaster::ustawieniaPrzesylu(std::string predkoscTransmisji, std::string liczbaBitow, std::string parity)
{

    QString::fromStdString(predkoscTransmisji);
    GetCommState(hComm, &dcb);

    int predkoscTransmisjiInt = stoi(predkoscTransmisji);
    if(predkoscTransmisjiInt == 19200) dcb.BaudRate = CBR_19200;
    else if(predkoscTransmisjiInt == 110) dcb.BaudRate = CBR_110;
    else if(predkoscTransmisjiInt == 300) dcb.BaudRate = CBR_300;
    else if(predkoscTransmisjiInt == 600) dcb.BaudRate = CBR_600;
    else if(predkoscTransmisjiInt == 1200) dcb.BaudRate = CBR_1200;
    else if(predkoscTransmisjiInt == 2400) dcb.BaudRate = CBR_2400;
    else if(predkoscTransmisjiInt == 4800) dcb.BaudRate = CBR_4800;
    else if(predkoscTransmisjiInt == 9600) dcb.BaudRate = CBR_9600;
    else if(predkoscTransmisjiInt == 1440) dcb.BaudRate = CBR_14400;
    else if(predkoscTransmisjiInt == 38400) dcb.BaudRate = CBR_38400;
    else if(predkoscTransmisjiInt == 57600) dcb.BaudRate = CBR_57600;
    else dcb.BaudRate = CBR_115200;



    if(liczbaBitow == "8")
    {
        dcb.ByteSize = 8;
    }
    else
    {
        dcb.ByteSize = 7;
    }


    if(parity == "Żaden")
    {
        dcb.Parity = NOPARITY;
        dcb.StopBits = TWOSTOPBITS;
    }
    else if(parity == "Nieparzysty")
    {
        dcb.Parity = ODDPARITY;
        dcb.StopBits = ONESTOPBIT;
    }
    else
    {
        dcb.Parity = EVENPARITY;
        dcb.StopBits = ONESTOPBIT;
    }

    if(!SetCommState(hComm, &dcb))
    {
        return false;
    }

    timeouts = { 1,
                 0,
                 25,
                 0,
                 0
    };

    if(!SetCommTimeouts(hComm, &timeouts))
    {
        return false;
    }
    COMSTAT ComStat;
    ClearCommError( hComm, 0, &ComStat );

    return true;
}

void ModbusMaster::rozlacz()
{
    CloseHandle(hComm);
}


zwrotWyslaniaRamki ModbusMaster::wlaczTrybGraficzny(int adresUrzadzenia, bool wlacz)
{
    unsigned char *ramkaRTU = new unsigned char[11];
    ramkaRTU[0] = (unsigned char) adresUrzadzenia;                //adres urdzadzenia
    ramkaRTU[1] = (unsigned char) 16;                         //kod funkcji
    ramkaRTU[2] = (unsigned char) 00;
    ramkaRTU[3] = (unsigned char) 73;
    ramkaRTU[4] = (unsigned char) 00;
    ramkaRTU[5] = (unsigned char) 1;
    ramkaRTU[6] = (unsigned char) 2;
    ramkaRTU[7] = (unsigned char) 00;

    if(wlacz)
    {
        ramkaRTU[8] = (unsigned char) 01;
    }
    else
    {
        ramkaRTU[8] = (unsigned char) 00;
    }

    unsigned char *CRC = obliczCRC(ramkaRTU,9);

    ramkaRTU[9]= CRC[0];                              //CRC Hi
    ramkaRTU[10]= CRC[1];                               //CRC Lo
    delete[] CRC;
    unsigned char *odpowiedz = new unsigned char[6];
    for(int i = 0; i < 6; i++)
    {
        odpowiedz[i] = ramkaRTU[i];
    }

    zwrotWyslaniaRamki zwrotWyslaniaRamki = wyslijRamke(adresUrzadzenia,ramkaRTU,11,odpowiedz,6);
    delete []ramkaRTU;
    delete []odpowiedz;
    return zwrotWyslaniaRamki;
}

zwrotWyslaniaRamki ModbusMaster::zaswiec(int adresUrzadzenia, int numerWiersza, unsigned char *wiadomosc, int dlugoscWiadomosci)
{

    unsigned char *ramkaRTU = new unsigned char[27];
    ramkaRTU[0] = (unsigned char) adresUrzadzenia;                //adres urdzadzenia
    ramkaRTU[1] = (unsigned char) 16;                         //kod funkcji
    ramkaRTU[2] = (unsigned char) 0;
    ramkaRTU[3] = (unsigned char) 74;
    ramkaRTU[4] = (unsigned char) 00;
    ramkaRTU[5] = (unsigned char) (dlugoscWiadomosci + 2)/2;
    ramkaRTU[6] = (unsigned char) dlugoscWiadomosci + 2;
    ramkaRTU[7]=(unsigned char) ((numerWiersza >> 8) & 0xFF);
    ramkaRTU[8]=(unsigned char) (numerWiersza & 0xFF);

    for (int i = 0; i < dlugoscWiadomosci; i ++)
    {
        ramkaRTU[9 + i]= wiadomosc[i];
    }

    unsigned char *CRC = obliczCRC(ramkaRTU, 9 + dlugoscWiadomosci);

    ramkaRTU[9 + dlugoscWiadomosci]= CRC[0];                              //CRC Hi
    ramkaRTU[9 + dlugoscWiadomosci + 1]= CRC[1];                               //CRC Lo
    delete[] CRC;
    unsigned char *odpowiedz = new unsigned char[6];
    for(int i = 0; i < 6; i++)
    {
        odpowiedz[i] = ramkaRTU[i];
    }
    zwrotWyslaniaRamki zwrotWyslaniaRamki = wyslijRamke(adresUrzadzenia,ramkaRTU,9 + dlugoscWiadomosci + 2,odpowiedz,6);

    delete [] ramkaRTU;
    delete [] odpowiedz;

    return zwrotWyslaniaRamki;
}

unsigned char *ModbusMaster::obliczCRC(unsigned char *pMessage, unsigned int NumberOfBytes)
{
    static unsigned char aCRCHi[] =
    {
    0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81,
    0x40, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0,
    0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01,
    0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41,
    0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81,
    0x40, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0,
    0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01,
    0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40,
    0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81,
    0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0,
    0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01,
    0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41,
    0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81,
    0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0,
    0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01,
    0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41,
    0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81,
    0x40
    };
    static unsigned char aCRCLo[] =
    {
    0x00, 0xc0, 0xc1, 0x01, 0xc3, 0x03, 0x02, 0xc2, 0xc6, 0x06, 0x07, 0xc7, 0x05, 0xc5, 0xc4,
    0x04, 0xcc, 0x0c, 0x0d, 0xcd, 0x0f, 0xcf, 0xce, 0x0e, 0x0a, 0xca, 0xcb, 0x0b, 0xc9, 0x09,
    0x08, 0xc8, 0xd8, 0x18, 0x19, 0xd9, 0x1b, 0xdb, 0xda, 0x1a, 0x1e, 0xde, 0xdf, 0x1f, 0xdd,
    0x1d, 0x1c, 0xdc, 0x14, 0xd4, 0xd5, 0x15, 0xd7, 0x17, 0x16, 0xd6, 0xd2, 0x12, 0x13, 0xd3,
    0x11, 0xd1, 0xd0, 0x10, 0xf0, 0x30, 0x31, 0xf1, 0x33, 0xf3, 0xf2, 0x32, 0x36, 0xf6, 0xf7,
    0x37, 0xf5, 0x35, 0x34, 0xf4, 0x3c, 0xfc, 0xfd, 0x3d, 0xff, 0x3f, 0x3e, 0xfe, 0xfa, 0x3a,
    0x3b, 0xfb, 0x39, 0xf9, 0xf8, 0x38, 0x28, 0xe8, 0xe9, 0x29, 0xeb, 0x2b, 0x2a, 0xea, 0xee,
    0x2e, 0x2f, 0xef, 0x2d, 0xed, 0xec, 0x2c, 0xe4, 0x24, 0x25, 0xe5, 0x27, 0xe7, 0xe6, 0x26,
    0x22, 0xe2, 0xe3, 0x23, 0xe1, 0x21, 0x20, 0xe0, 0xa0, 0x60, 0x61, 0xa1, 0x63, 0xa3, 0xa2,
    0x62, 0x66, 0xa6, 0xa7, 0x67, 0xa5, 0x65, 0x64, 0xa4, 0x6c, 0xac, 0xad, 0x6d, 0xaf, 0x6f,
    0x6e, 0xae, 0xaa, 0x6a, 0x6b, 0xab, 0x69, 0xa9, 0xa8, 0x68, 0x78, 0xb8, 0xb9, 0x79, 0xbb,
    0x7b, 0x7a, 0xba, 0xbe, 0x7e, 0x7f, 0xbf, 0x7d, 0xbd, 0xbc, 0x7c, 0xb4, 0x74, 0x75, 0xb5,
    0x77, 0xb7, 0xb6, 0x76, 0x72, 0xb2, 0xb3, 0x73, 0xb1, 0x71, 0x70, 0xb0, 0x50, 0x90, 0x91,
    0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9c, 0x5c,
    0x5d, 0x9d, 0x5f, 0x9f, 0x9e, 0x5e, 0x5a, 0x9a, 0x9b, 0x5b, 0x99, 0x59, 0x58, 0x98, 0x88,
    0x48, 0x49, 0x89, 0x4b, 0x8b, 0x8a, 0x4a, 0x4e, 0x8e, 0x8f, 0x4f, 0x8d, 0x4d, 0x4c, 0x8c,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
    0x40
    };
    unsigned char HiByte = 0xFF;
    unsigned char LoByte = 0xFF;
    unsigned char Index;
    while (NumberOfBytes--)
    {
    Index = HiByte ^ *pMessage++;
    HiByte = LoByte ^ aCRCHi[Index];
    LoByte = aCRCLo[Index];
    };

    unsigned char *tablica = new unsigned char[2];
    tablica[0] = HiByte;
    tablica[1] = LoByte;
    return tablica;
}

bool ModbusMaster::sprawdzCRC(unsigned char *wiadomosc, unsigned int dlugoscWiadomosci)
{
    unsigned char *poprawaneCRC = obliczCRC(wiadomosc,dlugoscWiadomosci -2);

    if(poprawaneCRC[0] == wiadomosc[dlugoscWiadomosci -2] && poprawaneCRC[1] == wiadomosc[dlugoscWiadomosci -1])
    {
        delete[] poprawaneCRC;
        return true;
    }
    else
    {
        delete[] poprawaneCRC;
        return false;
    }
}

zwrotWyslaniaRamki ModbusMaster::wyslijRamke(int adresUrzadzenia, unsigned char *ramkaRTU, unsigned int dlugoscRamki, unsigned char *odpowiedz,int dlugoscOdpowiedzi)
{
    m.lock();
    COMSTAT ComStat;
    if(!ClearCommError( hComm, 0, &ComStat ))
    {
        if(!sprobujPolaczycPonownieZPortem())
        {
            m.unlock();
            return bladPolaczeniaCom;
        }
    }
    int i = 0;
    while((int) ComStat.cbInQue != 0 && i < 20)
    {
        unsigned char tmp[1000];
        ReadFile(hComm, tmp, 1000, 0, 0);
        if(!ClearCommError( hComm, 0, &ComStat ))
        {
            if(!sprobujPolaczycPonownieZPortem())
            {
                m.unlock();
                return bladPolaczeniaCom;
            }
        }
        i++;
    }
    if(i >= 20)
    {
        m.unlock();
        return bladPolaczeniaCom;
    }

    if(!WriteFile(hComm, ramkaRTU, dlugoscRamki , 0, 0))
    {
        if(!sprobujPolaczycPonownieZPortem())
        {
            m.unlock();
            return bladPolaczeniaCom;
        }
    }
    else
    {
        if(adresUrzadzenia!= 0)
        {

            zwrot zwrot = czekanieNaOdpowiedz(odpowiedz,dlugoscOdpowiedzi);
            if(zwrot != poprawna)
            {
                if(zwrot == bladPolaczeniaComZwrot)
                {
                    m.unlock();
                    return bladPolaczeniaCom;
                }
                        else
                        {
                            m.unlock();
                            return bladPolaczeniaZWyswietlaczem;
                        }
            }
        }
    }
    m.unlock();
    return wykonano;

}

zwrot ModbusMaster::czekanieNaOdpowiedz (unsigned char *odpowiedz,int dlugoscOdpowiedzi)
{
    unsigned char read_buffor[256];
    DWORD ileBajtow = 0;

    if(!ReadFile(hComm, &read_buffor, 256, &ileBajtow, 0))
    {
        if(!sprobujPolaczycPonownieZPortem())
        {
            return bladPolaczeniaComZwrot;
        }
    }

    if (ileBajtow < 3 )
    {
        return brakOdpowiedzilubZla;
    }
    else if(sprawdzCRC(read_buffor,ileBajtow))
    {
        if(read_buffor[0] == odpowiedz[0])
        {
            bool poprawnaOdpowiedz = true;
            for(int i = 1; i < dlugoscOdpowiedzi; i ++)
            {
                if(read_buffor[i] != odpowiedz[i])
                {
                    poprawnaOdpowiedz = false;
                }
            }
            if(poprawnaOdpowiedz)
            {
                if(dlugoscOstatniejOdebranejWiadomosci != 0)
                {
                    delete [] ostatniaOdebranaWiadomosc;
                }
                dlugoscOstatniejOdebranejWiadomosci = ileBajtow;
                ostatniaOdebranaWiadomosc = new unsigned char[dlugoscOstatniejOdebranejWiadomosci];
                return poprawna;
            }
            else if(read_buffor[1] == (unsigned char) ((int) odpowiedz[1] + 128))
            {
                if(read_buffor[2] == (unsigned char) 01)
                {
                    return nFunkcja;
                }
                else if(read_buffor[2] == (unsigned char) 02)
                {
                    return nAdresDanych;
                }
                else if(read_buffor[2] == (unsigned char) 03)
                {
                    return nWartoscDanych;
                }
                else if(read_buffor[2] == (unsigned char) 04)
                {
                    return niepowodzenie;
                }
                else if(read_buffor[2] == (unsigned char) 05)
                {
                    return potwierdzenie;
                }
                else if(read_buffor[2] == (unsigned char) 06)
                {
                    return jednostkaPodrzednaZajeta;
                }
                else if(read_buffor[2] == (unsigned char) 07)
                {
                    return bladParzystosciPamieci;
                }
                else
                {
                    return brakOdpowiedzilubZla;
                }
            }
            else
            {
                return brakOdpowiedzilubZla;
            }
        }
        else
        {
            return brakOdpowiedzilubZla;
        }
    }
    else
    {
        return brakOdpowiedzilubZla;
    }

}

bool ModbusMaster::sprobujPolaczycPonownieZPortem()
{
    this->rozlacz();
    if(!this->ustawieniaPort(port))
    {
        return false;
    }
    if(!SetCommState(hComm, &dcb))
    {
        return false;
    }
    return SetCommTimeouts(hComm, &timeouts);
}





