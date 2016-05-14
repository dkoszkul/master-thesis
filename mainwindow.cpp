#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    simSettingsDialog = new SimSettingsDialog;
    matlabExporter = new MatlabExporter;
    algorithm = new Algorithm;
    algorithm->setMainWindow(this);

    ui->setupUi(this);

    console->setEnabled(false);
    ui->consoleLayout->addWidget(console);
    ui->connectButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);
    ui->uartSettingsButton->setEnabled(true);

    setCentralWidget(ui->tabWidget);

    matlabExporter->setSimulation(simulation);

    initActionsConnections();
    setupAlgorithmResultTab();

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
    setupSceneViewTab();

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

    resultPlot->detachItems();
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->attach( resultPlot );
    grid->setPen(QPen(Qt::gray));
    resultPlot->replot();


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

void MainWindow::handleExportAlgorithmResults()
{

    algorithm->exportAlgorithmResultsToMatlabScript();
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
    connect(ui->loadRealResultsButton, SIGNAL(clicked()), algorithm, SLOT(handleRealResults()));
    connect(ui->saveAlgorithmResults, SIGNAL(clicked()),this, SLOT(handleExportAlgorithmResults()));

    connect(ui->settingsButton, SIGNAL(clicked()), simSettingsDialog, SLOT(show()));
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
    std::vector<Receiver *> receivers = simulation->getReceivers();

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

void MainWindow::setupAlgorithmResultTab()
{
    resultPlot = new QwtPlot;
    resultPlot->setAxisScale( plot->xBottom, 0.0, 1500.0 );
    resultPlot->setAxisScale( plot->yLeft, -90.0, 90.0);
    resultPlot->setAxisTitle(plot->yLeft,"angle [degree]");
    resultPlot->setAxisTitle(plot->xBottom,"time [us]");
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->attach( resultPlot );
    grid->setPen(QPen(Qt::gray));


    simulation->setResultPlot(resultPlot);
    algorithm->setAlgorithmResultPlot(resultPlot);

    ui->resultVerticalLayout->addWidget(resultPlot);
}

void MainWindow::setupSceneViewTab()
{
    scene = new QwtPlot;
    int xMin=0, xMax=0, yMin=0, yMax=0;

    xMin = xMax = simulation->getEmitter()->getPositionX();
    yMin = yMax = simulation->getEmitter()->getPositionY();

    QwtPlotMarker* m = new QwtPlotMarker();
    m->setSymbol(new QwtSymbol( QwtSymbol::Ellipse, Qt::black, Qt::NoPen, QSize( 10, 10 ) ) );
    m->setValue( QPointF( simulation->getEmitter()->getPositionX(), simulation->getEmitter()->getPositionY()) );
    m->attach( scene );

    for(auto r=simulation->getReceivers().begin();r!=simulation->getReceivers().end();r++){
        int receiverX = (*r)->getPositionX();
        int receiverY = (*r)->getPositionY();
        if(receiverX<xMin){
            xMin = receiverX;
        }else if(receiverX > xMax){
            xMax = receiverX;
        }

        if(receiverY<yMin){
            yMin = receiverY;
        }else if(receiverY > yMax){
            yMax = receiverY;
        }
        QwtPlotMarker* m = new QwtPlotMarker();
        m->setSymbol(new QwtSymbol( QwtSymbol::Ellipse, Qt::blue, Qt::NoPen, QSize( 30, 30 ) ) );
        m->setValue( QPointF( receiverX, receiverY ) );
        m->attach( scene );
    }

    for(auto r=simulation->getObstacles().begin();r!=simulation->getObstacles().end();r++){
        int receiverX = (*r)->getPositionX();
        int receiverY = (*r)->getPositionY();
        if(receiverX<xMin){
            xMin = receiverX;
        }else if(receiverX > xMax){
            xMax = receiverX;
        }

        if(receiverY<yMin){
            yMin = receiverY;
        }else if(receiverY > yMax){
            yMax = receiverY;
        }
        QwtPlotMarker* m = new QwtPlotMarker();
        m->setSymbol(new QwtSymbol( QwtSymbol::Ellipse, Qt::black, Qt::NoPen, QSize( 30, 30 ) ) );
        m->setValue( QPointF( receiverX, receiverY ) );
        m->attach( scene );
    }

    scene->setAxisScale( plot->xBottom, xMin-15, xMax+15);
    scene->setAxisScale( plot->yLeft, yMin-15, yMax+15);
    scene->setAxisTitle(plot->yLeft,"Y distance [mm]");
    scene->setAxisTitle(plot->xBottom,"X distance [mm]");
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->attach( scene );
    grid->setPen(QPen(Qt::gray));

    ui->sceneVerticalLayout->addWidget(scene);
}

void MainWindow::createXAxisLine(QwtPlot* plot)
{
    QwtPlotMarker *mY = new QwtPlotMarker();
    mY->setLabelAlignment( Qt::AlignRight | Qt::AlignTop );
    mY->setLineStyle( QwtPlotMarker::HLine );
    mY->setLinePen(QPen(Qt::gray));
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
