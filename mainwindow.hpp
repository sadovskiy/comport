#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QMultiMap>
#include <QSerialPort>

class QSerialPort;
class PortSetup;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_openPushButton_clicked();

    void on_closePushButton_clicked();

    void on_writePushButton_clicked();

    void on_portsComboBox_currentIndexChanged(const QString &arg1);

    void serialReceived();

    void serialWriten(qint64 data);

    void finish();

    void load();
    void save();

private:
    Ui::MainWindow *ui;

    QMultiMap<int, qint32> portIndex;
    QMultiMap<int, QString> portString;

    QMultiMap<int, qint32> baudrateIndex;
    QMultiMap<int, QString> baudrateString;

    QMultiMap<int, qint32> databitsIndex;
    QMultiMap<int, QString> databitsString;

    QMultiMap<int, qint32> parityIndex;
    QMultiMap<int, QString> parityString;

    QMultiMap<int, qint32> stopbitsIndex;
    QMultiMap<int, QString> stopbitsString;

    QMultiMap<int, qint32> flowcontrolIndex;
    QMultiMap<int, QString> flowcontrolString;

    QSerialPort *serial;

    void readSettings();
    void writeSettings();

    void setPortSettings();
    void getPortSettings();
};

#endif // MAINWINDOW_HPP
