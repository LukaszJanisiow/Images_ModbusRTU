#include "mainwindow.h"
#include "ui_mainwindow.h"





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    logika = new Logika();
    this->setVisible(true);
    setWindowTitle("Konwerter obrazów Modbus");
    setWindowIcon(QIcon(":/new/prefix1/obrazy/images.png"));

    this->tworzenieAction();
    this->tworzenieMenuGornego();

    wyswietlaczScene = new WyswietlaczScene(this,128,64);

    wyswietlaczView = new WyswietlaczView(this);

    wyswietlaczView->setScene(wyswietlaczScene);


    ui->gridLayout->addWidget(wyswietlaczView,0,0);

    vboxPasekBoczny = new QVBoxLayout();

    this->dodawanieElemetowWczytywaniaGrafiki();
    this->dodawanieElementowObslugiModbus();
    ui->gridLayout->addLayout(vboxPasekBoczny,0,1,Qt::AlignTop);

    connect(logika,SIGNAL(sygnalBrakOdpowiedziSlave(int)),this,SLOT(brakOdpowiedziSlave(int)));
    connect(logika,SIGNAL(sygnalpolaczonolubRozlaczonoZModbusem(bool)),this,SLOT(polaczonolubRozlaczonoZModbusem(bool)));
    connect(logika,SIGNAL(sygnalOtworzenieOknaModbusPortProblem(std::string)),this,SLOT(otworzenieOknaModbusPortProblem(std::string)));
    connect(logika,SIGNAL(otworzOknoPolaczenieModbus()),this,SLOT(otwarcieOknaPolaczeniaModbus()));
    connect(logika,SIGNAL(otworzOknoEdycjiModbus(std::string,std::string, std::string, bool)),this,SLOT(otwarcieOknaPolaczeniaModbus(std::string,std::string, std::string, bool)));
    connect(logika,SIGNAL(otworzOknoOpcjiWyswietlacza(int, int, bool)),this,SLOT(otwarcieOknaOpcjiwyswietlacza(int, int, bool)));
    connect(logika,SIGNAL(sygnalzmienWymiaryScena(int,int)),this,SLOT(zmienWymiaryScena(int,int)));
    connect(logika,SIGNAL(otworzOknoObrobkiObrazu(algorytmObrobkiObrazu)),this,SLOT(otwarcieOknoObrobkiObrazu(algorytmObrobkiObrazu)));
    connect(logika,SIGNAL(otworzoknoDialogoweDoPobieraniaObrazu()),this,SLOT(otwarcieoknaDialogoweDoPobieraniaObrazu()));
    connect(logika,SIGNAL(sygnalZaswiecLedScena(int,int, bool)),this,SLOT(zaswiecLedScena(int, int, bool)));
    connect(logika,SIGNAL(zamknijOknoDialogowe()),this,SLOT(zamknijOkno()));
    connect(logika,SIGNAL(sygnalValueSliderGranicaSkaliSzarosci(int)),this,SLOT(setValueSliderGranicaSkaliSzarosci(int)));
    connect(logika,SIGNAL(sygnalczyWylaczycWszystkieTransmisjeTrybRozgloszeniowy(int,int)),this,SLOT(czyWylaczycWszystkieTransmisjeTrybRozgloszeniowy(int,int)));
    connect(logika,SIGNAL(sygnalzmienStanPrzyciskowModbus(coRobiUrzadzenie)),this,SLOT(zmienStanPrzyciskowModbus(coRobiUrzadzenie)));
    connect(logika,SIGNAL(sygnalmaksymalnaIloscTransmisjiZostalaOsiagnieta()),this,SLOT(maksymalnaIloscTransmisjiZostalaOsiagnieta()));
    connect(logika,SIGNAL(sygnalzmienIloscTrwajacychTransmisji(int)),this,SLOT(zmienIloscTrwajacychTransmisji(int)));
    connect(logika,SIGNAL(sygnaldialogOknoTrwajaTransmisje(bool)),this,SLOT(dialogOknoTrwajaTransmisje(bool)));
    connect(logika,SIGNAL(sygnalzakonczenieWysylania(int)),this,SLOT(zakonczenieWysylania(int)));
    connect(logika,SIGNAL(sygnalbrakPolaczeniaCom()),this,SLOT(brakPolaczeniaCom()));

}

