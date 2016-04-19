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
    filename.append("/result_");
    filename.append(std::to_string(QDateTime::currentMSecsSinceEpoch()).c_str());
    filename.append(".m");
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream<<"figure()"<<endl;
        stream<<"grid on;"<<endl;
        stream<<"hold on;"<<endl;
        stream<<"axis equal;"<<endl;
        stream<<endl<<"% Receivers positions"<<endl;
        std::list<Receiver *> receivers = simulation->getReceivers();
        for(auto receiver = receivers.begin(); receiver != receivers.end();receiver++){
            stream<<"circle("<<(*receiver)->getPositionX()<<","<<(*receiver)->getPositionY()<<",5,'b');"<<endl;
        }

        stream<<endl<<"% Emitter position"<<endl;
        Object *emitter = simulation->getEmitter();
        stream<<"plot("<<emitter->getPositionX()<<","<<emitter->getPositionY()<<",'ko','MarkerSize',30);"<<endl;

        stream<<endl<<"% Obstacle positions"<<endl;
        std::list<Obstacle *> obstacles = simulation->getObstacles();
        for(auto obstacle = obstacles.begin(); obstacle != obstacles.end();obstacle++){
            stream<<"plot("<<(*obstacle)->getPositionX()<<","<<(*obstacle)->getPositionY()<<",'x','MarkerSize',18);"<<endl;
        }
        stream<<endl<<"% wave data"<<endl;
        int receiverNumber = 1;
        stream<<"figure()"<<endl;

        for(auto receiver = receivers.begin(); receiver != receivers.end();receiver++, receiverNumber++){
            std::vector<double> signalX = (*receiver)->getSignal()->getSignalX();
            std::vector<double> signalY = (*receiver)->getSignal()->getSignalY();

            stream<<endl<< "x"<<receiverNumber<<"=[";
            for(auto x = signalX.begin(); x != signalX.end(); ++x) {
                stream<<" "<<(*x);
            }
            stream<<"];"<<endl;
            stream << "y"<<receiverNumber<<"=[";
            for(auto y = signalY.begin(); y != signalY.end(); ++y) {
                stream<<" "<<(*y);
            }
            stream<<"];"<<endl;
            stream<<"subplot("<<receivers.size()<<"1"<<receiverNumber<<")"<<endl;
            stream<<"grid on;"<<endl;
            stream<<"hold on;"<<endl;
            stream<<endl<<"plot(x"<<receiverNumber<<",y"<<receiverNumber<<");"<<endl;
            stream<<"xlabel('time [uS]');"<<endl<<"ylabel('signal amplitude');"<<endl;
        }

        stream<<endl<<"'--------------------------------'"<<endl;
        for(int i=0;i<receivers.size();i++){
            stream <<endl<< "x";
            stream<<std::to_string(i).c_str();
            stream<<"=[";
            std::map<int,std::vector<double>> timeByReceiverNumber = simulation->getTimeByReceiverNumber();
            for(auto it = timeByReceiverNumber[i].begin(); it != timeByReceiverNumber[i].end(); ++it) {
                stream<<" "<<(*it);
            }
            stream<<"];"<<endl;
            stream << "y";
            stream<<std::to_string(i).c_str();
            stream<<"=[";
            std::map<int,std::vector<double>> deltaTByReceiverNumber = simulation->getDeltaTByReceiverNumber();
            for(auto it = deltaTByReceiverNumber[i].begin(); it != deltaTByReceiverNumber[i].end(); ++it) {
                stream<<" "<<(*it);
            }
            stream<<"];"<<endl;
        }
        stream<<"figure()"<<endl;
        stream<<"grid on;"<<endl;
        stream<<"hold on;"<<endl;
        for(int i=0;i<receivers.size();i++){
            stream<<"plot(x"<<i<<",y"<<i<<");"<<endl;
        }

    }
    file.close();

}

void MatlabExporter::setSimulation(Simulation *value)
{
    simulation = value;
}
