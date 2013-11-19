#include <QtCore/QTimer>
#include <unistd.h>

#include "worker_node/drone.h"

Drone::Drone(QObject * parent): QObject(parent) {
  process = NULL;
}

Drone::~Drone() {
  delete process;
}

QProcess* Drone::getProcess() {
  return process;
}

int Drone::getID() {
  return id;
}

int Drone::getPlayPID() {
  return playPID;
}

QVector<std::string> Drone::getFlightTask() {
  return flightTask;
}

void Drone::setID(const int & id) {
  this->id = id;
}

void Drone::setPlayPID(const int & playPID) {
  this->playPID = playPID;
}

void Drone::setFlightTask(const QVector<std::string> & flightTask) {
  this->flightTask = flightTask;
}

void Drone::setProgram(const QString & program) {
  this->program = program;
}

void Drone::startTask() {
  
  process = new QProcess;
  process->setProcessChannelMode(QProcess::MergedChannels);
  process->start(program);
  
  process->waitForStarted();
  
  sleep(10);
  publishTum();
  
  
  process->waitForFinished(-1);
  
  emit signalTaskFinished(this);
}

void Drone::publishTum() {
  emit signalPublishTum(id);
}


