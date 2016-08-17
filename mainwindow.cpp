#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "portsetup.hpp"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSettings>
#include <QDebug>
#include <QList>
#include <unistd.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    serial(new QSerialPort(this))
{
    ui->setupUi(this);

    connect(serial, SIGNAL(readyRead()),
            this, SLOT(serialReceived()));
    connect(serial, SIGNAL(bytesWritten(qint64)),
            this, SLOT(serialWriten(qint64)));
    connect(serial, SIGNAL(readChannelFinished()),
            this, SLOT(finish()));

    connect(ui->savePushButton, SIGNAL(clicked()),
            this, SLOT(save()));

    portIndex.insert(0, 0);
    portString.insert( 0, tr("None") );
    ui->portsComboBox->addItem( tr("None") );
    int index = 1;
    // Получить список доступных посделовательных портов
    foreach (const QSerialPortInfo &info,
             QSerialPortInfo::availablePorts() ) {
        portIndex.insert(index, index);
        portString.insert( index, info.portName() );
        ui->portsComboBox->addItem( info.portName() );
        index++;

    }

    QSerialPortInfo info;
    QSerialPort::BaudRate br = QSerialPort::UnknownBaud;
    baudrateIndex.insert(0, 0);
    baudrateString.insert( 0, tr("None") );
    ui->baudRateComboBox->addItem( tr("None") );
    for (int var = 0, index = 1; var < info.standardBaudRates().size(); ++var) {
        qint32 value = info.standardBaudRates().at(var);
        switch (value) {
        case QSerialPort::Baud1200:
            br = QSerialPort::Baud1200;
            break;
        case QSerialPort::Baud2400:
            br = QSerialPort::Baud2400;
            break;
        case QSerialPort::Baud4800:
            br = QSerialPort::Baud4800;
            break;
        case QSerialPort::Baud9600:
            br = QSerialPort::Baud9600;
            break;
        case QSerialPort::Baud19200:
            br = QSerialPort::Baud19200;
            break;
        case QSerialPort::Baud38400:
            br = QSerialPort::Baud38400;
            break;
        case QSerialPort::Baud57600:
            br = QSerialPort::Baud57600;
            break;
        case QSerialPort::Baud115200:
            br = QSerialPort::Baud115200;
            break;
        }

        if (value == br) {
            baudrateIndex.insert(index, value);
            baudrateString.insert( index, QString().number(value) );
            ui->baudRateComboBox->addItem( QString().number(value) );
            index++;
        }
    }


    databitsIndex.insert(0, 0);
    databitsString.insert( 0, tr("None") );
    ui->dataBitsComboBox->addItem( tr("None") );

    for (int var = 5, index = 1; var < 9; ++var, ++index) {
        databitsIndex.insert(index, var);
        databitsString.insert( index, QString().number(var) );
        ui->dataBitsComboBox->addItem( QString().number(var) );
    }


    parityIndex.insert(0, 0);
    parityString.insert( 0, tr("None") );
    ui->parityComboBox->addItem( tr("None") );

    parityIndex.insert(1, 2);
    parityString.insert( 1, tr("Even") );
    ui->parityComboBox->addItem( tr("Even") );

    parityIndex.insert(2, 3);
    parityString.insert( 2, tr("Odd") );
    ui->parityComboBox->addItem( tr("Odd") );

    parityIndex.insert(3, 4);
    parityString.insert( 3, tr("Space") );
    ui->parityComboBox->addItem( tr("Space") );

    parityIndex.insert(4, 5);
    parityString.insert( 4, tr("Mark") );
    ui->parityComboBox->addItem( tr("Mark") );


    stopbitsIndex.insert(0, 0);
    stopbitsString.insert( 0, tr("None") );
    ui->stopBitsComboBox->addItem( tr("None") );

    stopbitsIndex.insert(1, 1);
    stopbitsString.insert( 1, tr("One") );
    ui->stopBitsComboBox->addItem( tr("One") );

    stopbitsIndex.insert(2, 3);
    stopbitsString.insert( 2, tr("One And Half") );
    ui->stopBitsComboBox->addItem( tr("One And Half") );

    stopbitsIndex.insert(3, 2);
    stopbitsString.insert( 3, tr("Two") );
    ui->stopBitsComboBox->addItem( tr("Two") );


    flowcontrolIndex.insert(0, 0);
    flowcontrolString.insert( 0, tr("None") );
    ui->flowControlComboBox->addItem( tr("None") );

    flowcontrolIndex.insert(1, 1);
    flowcontrolString.insert( 1, tr("Hardware") );
    ui->flowControlComboBox->addItem( tr("Hardware") );

    flowcontrolIndex.insert(2, 2);
    flowcontrolString.insert( 2, tr("Software") );
    ui->flowControlComboBox->addItem( tr("Software") );


    if (ui->portsComboBox->count() > 1) {
        load();
        ui->baudRateComboBox->setEnabled(true);
        ui->dataBitsComboBox->setEnabled(true);
        ui->parityComboBox->setEnabled(true);
        ui->stopBitsComboBox->setEnabled(true);
        ui->flowControlComboBox->setEnabled(true);
        ui->savePushButton->setEnabled(true);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    serial->close();
}

void MainWindow::on_openPushButton_clicked()
{
    serial->open(QIODevice::ReadWrite);

    if(serial->isOpen())
    {
        qDebug() << "Port is opened!";
    }


    if(serial->isWritable())
    {
        qDebug() << "Yes, i can write to port!";
    }


}

void MainWindow::on_closePushButton_clicked()
{
    serial->close();
}

void MainWindow::on_writePushButton_clicked()
{
    // Записываем данные с конвертацией из формата const char[] в формат HEX
    // QByteArray::fromHex("000BDF2F2F4A32");

    serial->clear();
    //    serial->setFlowControl(QSerialPort::HardwareControl);

    qint64 bytes = serial->write( QByteArray::fromHex(ui->writeDataInPortLineEdit->text().toLatin1()),
                                  QByteArray::fromHex(ui->writeDataInPortLineEdit->text().toLatin1()).size() );


    qDebug() << "bytes: " << bytes;
}

void MainWindow::on_portsComboBox_currentIndexChanged(const QString &arg1)
{
    QSerialPortInfo info(arg1);

    if (!info.isNull()) {
        ui->manufacturerValueLabel->setText(info.manufacturer());
        ui->descriptionValueLabel->setText(info.description());
        ui->serialNumberValueLabel->setText(info.serialNumber());
        ui->vendorIdentifierValueLabel->setText(QString(info.vendorIdentifier()).toLocal8Bit().toHex());
        ui->productIdentifierValueLabel->setText(QString(info.productIdentifier()).toLocal8Bit().toHex());
        ui->systemLocationValueLabel->setText(info.systemLocation());
    }
    else {
        ui->manufacturerValueLabel->clear();
        ui->descriptionValueLabel->clear();
        ui->serialNumberValueLabel->clear();
        ui->vendorIdentifierValueLabel->clear();
        ui->productIdentifierValueLabel->clear();
        ui->systemLocationValueLabel->clear();
    }

}

void MainWindow::serialReceived()
{
    ui->readValueLabel->clear();
    int byte = 1;

//    QByteArray bytes = serial->readAll();



    const qint64 bytesAvailable = serial->bytesAvailable();
//    if (bytesAvailable < byte)
//        return;
    static QByteArray bytes = 0;

    bytes = serial->read(byte);

    ui->readValueLabel->setText(bytes.toHex().toUpper());
    qDebug() << QString(bytes.toHex().toUpper());
}

void MainWindow::serialWriten(qint64 data)
{
    qDebug() << "Byte writing: " << data;
}

void MainWindow::finish()
{
    qDebug() << "finis!";
}

void MainWindow::load()
{
    readSettings();
    setPortSettings();
    getPortSettings();
}

void MainWindow::save()
{
    setPortSettings();
    writeSettings();
    getPortSettings();
}

void MainWindow::readSettings()
{
    QSettings settings("MySoft", "Serial Port");

    restoreGeometry(settings.value("geometryMainWindow").toByteArray());

    ui->portsComboBox->setCurrentIndex(settings.value("port").toInt());
    ui->baudRateComboBox->setCurrentIndex(settings.value("baudRate").toInt());
    ui->dataBitsComboBox->setCurrentIndex(settings.value("dataBits").toInt());
    ui->parityComboBox->setCurrentIndex(settings.value("parity").toInt());
    ui->stopBitsComboBox->setCurrentIndex(settings.value("stopBits").toInt());
    ui->flowControlComboBox->setCurrentIndex(settings.value("flowControl").toInt());
}

void MainWindow::writeSettings()
{
    QSettings settings("MySoft", "Serial Port");

    settings.setValue("geometryMainWindow", saveGeometry());

    settings.setValue("port", ui->portsComboBox->currentIndex());
    settings.setValue("baudRate", ui->baudRateComboBox->currentIndex());
    settings.setValue("dataBits", ui->dataBitsComboBox->currentIndex());
    settings.setValue("parity", ui->parityComboBox->currentIndex());
    settings.setValue("stopBits", ui->stopBitsComboBox->currentIndex());
    settings.setValue("flowControl", ui->flowControlComboBox->currentIndex());
}

void MainWindow::setPortSettings()
{
    QString port = "None";
    port = portString.find(ui->portsComboBox->currentIndex()).value();
    serial->setPortName(port);

    QSerialPort::BaudRate br = QSerialPort::UnknownBaud;
    br = (QSerialPort::BaudRate)baudrateIndex.find(ui->baudRateComboBox->currentIndex()).value();
    serial->setBaudRate(br);

    QSerialPort::DataBits db = QSerialPort::UnknownDataBits;
    db = (QSerialPort::DataBits)databitsIndex.find(ui->dataBitsComboBox->currentIndex()).value();
    serial->setDataBits(db);

    QSerialPort::Parity par = QSerialPort::UnknownParity;
    par = (QSerialPort::Parity)parityIndex.find(ui->parityComboBox->currentIndex()).value();
    serial->setParity(par);

    QSerialPort::StopBits sb = QSerialPort::UnknownStopBits;
    sb = (QSerialPort::StopBits)stopbitsIndex.find(ui->stopBitsComboBox->currentIndex()).value();
    serial->setStopBits(sb);

    QSerialPort::FlowControl fc = QSerialPort::UnknownFlowControl;
    fc = (QSerialPort::FlowControl)flowcontrolIndex.find(ui->flowControlComboBox->currentIndex()).value();
    serial->setFlowControl(fc);
}

void MainWindow::getPortSettings()
{
    qDebug() << serial->portName();
    qDebug() << serial->baudRate();
    qDebug() << serial->dataBits();
    qDebug() << serial->parity();
    qDebug() << serial->stopBits();
    qDebug() << serial->flowControl();
}
