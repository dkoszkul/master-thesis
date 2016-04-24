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

}

void MainWindow::handleClearButton()
{

}


void MainWindow::initActionsConnections()
{
    connect(ui->simulate,           &QPushButton::clicked, simulation,      &Simulation::simulate);

    connect(serial,                 &QSerialPort::error(QSerialPort::SerialPortError), this, &MainWindow::handleError(QSerialPort::SerialPortError));
    connect(serial,                 &QSerialPort::readyRead, this,          &MainWindow::readData);
    connect(console,                &Console::getData(QByteArray), this,    &MainWindow::writeData(QByteArray));

    connect(ui->uartSettingsButton, &QPushButton::clicked,  settings,       &SettingsDialog::show);
    connect(ui->connectButton,      &QPushButton::clicked,  this,           &MainWindow::openSerialPort);
    connect(ui->disconnectButton,   &QPushButton::clicked,  this,           &MainWindow::closeSerialPort);
    connect(ui->clearConsoleButton, &QPushButton::clicked,  console,        &Console::clear);

    connect(ui->startButton,        &QPushButton::clicked,  this,           &MainWindow::sendStartSignal);
    connect(ui->stopButton,         &QPushButton::clicked,  this,           &MainWindow::sendStopSignal);

    connect(ui->load,               &QPushButton::clicked,  this,           &MainWindow::openAndLoadConfiguration);
    connect(ui->clear,              &QPushButton::clicked,  this,           &MainWindow::handleClearButton);

    /* connections for plot tab */
    connect(ui->save,               &QPushButton::clicked,  matlabExporter, &MatlabExporter::exportResults);
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
