#include "worker_node/dronelauncher.h"

DroneLauncher::DroneLauncher(QObject* parent): QObject(parent) {

}

DroneLauncher::~DroneLauncher() {

}

void DroneLauncher::addDrone(const int & id, const QString & program) {
  
  QThread * droneThread = new QThread;
  Drone * drone = new Drone;
  
  QObject::connect(droneThread, &QThread::started, drone, &Drone::startTask);
  QObject::connect(drone, (void (Drone::*)(Drone*))&Drone::signalTaskFinished,
		   this, (void (DroneLauncher::*)(Drone*))&DroneLauncher::removeDrone, Qt::DirectConnection);
  QObject::connect(drone, &Drone::destroyed, droneThread, &QThread::quit);
  QObject::connect(droneThread, &QThread::finished, droneThread, &QThread::deleteLater);
  
  drone->setID(id);
  drone->setProgram(program);
  
  drone->moveToThread(droneThread);
  
  drones.insert(id, drone);
  
  droneThread->start();
  
  QProcess * process = NULL;
 
  while (process == NULL) {
    process = drone->getProcess();
    sleep(1);
  }
  
  while (process->state() != QProcess::Running) {
    sleep(1);
  }
}

void DroneLauncher::removeDrone(Drone * drone) {

  int id = drone->getID();
  drones.remove(id);
  
  delete drone;
  
  emit signalTaskCompleted(id);
}



