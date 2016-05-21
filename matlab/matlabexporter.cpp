#include "matlabexporter.h"

MatlabExporter::MatlabExporter(QObject *parent) : QObject(parent)
{
simulation = nullptr;
}

void MatlabExporter::exportResults(QString* filename){

    if(!filename->contains((".m"))){
        filename->append(".m");
    }
    QFile file(*filename);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream<<"figure()"<<endl;
        stream<<"grid on;"<<endl;
        stream<<"hold on;"<<endl;
        stream<<"axis equal;"<<endl;
        stream<<endl<<"% Receivers positions"<<endl;
        std::vector<Receiver *> receivers = simulation->getReceivers();
        for(auto receiver = receivers.begin(); receiver != receivers.end();receiver++){
            stream<<"circle("<<(*receiver)->getPositionX()<<","<<(*receiver)->getPositionY()<<",1.5,'k');"<<endl;
        }

        stream<<endl<<"% Obstacle positions"<<endl;
        std::vector<Obstacle *> obstacles = simulation->getObstacles();
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
            stream<<"xlabel('time [us]');"<<endl<<"ylabel('signal amplitude');"<<endl;
        }

        stream<<endl<<"'--------------------------------'"<<endl;
        for(unsigned int i=0;i<receivers.size();i++){
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
        for(unsigned int i=0;i<receivers.size();i++){
            stream<<"plot(x"<<i<<",y"<<i<<");"<<endl;
        }

        stream<<"figure()"<<endl;
        stream<<"grid on;"<<endl;
        stream<<"hold on;"<<endl;
        std::vector<Point *> algResPlot = simulation->getAlgorithmResultsToPlot();
         for(auto point = algResPlot.begin(); point != algResPlot.end();point++){
             stream<<"plot("<<(*point)->getX()<<","<<(*point)->getY()<<",'r.','MarkerSize',12);"<<endl;
         }
         stream<<"x=[ ";
         for(auto point = algResPlot.begin(); point != algResPlot.end();point++){
             stream<<(*point)->getX()<<" ";
         }
         stream<<"];"<<endl;

         stream<<"y=[ ";
         for(auto point = algResPlot.begin(); point != algResPlot.end();point++){
             stream<<(*point)->getY()<<" ";
         }
         stream<<"];"<<endl;
         stream<<"plot(x,y);"<<endl;

    }
    file.close();

}

void MatlabExporter::setSimulation(Simulation *value)
{
    simulation = value;
}
