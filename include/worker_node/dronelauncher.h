#include <ros/ros.h>

#include <QtCore/QMap>
#include <QtCore/QThread>

#include <cloudrone/Drone.h>
#include "worker_node/drone.h"

class DroneLauncher : public QObject {
  Q_OBJECT

public:
  explicit DroneLauncher(QObject * parent = 0);
  ~DroneLauncher();
  
  bool startDrone(const int & id, const QString & program);
  bool pauseDrone(const int & id);
  bool resumeDrone(const int & id);
  bool removeDrone(const int & id);
  
private:
  QMap<int,Drone*>drones;
  int findPlayPID(const int & parentPID);
  bool launchProcess(const int & id, const int & signal);
  
private slots:
  void removeDrone(Drone * drone);
  void publishTum(const int & id);
  
signals:
  void signalTaskCompleted(const int & id);
  void signalPublishTum(const int & id);
};