#include "dialogopcjewyswietlacz.h"
#include "ui_dialogopcjewyswietlacz.h"

DialogOpcjeWyswietlacz::DialogOpcjeWyswietlacz(int szerokosc, int wysokosc, bool czyTrwaTransmisja):
    ui(new Ui::DialogOpcjeWyswietlacz)
{
    ui->setupUi(this);
    this->setWindowTitle("Opcje wyświetlacza");
    this->setFixedSize(300,150);
    this->setCzyTrwaTransmisja(czyTrwaTransmisja);

    napisSzerokosc= new QLabel("Szerokość wyświetlacza w pikselach");
    spinBoxSzerokosc = new QSpinBox();
    spinBoxSzerokosc->setMinimum(1);
    spinBoxSzerokosc->setMaximum(2500);
    spinBoxSzerokosc->setValue(szerokosc);



    boxLayoutPierwszy = new QHBoxLayout();
    boxLayoutPierwszy->addWidget(napisSzerokosc);
    boxLayoutPierwszy->addWidget(spinBoxSzerokosc);
    ui->verticalLayout->addLayout(boxLayoutPierwszy);

    napisWysokosc= new QLabel("Wysokość wyświetlacza w pikselach");
    spinBoxWysokosc = new QSpinBox();
    spinBoxWysokosc->setMinimum(1);
    spinBoxWysokosc->setMaximum(2500);
    spinBoxWysokosc->setValue(wysokosc);



    boxLayoutDrugi = new QHBoxLayout();
    boxLayoutDrugi->addWidget(napisWysokosc);
    boxLayoutDrugi->addWidget(spinBoxWysokosc);
    ui->verticalLayout->addLayout(boxLayoutDrugi);

    ui->verticalLayout->addSpacing(30);

    pushButtonOK = new QPushButton("OK");
    pushButtonAnuluj = new QPushButton("Anuluj");

    boxLayoutSzosty = new QHBoxLayout();
    boxLayoutSzosty->addWidget(pushButtonOK);
    boxLayoutSzosty->addWidget(pushButtonAnuluj);
    ui->verticalLayout->addLayout(boxLayoutSzosty);

    QObject::connect(pushButtonOK, SIGNAL(clicked()),this, SLOT(przyciskOKKlik()));
    QObject::connect(pushButtonAnuluj, SIGNAL(clicked()),this, SLOT(przyciskAnulujKlik()));

}

DialogOpcjeWyswietlacz::~DialogOpcjeWyswietlacz()
{
    delete spinBoxSzerokosc;
    delete spinBoxWysokosc;
    delete pushButtonOK;
    delete pushButtonAnuluj;
    delete boxLayoutPierwszy;
    delete boxLayoutDrugi;
    delete napisSzerokosc;
    delete napisWysokosc;
    delete ui;
}

void DialogOpcjeWyswietlacz::przyciskOKKlik()
{
    if(spinBoxSzerokosc->value() > 256 || spinBoxWysokosc->value() > 128)
    {
        QMessageBox::information(this,"Informacja"," Maksymalny rozmiar wyświetlacza to 256x128." );

    }
    else if(spinBoxSzerokosc->value()%16 != 0 || spinBoxWysokosc->value()%8 != 0)
    {
        QMessageBox::information(this,"Informacja"," Szerokość wyświetlacza musi być wielokrotnością 16, a wysokość 8." );

    }
    else
    {
        if(this->czyTrwaTransmisja)
        {
            QMessageBox msgBox;
            msgBox.setText("Trwa transmisja do wyświetlaczy. Zmiana wymiarów wyswietlacza automatycznie zakończy odbywające się transmisję.");
            msgBox.setInformativeText("Czy jesteś pewien?");
            msgBox.setWindowFlags(Qt::WindowType::WindowStaysOnTopHint);
            msgBox.setIcon(QMessageBox::Information);
            QPushButton *pushButtonTak = msgBox.addButton(tr("Tak"), QMessageBox::YesRole);
            QPushButton *pushButtonNie = msgBox.addButton(tr("Anuluj"), QMessageBox::NoRole);


            msgBox.exec();

            if (msgBox.clickedButton() == pushButtonTak) {
                delete pushButtonTak;
                delete pushButtonNie;
                close();
                emit przyciskOkKlikniety(spinBoxSzerokosc->value(),spinBoxWysokosc->value());
            } else if (msgBox.clickedButton() == pushButtonNie) {
                delete pushButtonTak;
                delete pushButtonNie;
                przyciskAnulujKlik();
            }
        }
        else
        {
            close();
            emit przyciskOkKlikniety(spinBoxSzerokosc->value(),spinBoxWysokosc->value());
        }



    }

}

void DialogOpcjeWyswietlacz::przyciskAnulujKlik()
{
    close();
}

void DialogOpcjeWyswietlacz::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit oknoZamkniete();
}

void DialogOpcjeWyswietlacz::reject()
{
    close();
}
