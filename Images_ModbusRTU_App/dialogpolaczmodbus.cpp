#include "dialogpolaczmodbus.h"
#include "ui_dialogpolaczmodbus.h"




DialogPolaczModbus::DialogPolaczModbus() :
    ui(new Ui::DialogPolaczModbus)
{
    ui->setupUi(this);
    this->setFixedSize(300,300);
    this->setWindowTitle("Połączenie Modbus");
    edycja = false;
    napisPorty = new QLabel("Wybierz Port");
    comboBoxPorty = new QComboBox();
    for(int i = 1; i < 11; i ++)
    {
        std::string nazwa = "COM" + std::to_string(i);
        comboBoxPorty->addItem(QString::fromStdString(nazwa));
    }
    comboBoxPorty->setCurrentIndex(5);
    pierwszy = new QHBoxLayout();
    pierwszy->addWidget(napisPorty);
    pierwszy->addWidget(comboBoxPorty);
    ui->verticalLayout->addLayout(pierwszy);

    dodanieRzeczyEdytowalnych();
    comboBoxPredkoscTransmisji->setCurrentIndex(8);
    comboBoxLiczbaBitow->setCurrentIndex(1);
    comboBoxParity->setCurrentIndex(2);


}

DialogPolaczModbus::DialogPolaczModbus(std::string predkoscTransmisji, std::string liczbaBitow, std::string parity):
    ui(new Ui::DialogPolaczModbus)
{
    ui->setupUi(this);
    this->setFixedSize(300,250);
    this->setWindowTitle("Edycja parametrów Modbus");
    edycja = true;
    dodanieRzeczyEdytowalnych();
    comboBoxPredkoscTransmisji->setCurrentText(QString::fromStdString(predkoscTransmisji));
    comboBoxLiczbaBitow->setCurrentText(QString::fromStdString(liczbaBitow));
    comboBoxParity->setCurrentText(QString::fromStdString(parity));

}

DialogPolaczModbus::~DialogPolaczModbus()
{
    if(!edycja)
    {
        delete comboBoxPorty;
        delete pierwszy;
        delete napisPorty;
    }
    delete comboBoxLiczbaBitow;
    delete comboBoxPredkoscTransmisji;
    delete comboBoxParity;
    delete pushButtonOK;
    delete pushButtonAnuluj;
    delete drugi;
    delete trzeci;
    delete czwarty;
    delete siodmy;
    delete napisPredkosciTransmisji;
    delete napisLiczbaBitow;
    delete napisParity;
    delete ui;
}

void DialogPolaczModbus::przyciskOKKlik()
{
    struct ParametryModbus pM;
    if(!edycja)
    {
       pM.port = comboBoxPorty->currentText().toStdString();
    }
    pM.predkoscTransmisji = comboBoxPredkoscTransmisji->currentText().toStdString();
    pM.liczbaBitow = comboBoxLiczbaBitow->currentText().toStdString();
    pM.parity = comboBoxParity->currentText().toStdString();

    if(this->czyTrwaTransmisja && edycja)
    {
        QMessageBox msgBox;
        msgBox.setText("Trwa transmisja do wyświetlaczy. Edycja parametrów połączenia automatycznie zakończy wszystkie transmisję.");
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
            emit przyciskOkKlikniety(pM);
        } else if (msgBox.clickedButton() == pushButtonNie) {
            delete pushButtonTak;
            delete pushButtonNie;
            przyciskAnulujKlik();
        }
    }
    else
    {
        close();
        emit przyciskOkKlikniety(pM);
    }

}

void DialogPolaczModbus::przyciskAnulujKlik()
{
    close();
}

void DialogPolaczModbus::dodanieRzeczyEdytowalnych()
{
    napisPredkosciTransmisji = new QLabel("Prędkość transmisji");
    comboBoxPredkoscTransmisji = new QComboBox();


    QStringList listaPredkosci = QStringList() << tr("110") << tr("300") << tr("600") << tr("1200") << tr("2400") << tr("4800") << tr("9600") << tr("14400") << tr("19200") << tr("38400")<< tr("56000") << tr("57600") << tr("115200") << tr("128000") << tr("256000");
    comboBoxPredkoscTransmisji->addItems(listaPredkosci);


    drugi = new QHBoxLayout();
    drugi->addWidget(napisPredkosciTransmisji);
    drugi->addWidget(comboBoxPredkoscTransmisji);
    ui->verticalLayout->addLayout(drugi);

    napisLiczbaBitow = new QLabel("Liczba bitów znaku");
    comboBoxLiczbaBitow = new QComboBox();
    comboBoxLiczbaBitow->addItem("7");
    comboBoxLiczbaBitow->addItem("8");


    trzeci = new QHBoxLayout();
    trzeci->addWidget(napisLiczbaBitow);
    trzeci->addWidget(comboBoxLiczbaBitow);
    ui->verticalLayout->addLayout(trzeci);

    napisParity = new QLabel("Parzystość");
    comboBoxParity = new QComboBox();
    comboBoxParity->addItem("Nieparzysty");
    comboBoxParity->addItem("Parzysty");
    comboBoxParity->addItem("Żaden");


    czwarty = new QHBoxLayout();
    czwarty->addWidget(napisParity);
    czwarty->addWidget(comboBoxParity);
    ui->verticalLayout->addLayout(czwarty);


    ui->verticalLayout->addSpacing(30);

    pushButtonOK = new QPushButton("OK");
    pushButtonAnuluj = new QPushButton("Anuluj");

    siodmy = new QHBoxLayout();
    siodmy->addWidget(pushButtonOK);
    siodmy->addWidget(pushButtonAnuluj);
    ui->verticalLayout->addLayout(siodmy);

    QObject::connect(pushButtonOK, SIGNAL(clicked()),this, SLOT(przyciskOKKlik()));
    QObject::connect(pushButtonAnuluj, SIGNAL(clicked()),this, SLOT(przyciskAnulujKlik()));
}

void DialogPolaczModbus::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit oknoZamkniete();
}

void DialogPolaczModbus::reject()
{
    close();
}






