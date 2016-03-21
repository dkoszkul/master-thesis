#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "paintwidget.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->simulation = new Simulation();

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
}

MainWindow::~MainWindow()
{
    delete ui;
}
