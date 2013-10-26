#include "worker_node/dronelauncher.h"
#include <signal.h>

DroneLauncher::DroneLauncher(QObject* parent): QObject(parent) {

}

DroneLauncher::~DroneLauncher() {

}

bool DroneLauncher::startDrone(const int & id, const QString & program) {
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
    sleep(1);
    process = drone->getProcess();
  }
  
  while (process->state() != QProcess::Running) {
    sleep(1);
  }
  
  drones[id]->setPlayPID(findPlayPID(process->pid()));
  
  return true;
}

void DroneLauncher::removeDrone(Drone * drone) {
  int id = drone->getID();
  drones.remove(id);
  
  delete drone;
  
  emit signalTaskCompleted(id);
}

int DroneLauncher::findPlayPID(const int & parentPID) {
  QProcess pidFinder;
 
  pidFinder.start(QString(
    "sh -c \"ps -o pid,command --ppid %1 | grep '/rosbag/play' | grep -Eo '^[0-9]*' \"").arg(QString::number(parentPID)));
  
  pidFinder.waitForFinished(-1);
  QString playStringPID = pidFinder.readAll();
  
  return QString(pidFinder.readAll()).toInt();
}

bool DroneLauncher::launchProcess(const int & id, const int & signal) {
  return (!kill(id, signal));
}

bool DroneLauncher::removeDrone(const int & id) {
  if (drones[id]->getProcess()) {
    int pid = drones[id]->getProcess()->pid();
    std::cout << pid << std::endl;
    return launchProcess(pid, SIGTERM);
  }
  else {
    return false;
  }
}

bool DroneLauncher::pauseDrone(const int & id) {
  if (drones[id]->getProcess()) {
    int playPID = drones[id]->getPlayPID();
    return launchProcess(playPID, SIGSTOP);
  }
  else {
    return false;
  }
}

bool DroneLauncher::resumeDrone(const int & id) {
  if (drones[id]->getProcess()) {
    int playPID = drones[id]->getPlayPID();
    return launchProcess(playPID, SIGCONT);
  }
  else {
    return false;
  }
}