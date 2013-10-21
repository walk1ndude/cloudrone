#include "worker_node/dronethread.h"
#include <iostream>

DroneThread::DroneThread(const QString& program, QObject * parent): QThread(parent) {
  this->program = program;
  process = new QProcess;
  process->moveToThread(this);
}

DroneThread::~DroneThread() {

}

QProcess* DroneThread::getProcess() {
  return process;
}

void DroneThread::run()
{
  process->setProcessChannelMode(QProcess::MergedChannels);
  process->start(program);
  
  process->waitForStarted();
  
  bool notFinished = false;
  
  while (notFinished) {
    notFinished = process->waitForFinished(10000);
    std::cout << notFinished << std::endl;
  }
}