void MainWindow::otworzenieOknaModbusPortProblem(std::string port)
{
    QMessageBox::warning(this,"Ostrzeżenie","Nie udało połączyć się z portem " + QString::fromStdString(port) + ".\n" + "Port z którym chcesz się połączyć jest zajęty lub urządzenie nie jest podłączone." );
}

void MainWindow::otwarcueOknoEdycjiModbus(std::string predkoscTransmisji, std::string liczbaBitow, std::string parity, bool czyTrwaTransmisja)
{
    dialogOkno = new DialogPolaczModbus(predkoscTransmisji,liczbaBitow,parity);
    dialogOkno->setCzyTrwaTransmisja(czyTrwaTransmisja);
    dialogOkno->setParent(this);
    dialogOkno->setWindowFlag(Qt::Dialog);
    dialogOkno->setModal(Qt::ApplicationModal);
    dialogOkno->show();
    connect(dialogOkno,SIGNAL(przyciskOkKlikniety(ParametryModbus)),logika,SLOT(edytujModbus(ParametryModbus)));
    connect(dialogOkno,SIGNAL(oknoZamkniete()),logika,SLOT(dialogOknoZamkniete()));
}

void MainWindow::otwarcieOknaOpcjiwyswietlacza(int szerokosc, int wysokosc, bool czyTrwaTransmisji)
{

    dialogOkno = new DialogOpcjeWyswietlacz(szerokosc,wysokosc,czyTrwaTransmisji);
    dialogOkno->setParent(this);
    dialogOkno->setWindowFlag(Qt::Dialog);
    dialogOkno->setModal(Qt::ApplicationModal);
    dialogOkno->show();
    connect(dialogOkno,SIGNAL(przyciskOkKlikniety(int,int)),logika,SLOT(zmianaWymiarowWyswietlacz(int,int)));
    connect(dialogOkno,SIGNAL(oknoZamkniete()),logika,SLOT(dialogOknoZamkniete()));
}

void MainWindow::polaczonolubRozlaczonoZModbusem(bool polaczono)
{
    actionModbusPolacz->setEnabled(!polaczono);
    actionModbusEdytuj->setEnabled(polaczono);
    actionModbusRozlacz->setEnabled(polaczono);
    groupBoxModbus->setEnabled(polaczono);
    if(polaczono)
    {
        groupBoxModbus->setToolTip("");
        statusBar()->showMessage(tr("Połączono z Modbusem"));
    }
    else
    {
        this->setEnabled(true);
        statusBar()->showMessage(tr("Rozłączono z Modbusem"));
        groupBoxModbus->setToolTip("Połącz się z Modbusem");
    }
}

