#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtGlobal>
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>

#include <QMessageBox>
#include <QLabel>
#include <QtSerialPort/QSerialPort>

#include "simulation.h"
#include "console/console.h"

QT_BEGIN_NAMESPACE

class QLabel;

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class Console;
class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openSerialPort();
    void closeSerialPort();
    void writeData(const QByteArray &data);
    void readData();
    void sendStartSignal();
    void sendStopSignal();

    void handleError(QSerialPort::SerialPortError error);

private:
    Ui::MainWindow *ui;
    Simulation* simulation;

    void initActionsConnections();
    void showStatusMessage(const QString &message);

    QLabel *status;
    Console *console;
    QSerialPort *serial;
    SettingsDialog *settings;

};

#endif // MAINWINDOW_H
