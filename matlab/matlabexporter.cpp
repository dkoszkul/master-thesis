#include "matlabexporter.h"

MatlabExporter::MatlabExporter(QObject *parent) : QObject(parent)
{
    filePath ="/home/dominik/Desktop";
}

void MatlabExporter::setFilePath(const std::string &value)
{
    filePath = value;
}

void MatlabExporter::exportResults(){

    QString filename;
    filename.append(filePath.c_str());
    filename.append("/result.txt");
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream<<"figure()"<<endl;
        stream<<"grid on;"<<endl;
        stream<<"hold on;"<<endl;
        stream<<"axis equal;"<<endl;
        stream<<"% Receivers positions"<<endl;
        for(auto receiver = receivers.begin(); receiver != receivers.end();receiver++){
            stream<<"circle("<<(*receiver)->getPositionX()<<","<<(*receiver)->getPositionY()<<",5);"<<endl;
        }
        stream<<"% Emitter position"<<endl;
        stream<<"plot("<<emitter->getPositionX()<<","<<emitter->getPositionY()<<",'ko','MarkerSize',30);"<<endl;
        stream<<"% Obstacle positions"<<endl;
        for(auto obstacle = obstacles.begin(); obstacle != obstacles.end();obstacle++){
            stream<<"plot("<<(*obstacle)->getPositionX()<<","<<(*obstacle)->getPositionY()<<",'x','MarkerSize',18);"<<endl;
        }

    }
    file.close();

}

void MatlabExporter::setObstacles(const std::list<Obstacle *> &value)
{
    obstacles = value;
}

void MatlabExporter::setEmitter(Emitter *value)
{
    emitter = value;
}

void MatlabExporter::setReceivers(const std::list<Receiver *> &value)
{
    receivers = value;
}