void MainWindow::ustawGrafike(QImage image)
{
    labelGrafika->setPixmap(QPixmap::fromImage(image).scaled(256,128,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    pushButtonWyswietlObrazek->setEnabled(true);
    checkboxZachowacProporcje->setEnabled(true);
    pushButtonWyswietlObrazek->setToolTip("Grafika zostanie wyświetlona na podglądzie wyświetlacza");
}

void MainWindow::otworzenieOknaGrafikaWczytanieProblem()
{
    QMessageBox::warning(this,"Ostrzeżenie","Nie udało wczytać się grafiki.");
}

void MainWindow::zmienWymiaryScena(int szerokosc, int wysokosc)
{
    delete wyswietlaczScene;
    wyswietlaczScene = new WyswietlaczScene(this,szerokosc,wysokosc);
    wyswietlaczView->setScene(wyswietlaczScene);
}

void MainWindow::otwarcieOknoObrobkiObrazu(algorytmObrobkiObrazu metodaSkalowania)
{
    dialogOkno = new DialogOpcjeObrobkaObrazu(metodaSkalowania);
    dialogOkno->setParent(this);
    dialogOkno->setWindowFlag(Qt::Dialog);
    dialogOkno->setModal(Qt::ApplicationModal);
    dialogOkno->show();
    connect(dialogOkno,SIGNAL(przyciskOkKlikniety(algorytmObrobkiObrazu)),logika,SLOT(zmianaAlgorytmuObrobkiObrazu(algorytmObrobkiObrazu)));
    connect(dialogOkno,SIGNAL(oknoZamkniete()),logika,SLOT(dialogOknoZamkniete()));
}

void MainWindow::otwarcieoknaDialogoweDoPobieraniaObrazu()
{
    QString nazwaPliku = QFileDialog::getOpenFileName(this, tr("Wybierz grafikę"),"",tr("Images (*.png *.jpeg *.jpg *.tif )"));
    if(!nazwaPliku.isNull())
    {
        if(logika->wczytajGrafike(nazwaPliku))
        {
            this->ustawGrafike(QImage(nazwaPliku));
        }
        else
        {
            this->otworzenieOknaGrafikaWczytanieProblem();
        }
    }
}

void MainWindow::zaswiecLedScena(int x, int y, bool czyZaswiecic)
{
    wyswietlaczScene->zaswiecLed(x,y,czyZaswiecic);
}



MainWindow::~MainWindow()
{
    delete logika;
    delete menuOpcje;
    delete menuModbus;

    delete actionModbusPolacz;
    delete actionModbusEdytuj;
    delete actionModbusRozlacz;
    delete actionWyswietlaczOpcje;
    delete actionObrobkaObrazuOpcje;

    delete wyswietlaczScene;
    delete wyswietlaczView;

    delete vboxPasekBoczny;

    delete labelGrafika;
    delete pushButtonOtworzObrazek;
    delete checkboxZachowacProporcje;
    delete pushButtonWyswietlObrazek;
    delete sliderGranicaSkaliSzarosci;
    delete pushButtonAutomatyczneProgowanie;
    delete vboxGrafika;
    delete groupBoxGrafika;
    delete vboxProgSkaliSzarosci;
    delete groupBoxProgSkaliSzarosci;


    delete labelAdresUrzadzenia;
    delete spinBoxAdresUrzadzenia;
    delete adresUrzadzeniaLayout;
    delete pushButtonWlaczTrybGraficznyModbus;
    delete pushButtonWylaczTrybGraficznyModbus;
    delete trybGraficznyLayout;
    delete pushButtonWyslijObrazModbus;
    delete pushButtonZatrzymajWysylanieObrazuModbus;
    delete wyslijZatrzymajLayout;
    delete checkboxAutomatyczneWysylanieModbus;
    delete labelIloscTrwajacychTransmisji;
    delete labelLiczbaTrwajacychTransmisji;
    delete trwajaceTransmisjeLayout;
    delete pushButtonZakonczWszystkieTransmisjeModbus;

    delete vboxModbus;
    delete groupBoxModbus;
    delete ui;
}






void MainWindow::modbusRozlaczKlikniety()
{
    statusBar()->showMessage(tr("Trwa rozłączanie z Modbusem"));
    this->setEnabled(false);
    logika->przyciskRozlaczModbus();
}






void MainWindow::wyswietlObrazekKlikniety()
{ 
    logika->przyciskWyswietlObrazek(sliderGranicaSkaliSzarosci->value(),checkboxZachowacProporcje->isChecked());
    sliderGranicaSkaliSzarosci->setEnabled(true);
    pushButtonAutomatyczneProgowanie->setEnabled(true);
}





void MainWindow::wyslijObrazModbusKlikniety()
{
    checkboxAutomatyczneWysylanieModbus->setEnabled(false);
    pushButtonZatrzymajWysylanieObrazuModbus->setEnabled(true);
    przyciskiModbusEnabled(false);
    logika->przyciskWyslijObrazModbus(spinBoxAdresUrzadzenia->value());
}

void MainWindow::zatrzymajWysylanieObrazModbusKlikniety()
{
    pushButtonZatrzymajWysylanieObrazuModbus->setEnabled(false);
    logika->przyciskZatrzymajWysylanieObrazModbus(spinBoxAdresUrzadzenia->value());
}

void MainWindow::wlaczTrybGraficznyModbusKlikniety()
{
    checkboxAutomatyczneWysylanieModbus->setEnabled(false);
    przyciskiModbusEnabled(false);
    logika->przyciskWlaczTrybGraficznyModbus(true,spinBoxAdresUrzadzenia->value());
}

void MainWindow::wylaczTrybGraficznyModbusKlikniety()
{
    checkboxAutomatyczneWysylanieModbus->setEnabled(false);
    przyciskiModbusEnabled(false);
    logika->przyciskWlaczTrybGraficznyModbus(false,spinBoxAdresUrzadzenia->value());
}


void MainWindow::automatyczneWysylanieModbusKlikniety()
{
    if(checkboxAutomatyczneWysylanieModbus->checkState() == Qt::Unchecked)
    {
        logika->przyciskWlaczTrybAutomatycznyModbus(spinBoxAdresUrzadzenia->value(), false);


    }
    else if(checkboxAutomatyczneWysylanieModbus->checkState() == Qt::Checked)
    {
        przyciskiModbusEnabled(false);
        logika->przyciskWlaczTrybAutomatycznyModbus(spinBoxAdresUrzadzenia->value(), true);

    }
}



void MainWindow::brakOdpowiedziSlave(int adresUrzadzenia)
{
    pushButtonWyslijObrazModbus->setEnabled(true);
    QMessageBox::warning(this,"Ostrzeżenie","Brak lub niepoprawna odpowiedź urządzenia o numerze  " +  QString::number(adresUrzadzenia) + ".\n" + "Informacja mogła zostać nieotrzymana przez urządzenie.\n" + "Sprawdź połączenie." );
}

void MainWindow::otwarcieOknaPolaczeniaModbus()
{
    dialogOkno = new DialogPolaczModbus();
    dialogOkno->setParent(this);
    dialogOkno->setWindowFlag(Qt::Dialog);
    dialogOkno->setModal(Qt::ApplicationModal);
    dialogOkno->show();
    connect(dialogOkno,SIGNAL(przyciskOkKlikniety(ParametryModbus)),logika,SLOT(polaczModbus(ParametryModbus)));
    connect(dialogOkno,SIGNAL(oknoZamkniete()),logika,SLOT(dialogOknoZamkniete()));
}

void MainWindow::otwarcieOknaPolaczeniaModbus(std::string predkoscTransmisji, std::string liczbaBitow, std::string parity, bool czyTrwaTransmisja)
{
    dialogOkno = new DialogPolaczModbus(predkoscTransmisji,liczbaBitow,parity);
    dialogOkno->setCzyTrwaTransmisja(czyTrwaTransmisja);
    dialogOkno->setParent(this);
    dialogOkno->setWindowFlag(Qt::Dialog);
    dialogOkno->setModal(Qt::ApplicationModal);
    dialogOkno->show();
    connect(dialogOkno,SIGNAL(przyciskOkKlikniety(ParametryModbus)),logika,SLOT(edytujModbus(ParametryModbus)));
    connect(dialogOkno,SIGNAL(oknoZamkniete()),logika,SLOT(dialogOknoZamkniete()));
}

void MainWindow::brakPolaczeniaCom()
{
    QMessageBox::warning(this,"Ostrzeżenie","Brak połączenia z konwerterem." );

}


void MainWindow::czyWylaczycWszystkieTransmisjeTrybRozgloszeniowy(int adresUrzadzenia, int czynnosc)
{
    QMessageBox msgBox;
    msgBox.setText("Aby wywołać zapytanie w trybie rozgłoszeniowym konieczne jest zakończenie innych transmisji.");
    msgBox.setInformativeText("Czy jesteś pewien?");
    msgBox.setWindowFlags(Qt::WindowType::WindowStaysOnTopHint);
    msgBox.setIcon(QMessageBox::Information);
    QPushButton *pushButtonTak = msgBox.addButton(tr("Tak"), QMessageBox::YesRole);
    QPushButton *pushButtonNie = msgBox.addButton(tr("Anuluj"), QMessageBox::NoRole);


    msgBox.exec();

    if (msgBox.clickedButton() == pushButtonTak)
    {
        delete pushButtonTak;
        delete pushButtonNie;
        if(czynnosc == 1)
        {
            logika->zakonczWyslijDoUrzadzenia(adresUrzadzenia);
        }
        else if( czynnosc == 2)
        {
            logika->zakonczWlaczTrybGraficzny(adresUrzadzenia, true);
        }
        else if( czynnosc == 3)
        {
            logika->zakonczWlaczTrybGraficzny(adresUrzadzenia, false);
        }
        else if( czynnosc == 4)
        {
            logika->zakonczAutomatyczneWysylanie(adresUrzadzenia,true);
        }
        else if( czynnosc == 5)
        {
            logika->zakonczAutomatyczneWysylanie(adresUrzadzenia,false);
        }

    } else
    {
        delete pushButtonTak;
        delete pushButtonNie;
        zmienStanPrzyciskowModbus(logika->gettablicaUrzadzenPracujacych(adresUrzadzenia));
    }
}

void MainWindow::maksymalnaIloscTransmisjiZostalaOsiagnieta()
{
    QMessageBox::information(this,"Informacja","Maksymalna ilość jednoczesnych transmisji dla twojego urządzenia została osiągnięta. \n Zakończ inną transmisję, aby rozpocząć tą." );
}

void MainWindow::zmienIloscTrwajacychTransmisji(int liczbaTransmisji)
{
    labelLiczbaTrwajacychTransmisji->setText(QString::number(liczbaTransmisji));
    if(liczbaTransmisji == 0)
    {
        pushButtonZakonczWszystkieTransmisjeModbus->setEnabled(false);
    }
    else
    {
        pushButtonZakonczWszystkieTransmisjeModbus->setEnabled(true);
    }
}

void MainWindow::dialogOknoTrwajaTransmisje(bool tak)
{
    if(dialogOkno)
    {
        dialogOkno->setCzyTrwaTransmisja(tak);
    }

}

void MainWindow::setValueSliderGranicaSkaliSzarosci(int granica)
{
    sliderGranicaSkaliSzarosci->setValue(granica);
}






void MainWindow::dodawanieElemetowWczytywaniaGrafiki()
{
    groupBoxGrafika = new QGroupBox("Obraz");
    vboxGrafika = new QVBoxLayout();

    labelGrafika = new QLabel();
    QPixmap pix(256,128);
    pix.fill(QColor(Qt::white));
    labelGrafika->setPixmap(pix);
    labelGrafika->setMaximumSize(256,128);
    labelGrafika->setMinimumSize(256,128);
    labelGrafika->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    vboxGrafika->addWidget(labelGrafika);

    pushButtonOtworzObrazek = new QPushButton("Wczytaj obraz");
    QObject::connect(pushButtonOtworzObrazek, SIGNAL(clicked()),logika, SLOT(przyciskOtworzObrazek()));
    vboxGrafika->addWidget(pushButtonOtworzObrazek);

    checkboxZachowacProporcje = new QCheckBox("Zachować proporcje", this);
    checkboxZachowacProporcje->setStyleSheet("margin-left:50%; margin-right:50%;");
    checkboxZachowacProporcje->setEnabled(false);
    vboxGrafika->addWidget(checkboxZachowacProporcje);


    pushButtonWyswietlObrazek = new QPushButton("Wyświetl obraz");
    QObject::connect(pushButtonWyswietlObrazek, SIGNAL(clicked()),this, SLOT(wyswietlObrazekKlikniety()));
    pushButtonWyswietlObrazek->setEnabled(false);
    pushButtonWyswietlObrazek->setToolTip("Najpierw wczytaj obraz, aby móc go wyświetlić");
    vboxGrafika->addWidget(pushButtonWyswietlObrazek);

    groupBoxProgSkaliSzarosci = new QGroupBox("Próg binaryzacji");
    vboxProgSkaliSzarosci = new QVBoxLayout();
    sliderGranicaSkaliSzarosci = new QSlider(Qt::Horizontal,this);
    sliderGranicaSkaliSzarosci->setMinimum(0);
    sliderGranicaSkaliSzarosci->setMaximum(255);
    sliderGranicaSkaliSzarosci->setTickPosition(QSlider::TicksBelow);
    sliderGranicaSkaliSzarosci->setTickInterval(5);
    sliderGranicaSkaliSzarosci->setValue(127);
    QObject::connect(sliderGranicaSkaliSzarosci, SIGNAL(valueChanged(int)),logika, SLOT(sliderGranicaSkaliSzarosciZmienilWartosc(int)));
    sliderGranicaSkaliSzarosci->setEnabled(false);
    vboxProgSkaliSzarosci->addWidget(sliderGranicaSkaliSzarosci);

    pushButtonAutomatyczneProgowanie = new QPushButton("Automatyczne progowanie");
    QObject::connect(pushButtonAutomatyczneProgowanie, SIGNAL(clicked()),logika, SLOT(przyciskAutomatyczneProgowanie()));
    pushButtonAutomatyczneProgowanie->setEnabled(false);
    pushButtonAutomatyczneProgowanie->setToolTip("Najpierw wczytaj obraz, aby móc go wyświetlić");
    vboxProgSkaliSzarosci->addWidget(pushButtonAutomatyczneProgowanie);

    vboxGrafika->addStretch(1);
    groupBoxGrafika->setLayout(vboxGrafika);
    groupBoxGrafika->setMaximumWidth(260);
    vboxPasekBoczny->addWidget(groupBoxGrafika,0,Qt::AlignTop);

    vboxProgSkaliSzarosci->addStretch(1);
    groupBoxProgSkaliSzarosci->setLayout(vboxProgSkaliSzarosci);
    groupBoxProgSkaliSzarosci->setMaximumWidth(260);
    vboxPasekBoczny->addWidget(groupBoxProgSkaliSzarosci,0,Qt::AlignTop);

}

void MainWindow::dodawanieElementowObslugiModbus()
{
    groupBoxModbus = new QGroupBox("Modbus");
    vboxModbus = new QVBoxLayout();
    labelAdresUrzadzenia = new QLabel("Adres urządzenia");
    spinBoxAdresUrzadzenia = new QSpinBox();
    QObject::connect(spinBoxAdresUrzadzenia, SIGNAL(valueChanged(int)),logika, SLOT(pokazOdpowiedniePrzyciskiModbus(int)));
    spinBoxAdresUrzadzenia->setMinimum(0);
    spinBoxAdresUrzadzenia->setMaximum(255);
    spinBoxAdresUrzadzenia->setValue(1);
    spinBoxAdresUrzadzenia->setMaximumWidth(50);
    adresUrzadzeniaLayout = new QHBoxLayout();
    adresUrzadzeniaLayout->addWidget(labelAdresUrzadzenia);
    adresUrzadzeniaLayout->addWidget(spinBoxAdresUrzadzenia);
    vboxModbus->addLayout(adresUrzadzeniaLayout);

    pushButtonWlaczTrybGraficznyModbus= new QPushButton("Włącz tryb graficzny");
    QObject::connect(pushButtonWlaczTrybGraficznyModbus, SIGNAL(clicked()),this, SLOT(wlaczTrybGraficznyModbusKlikniety()));
    pushButtonWylaczTrybGraficznyModbus= new QPushButton("Wyłącz tryb graficzny");
    QObject::connect(pushButtonWylaczTrybGraficznyModbus, SIGNAL(clicked()),this, SLOT(wylaczTrybGraficznyModbusKlikniety()));
    trybGraficznyLayout = new QHBoxLayout();
    trybGraficznyLayout->addWidget(pushButtonWlaczTrybGraficznyModbus);
    trybGraficznyLayout->addWidget(pushButtonWylaczTrybGraficznyModbus);
    vboxModbus->addLayout(trybGraficznyLayout);

    wyslijZatrzymajLayout = new QHBoxLayout();
    pushButtonWyslijObrazModbus = new QPushButton("Wyślij obraz");
    QObject::connect(pushButtonWyslijObrazModbus, SIGNAL(clicked()),this, SLOT(wyslijObrazModbusKlikniety()));
    pushButtonZatrzymajWysylanieObrazuModbus= new QPushButton("Zatrzymaj");
    QObject::connect(pushButtonZatrzymajWysylanieObrazuModbus, SIGNAL(clicked()),this, SLOT(zatrzymajWysylanieObrazModbusKlikniety()));
    pushButtonZatrzymajWysylanieObrazuModbus->setEnabled(false);
    wyslijZatrzymajLayout->addWidget(pushButtonWyslijObrazModbus);
    wyslijZatrzymajLayout->addWidget(pushButtonZatrzymajWysylanieObrazuModbus);
    vboxModbus->addLayout(wyslijZatrzymajLayout);

    checkboxAutomatyczneWysylanieModbus= new QCheckBox("Automatyczne wysyłanie");
    QObject::connect(checkboxAutomatyczneWysylanieModbus, SIGNAL(clicked()),this, SLOT(automatyczneWysylanieModbusKlikniety()));
    vboxModbus->addWidget(checkboxAutomatyczneWysylanieModbus,0,Qt::AlignHCenter);

    vboxModbus->addSpacing(20);
    labelIloscTrwajacychTransmisji = new QLabel("Ilość trwających transmisji: ");
    labelLiczbaTrwajacychTransmisji = new QLabel("0");
    trwajaceTransmisjeLayout = new QHBoxLayout();
    trwajaceTransmisjeLayout->addWidget(labelIloscTrwajacychTransmisji,0,Qt::AlignRight);
    trwajaceTransmisjeLayout->addWidget(labelLiczbaTrwajacychTransmisji,0,Qt::AlignLeft);
    vboxModbus->addLayout(trwajaceTransmisjeLayout);

    pushButtonZakonczWszystkieTransmisjeModbus = new QPushButton("Zakończ wszystkie transmisje");
    pushButtonZakonczWszystkieTransmisjeModbus->setEnabled(false);
    QObject::connect(pushButtonZakonczWszystkieTransmisjeModbus, SIGNAL(clicked()),logika, SLOT(przyciskZakonczWszystkieTransmisjeModbusModbus()));
    vboxModbus->addWidget(pushButtonZakonczWszystkieTransmisjeModbus,0,Qt::AlignHCenter);

    groupBoxModbus->setLayout(vboxModbus);

    groupBoxModbus->setEnabled(false);
    groupBoxModbus->setToolTip("Połącz się z Modbusem");


    vboxPasekBoczny->addWidget(groupBoxModbus,0,Qt::AlignTop);
}

void MainWindow::tworzenieMenuGornego()
{

    menuModbus = menuBar()->addMenu(tr("&Modbus"));
    menuModbus->addAction(actionModbusPolacz);
    menuModbus->addAction(actionModbusEdytuj);
    menuModbus->addAction(actionModbusRozlacz);
    menuModbus->setToolTipsVisible(true);

    menuOpcje = menuBar()->addMenu(tr("&Opcje"));
    menuOpcje->addAction(actionWyswietlaczOpcje);
    menuOpcje->addAction(actionObrobkaObrazuOpcje);
    menuOpcje->setToolTipsVisible(true);

}

void MainWindow::tworzenieAction()
{
    actionModbusPolacz = new QAction(tr("&Połącz"), this);
    connect(actionModbusPolacz, SIGNAL(triggered()), logika, SLOT(przyciskPolaczModbus()));

    actionModbusEdytuj = new QAction(tr("&Edytuj"), this);
    actionModbusEdytuj->setEnabled(false);
    connect(actionModbusEdytuj, SIGNAL(triggered()), logika, SLOT(przyciskEdytujModbus()));

    actionModbusRozlacz = new QAction(tr("&Rozłącz"), this);
    actionModbusRozlacz->setEnabled(false);
    connect(actionModbusRozlacz, SIGNAL(triggered()), this, SLOT(modbusRozlaczKlikniety()));


    actionWyswietlaczOpcje = new QAction(tr("&Wyświetlacz"), this);
    connect(actionWyswietlaczOpcje, SIGNAL(triggered()), logika, SLOT(przyciskWyswietlaczOpcje()));

    actionObrobkaObrazuOpcje = new QAction(tr("&Obróbka obrazu"), this);
    connect(actionObrobkaObrazuOpcje, SIGNAL(triggered()), logika, SLOT(przyciskObrobkaObrazuOpcje()));

}



void MainWindow::przyciskiModbusEnabled(bool czyWlaczyc)
{
    pushButtonWyslijObrazModbus->setEnabled(czyWlaczyc);
    pushButtonWlaczTrybGraficznyModbus->setEnabled(czyWlaczyc);
    pushButtonWylaczTrybGraficznyModbus->setEnabled(czyWlaczyc);
    if(!czyWlaczyc)
    {
        pushButtonWyslijObrazModbus->setToolTip("Trwa transmisja");
        pushButtonWlaczTrybGraficznyModbus->setToolTip("Trwa transmisja");
        pushButtonWylaczTrybGraficznyModbus->setToolTip("Trwa transmisja");
    }
    else
    {
        pushButtonWyslijObrazModbus->setToolTip("");
        pushButtonWlaczTrybGraficznyModbus->setToolTip("");
        pushButtonWylaczTrybGraficznyModbus->setToolTip("");
    }
}

void MainWindow::zmienStanPrzyciskowModbus(coRobiUrzadzenie coRobi)
{
    if(coRobi == nic)
    {
        pushButtonWyslijObrazModbus->setEnabled(true);
        pushButtonZatrzymajWysylanieObrazuModbus->setEnabled(false);
        pushButtonWlaczTrybGraficznyModbus->setEnabled(true);
        pushButtonWylaczTrybGraficznyModbus->setEnabled(true);
        checkboxAutomatyczneWysylanieModbus->setEnabled(true);
        checkboxAutomatyczneWysylanieModbus->setCheckState(Qt::Unchecked);
        pushButtonWyslijObrazModbus->setToolTip("");
        pushButtonWlaczTrybGraficznyModbus->setToolTip("");
        pushButtonWylaczTrybGraficznyModbus->setToolTip("");
        checkboxAutomatyczneWysylanieModbus->setToolTip("");

    }
    else if(coRobi == trybManualny)
    {
        pushButtonWyslijObrazModbus->setEnabled(false);
        pushButtonZatrzymajWysylanieObrazuModbus->setEnabled(true);
        pushButtonWlaczTrybGraficznyModbus->setEnabled(false);
        pushButtonWylaczTrybGraficznyModbus->setEnabled(false);
        checkboxAutomatyczneWysylanieModbus->setEnabled(true);
        checkboxAutomatyczneWysylanieModbus->setCheckState(Qt::Unchecked);
        pushButtonWyslijObrazModbus->setToolTip("Trwa transmisja");
        pushButtonWlaczTrybGraficznyModbus->setToolTip("Trwa transmisja");
        pushButtonWylaczTrybGraficznyModbus->setToolTip("Trwa transmisja");
        checkboxAutomatyczneWysylanieModbus->setToolTip("");
    }
    else if(coRobi == trybAuto)
    {
        pushButtonWyslijObrazModbus->setEnabled(false);
        pushButtonZatrzymajWysylanieObrazuModbus->setEnabled(false);
        pushButtonWlaczTrybGraficznyModbus->setEnabled(false);
        pushButtonWylaczTrybGraficznyModbus->setEnabled(false);
        checkboxAutomatyczneWysylanieModbus->setEnabled(true);
        checkboxAutomatyczneWysylanieModbus->setCheckState(Qt::Checked);
        pushButtonWyslijObrazModbus->setToolTip("Trwa automatyczna transmisja");
        pushButtonWlaczTrybGraficznyModbus->setToolTip("Trwa automatyczna transmisja");
        pushButtonWylaczTrybGraficznyModbus->setToolTip("Trwa automatyczna transmisja");
        checkboxAutomatyczneWysylanieModbus->setToolTip("");
    }
    else
    {
        pushButtonWyslijObrazModbus->setEnabled(false);
        pushButtonZatrzymajWysylanieObrazuModbus->setEnabled(false);
        pushButtonWlaczTrybGraficznyModbus->setEnabled(false);
        pushButtonWylaczTrybGraficznyModbus->setEnabled(false);
        checkboxAutomatyczneWysylanieModbus->setEnabled(false);
        checkboxAutomatyczneWysylanieModbus->setCheckState(Qt::Unchecked);
        pushButtonWyslijObrazModbus->setToolTip("Tryb rozgloszeniowy wlaczony");
        pushButtonWlaczTrybGraficznyModbus->setToolTip("Tryb rozgloszeniowy wlaczony");
        pushButtonWylaczTrybGraficznyModbus->setToolTip("Tryb rozgloszeniowy wlaczony");
        checkboxAutomatyczneWysylanieModbus->setToolTip("Tryb rozgloszeniowy wlaczony");
    }
}

void MainWindow::zakonczenieWysylania(int adresUrzadzenia)
{
    if(adresUrzadzenia == spinBoxAdresUrzadzenia->value() || adresUrzadzenia == 0)
    {
        zmienStanPrzyciskowModbus(nic);
    }
}

void MainWindow::zamknijOkno()
{
    delete dialogOkno;
}
