/*!
  \file
  \brief Заголовочный файл класса по передаче данных по компорту.
        Весь механизм реализовано в главном окне приложения.

  Тут находятся определения класса MainWindow, в котором происходит работа с компортом.
  */

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

/*!
 * \brief MainWindow класс главного окна предназначен для работы с компортом.
 * \author Садовский Борис Сергеевич.
 * \version 1.0
 * \date Август 2016 года
 *
 * В стандартном классе реализованы несколько методов, которые осуществляют демонстрацию
 * основных моментов работы с компортом.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    /*!
     * \brief on_openPushButton_clicked метод срабатывающий при нажатии пользователем
     * кнопки открытия порта.
     */
    void on_openPushButton_clicked();

    /*!
     * \brief on_closePushButton_clicked метод срабатывающий при нажатии пользователем
     * кнопки закрытия порта.
     */
    void on_closePushButton_clicked();

    /*!
     * \brief on_writePushButton_clicked метод срабатывающий при нажатии пользователем
     * кнопки записи последовательности байт указанных в writeDataInPortLineEdit в порт.
     */
    void on_writePushButton_clicked();

    /*!
     * \brief on_portsComboBox_currentIndexChanged метод срабатывающий при смене пользователем
     * порта в списке доступных портов portsComboBox.
     * \param arg1 текущий выбранный порт.
     */
    void on_portsComboBox_currentIndexChanged(const QString &arg1);

    /*!
     * \brief serialReceived срабатывает при получении данных открытым портом и
     * обрабатывает полученные данные помещая прочитанные байты в readValueLabel.
     */
    void serialReceived();

    /*!
     * \brief serialWriten срабатывает если были записаны байты в порт, которые
     * передаются через параметр data.
     * \param data записанные в порт байты.
     */
    void serialWriten(qint64 data);

    /*!
     * \brief finish метод запускается по окончании записи.
     */
    void finish();

    /*!
     * \brief load срабатывает при запуске программы вызовом из конструктора.
     * Восстанавливает настройки предыдущего сеанса.
     */
    void load();

    /*!
     * \brief save срабатывает по нажатии пользователем кнопок сохранить.
     * Записывает настройки порта.
     */
    void save();

private:
    Ui::MainWindow *ui;

    /*! Ниже перечислены контейнеры для хранения параметров порта.
     * хранят в паре ключ - значение с ориентацией по индексу.
     * Ключ совпадает у каждой пары. Первый параметр в паре может быть
     * использован если нужен индекс, а второй если нужна строка настройки порта.
     */
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

    /// Прочитанный из порта массив байтов.
    QByteArray bytes;
    /// Число прочитанных байтов.
    qint64 bytesAvailable;

    /*!
     * \brief readSettings читает настройки из реестра или файла в зависимости от ОС.
     */
    void readSettings();

    /*!
     * \brief writeSettings записывает настройки.
     */
    void writeSettings();

    /*!
     * \brief setPortSettings задаёт настройки для порта перед его открытием.
     */
    void setPortSettings();


    /*!
     * \brief getPortSettings считывает текущие настройки порта для проверки их корректности.
     */
    void getPortSettings();
};

#endif // MAINWINDOW_HPP

