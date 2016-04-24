#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "paintwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    simulation = new Simulation;
    console = new Console;
    signal = new Signal;
    serial = new QSerialPort(this);
    status = new QLabel;
    settings = new SettingsDialog;
    matlabExporter = new MatlabExporter;

    ui->setupUi(this);

    console->setEnabled(false);
    ui->consoleLayout->addWidget(console);
    ui->connectButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);
    ui->uartSettingsButton->setEnabled(true);

    setCentralWidget(ui->tabWidget);

    matlabExporter->setSimulation(simulation);

    initActionsConnections();

    ui->simulate->setEnabled(false);
    ui->save->setEnabled(false);
    ui->clear->setEnabled(false);

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

void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}

void MainWindow::readData()
{
    QByteArray data = serial->readAll();
    console->putData(data);
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}

void MainWindow::openAndLoadConfiguration()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open simulation configuration file..."),
                "",
                "All files (*.*);;Configuration files (*.dk)"
                );
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        processLine(line);
    }

    setupSimulationTab();

    ui->load->setEnabled(false);
    ui->simulate->setEnabled(true);
    ui->save->setEnabled(true);
    ui->clear->setEnabled(true);

}

void MainWindow::handleClearButton()
{
    simulation->getReceivers().clear();
    simulation->getObstacles().clear();

    QLayout *layout = ui->plotLayout;
    QLayoutItem *item;
    while ((item = layout->takeAt(0))){
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
    layout->addItem(new QSpacerItem(0,0, QSizePolicy::Expanding, QSizePolicy::Minimum));

    ui->load->setEnabled(true);
    ui->simulate->setEnabled(false);
    ui->save->setEnabled(false);
    ui->clear->setEnabled(false);
}

void MainWindow::handleSave()
{
    QString filename = QFileDialog::getSaveFileName(
                this,
                tr("Save simulation results..."),
                "result.m",
                ".m"
                );
    matlabExporter->exportResults(&filename);
}


void MainWindow::initActionsConnections()
{
    connect(ui->simulate, SIGNAL(clicked()), simulation, SLOT(simulate()));

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(console, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));
    connect(ui->clearConsoleButton, SIGNAL(clicked()), console, SLOT(clear()));

    connect(ui->uartSettingsButton, SIGNAL(clicked()), settings, SLOT(show()));
    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(openSerialPort()));
    connect(ui->disconnectButton, SIGNAL(clicked()), this, SLOT(closeSerialPort()));
    connect(ui->startButton,SIGNAL(clicked()), this, SLOT(sendStartSignal()));
    connect(ui->stopButton,SIGNAL(clicked()), this, SLOT(sendStopSignal()));
    connect(ui->load, SIGNAL(clicked()), this, SLOT(openAndLoadConfiguration()));
    connect(ui->clear, SIGNAL(clicked()), this, SLOT(handleClearButton()));
    connect(ui->save, SIGNAL(clicked()), this, SLOT(handleSave()));
}

void MainWindow::showStatusMessage(const QString &message)
{
    status->setText(message);
}

void MainWindow::sendStartSignal(){
    if (serial->open(QIODevice::ReadWrite)) {
        writeData("start\r\n");
    }
}

void MainWindow::sendStopSignal(){
    if (serial->open(QIODevice::ReadWrite)) {
        writeData("stop\r\n");
    }

}

void MainWindow::setupSimulationTab()
{
    std::list<Receiver *> receivers = simulation->getReceivers();

    for(auto r=receivers.begin();r!=receivers.end();r++){
        std::cout<<(*r)->getReceiverNumber()<<std::endl;
        QwtPlot* plot = (*r)->getSignal()->getPlot();
        plot->setMaximumHeight(150);
        createXAxisLine(plot);
        ui->plotLayout->addWidget(plot);
    }

    plot = new QwtPlot();
    plot->setAxisScale( plot->xBottom, 0.0, 1500.0 );
    plot->setAxisScale( plot->yLeft, -1.0, 26);
    plot->setAxisTitle(plot->xBottom,"time [us]");
    plot->setAxisTitle(plot->yLeft,"Δt [us]");
    ui->plotLayout->addWidget(plot);


    simulation->setPlot(plot);

}

void MainWindow::createXAxisLine(QwtPlot* plot)
{
    QwtPlotMarker *mY = new QwtPlotMarker();
    mY->setLabelAlignment( Qt::AlignRight | Qt::AlignTop );
    mY->setLineStyle( QwtPlotMarker::HLine );
    mY->setYValue( 0.0 );
    mY->attach( plot );
}

void MainWindow::processLine(QString line)
{
    std::stringstream ss;
    ss<<line.toStdString();

    std::string name;
    int x,y;

    if(line.contains("emitter")){
        ss>>name>>x>>y;
        simulation->setEmitter(new Emitter(x,y,0));

    }else if(line.contains("receiver")){
        int receiverNumber;
        ss>>name>>receiverNumber>>x>>y;
        simulation->getReceivers().push_back(new Receiver(receiverNumber,x,y,0));

    }else if(line.contains("obstacle")){
        ss>>name>>x>>y;
        simulation->getObstacles().push_back(new Obstacle(x,y,0));
    }

}
