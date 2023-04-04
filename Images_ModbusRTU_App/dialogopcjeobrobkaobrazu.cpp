#include "dialogopcjeobrobkaobrazu.h"
#include "ui_dialogopcjeobrobkaobrazu.h"

DialogOpcjeObrobkaObrazu::DialogOpcjeObrobkaObrazu(algorytmObrobkiObrazu metodaSkalowania) :
    ui(new Ui::DialogOpcjeObrobkaObrazu)

{
    ui->setupUi(this);
    this->setWindowTitle("Opcje obróbki obrazu");
    this->setFixedSize(400,150);

    napisAlorytmZmianyRozmiaruObrazu = new QLabel("Algorytm zmiany rozmiaru obrazu");
    comboBoxAlorytmZmianyRozmiaruObrazu = new QComboBox();
    QStringList listaAlgorytmow = QStringList() << tr("Najbliższego sąsiada") << tr("Biliniowy") << tr("Bicubic") << tr("Box sampling") << tr("Autorski") ;
    comboBoxAlorytmZmianyRozmiaruObrazu->addItems(listaAlgorytmow);
    comboBoxAlorytmZmianyRozmiaruObrazu->setCurrentIndex(metodaSkalowania);


    boxLayoutPierwszy = new QHBoxLayout();
    boxLayoutPierwszy->addWidget(napisAlorytmZmianyRozmiaruObrazu);
    boxLayoutPierwszy->addWidget(comboBoxAlorytmZmianyRozmiaruObrazu);
    ui->verticalLayout->addLayout(boxLayoutPierwszy);

    ui->verticalLayout->addSpacing(30);

    pushButtonOK = new QPushButton("OK");
    pushButtonAnuluj = new QPushButton("Anuluj");

    boxLayoutDrugi = new QHBoxLayout();
    boxLayoutDrugi->addWidget(pushButtonOK);
    boxLayoutDrugi->addWidget(pushButtonAnuluj);
    ui->verticalLayout->addLayout(boxLayoutDrugi);

    QObject::connect(pushButtonOK, SIGNAL(clicked()),this, SLOT(przyciskOKKlik()));
    QObject::connect(pushButtonAnuluj, SIGNAL(clicked()),this, SLOT(przyciskAnulujKlik()));
}

DialogOpcjeObrobkaObrazu::~DialogOpcjeObrobkaObrazu()
{
    delete boxLayoutPierwszy;
    delete boxLayoutDrugi;
    delete napisAlorytmZmianyRozmiaruObrazu;
    delete comboBoxAlorytmZmianyRozmiaruObrazu;
    delete pushButtonOK;
    delete pushButtonAnuluj;
    delete ui;

}

void DialogOpcjeObrobkaObrazu::przyciskOKKlik()
{
    close();
    emit przyciskOkKlikniety((algorytmObrobkiObrazu)comboBoxAlorytmZmianyRozmiaruObrazu->currentIndex());
}

void DialogOpcjeObrobkaObrazu::przyciskAnulujKlik()
{
    close();
}

void DialogOpcjeObrobkaObrazu::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit oknoZamkniete();
}

void DialogOpcjeObrobkaObrazu::reject()
{
    close();
}
