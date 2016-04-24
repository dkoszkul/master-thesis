#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtGlobal>
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QMessageBox>
#include <QLabel>
#include <QtSerialPort/QSerialPort>
#include <QFileDialog>

#include "console/console.h"
#include "simulation/signal.h"
#include "simulation/simulation.h"

#include "paintwidget.h"
#include "console/settingsdialog.h"
#include "matlab/matlabexporter.h"

#include <iostream>
#include <sstream>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_point_data.h>
#include <QPixmap>
#include <QImage>

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

    void createXAxisLine(QwtPlot* plot);
    
private slots:
    void openSerialPort();
    void closeSerialPort();
    void writeData(const QByteArray &data);
    void readData();
    void sendStartSignal();
    void sendStopSignal();
    void handleError(QSerialPort::SerialPortError error);
    void openAndLoadConfiguration();
    void handleClearButton();

private:
    Ui::MainWindow *ui;
    Simulation* simulation;
    Signal* signal;

    void initActionsConnections();
    void showStatusMessage(const QString &message);

    QLabel *status;
    Console *console;
    QSerialPort *serial;
    SettingsDialog *settings;
    MatlabExporter *matlabExporter;

    void setupSimulationTab();

    QwtPlot* plot;
    void processLine(QString line);
};

#endif // MAINWINDOW_H
