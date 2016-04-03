#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "paintwidget.h"
#include "console/settingsdialog.h"

#include <iostream>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    simulation = new Simulation();
    console = new Console;
    signal = new Signal();

    ui->setupUi(this);

    console->setEnabled(false);
   // console->setMaximumHeight(100);
    ui->consoleLayout->addWidget(console);

    serial = new QSerialPort(this);
    status = new QLabel;
    settings = new SettingsDialog;

    //ui->statusBar->addWidget(status);
    ui->connectButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);
    ui->uartSettingsButton->setEnabled(true);

    /* plots */
    //ui->plotLayout->addWidget();
    QwtPlot* plot = new QwtPlot();
    plot->setAxisScale( plot->xBottom, 0.0, 1500.0 );

    plot->setAxisScale( plot->yLeft, -1.0, 1.0 );
    QwtPlotCurve *cSin = new QwtPlotCurve( "y = sin(x)" );
    cSin->setRenderHint( QwtPlotItem::RenderAntialiased );
    cSin->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
    cSin->setPen( Qt::red );
    cSin->attach( plot );
    cSin->setData( new FunctionData( ::sin ) );
    ui->plotLayout->addWidget(plot);

    list<Obstacle*> obstacles;
    obstacles.push_back(new Obstacle(100,120,0));
    obstacles.push_back(new Obstacle(120,150,0));
    obstacles.push_back(new Obstacle(190,190,0));

    list<Receiver*> sensors;
    sensors.push_back(new Receiver(20,100,0));
    sensors.push_back(new Receiver(20,130,0));
    sensors.push_back(new Receiver(20,160,0));

    Emitter* emitter = new Emitter(0,125,0);

    simulation->setEmitter(emitter);
    simulation->setReceivers(sensors);
    simulation->setObstacles(obstacles);



    connect(ui->pushButton, &QPushButton::clicked, simulation, &Simulation::simulate);

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(console, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));

    connect(ui->uartSettingsButton, SIGNAL(clicked()), settings, SLOT(show()));
    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(openSerialPort()));
    connect(ui->disconnectButton, SIGNAL(clicked()), this, SLOT(closeSerialPort()));
    connect(ui->clearConsoleButton, SIGNAL(clicked()), console, SLOT(clear()));

    connect(ui->startButton,SIGNAL(clicked()), this, SLOT(sendStartSignal()));
    connect(ui->stopButton,SIGNAL(clicked()), this, SLOT(sendStopSignal()));

    /* connections for plot tab */
    connect(ui->plotsSignalsButton,&QPushButton::clicked, signal, &Signal::showSignals);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
        console->setEnabled(true);
        console->setLocalEchoEnabled(p.localEchoEnabled);
        ui->disconnectButton->setEnabled(true);
        ui->connectButton->setEnabled(false);
        ui->uartSettingsButton->setEnabled(false);
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        showStatusMessage(tr("Open error"));
    }
}
//! [4]

//! [5]
void MainWindow::closeSerialPort()
{
    if (serial->isOpen()){
        serial->close();
    }
    console->setEnabled(false);
    ui->disconnectButton->setEnabled(false);
    ui->connectButton->setEnabled(true);
    ui->uartSettingsButton->setEnabled(true);
    showStatusMessage(tr("Disconnected"));
}

//! [6]
void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}
//! [6]

//! [7]
void MainWindow::readData()
{
    QByteArray data = serial->readAll();
    console->putData(data);
    //ui->lcdNumber_3->display((ui->lcdNumber_3->value())+1);
}
//! [7]

//! [8]
void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}
//! [8]

void MainWindow::initActionsConnections()
{
    /*connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionConfigure, SIGNAL(triggered()), settings, SLOT(show()));
    connect(ui->actionClear, SIGNAL(triggered()), console, SLOT(clear()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));*/
}

void MainWindow::showStatusMessage(const QString &message)
{
    status->setText(message);
}

void MainWindow::sendStartSignal(){
    std::cout<<"start"<<std::endl;
  //  if (serial->open(QIODevice::ReadWrite)) {
        console->putData("start\r\n");
   // }
}

void MainWindow::sendStopSignal(){
    std::cout<<"stop"<<std::endl;
   // if (serial->open(QIODevice::ReadWrite)) {
        console->putData("stop\r\n");
   // }

}


