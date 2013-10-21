#include "worker_node/dronelauncher.h"

DroneLauncher::DroneLauncher(QObject* parent): QObject(parent) {

}

DroneLauncher::~DroneLauncher() {

}

void DroneLauncher::addDrone(const int& id, const QString& program) {
  
  DroneThread * drone = new DroneThread(program);
  drones.insert(id, drone);
  
  QProcess * process = drone->getProcess();
  
  QObject::connect(process, SIGNAL(finished(int)), this, SLOT(removeDrone(int)));
  //QObject::connect(drone, (void (DroneThread::*)())&DroneThread::finished, this, &DroneThread::deleteLater);
  
  drone->start();
}

void DroneLauncher::removeDrone(int exitCode) {
  
 /* QProcess * process = qobject_cast<QProcess*>(QObject::sender());
  DroneThread * drone = qobject_cast<DroneThread*>(process->thread());
  drones.remove(drones.key(drone));
  
  drone->deleteLater();
  */
  std::cout << "WIN" << std::endl;
}



