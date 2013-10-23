#include <ros/ros.h>

#include <qt/QtCore/QMap>
#include <qt/QtCore/QThread>

#include <cloudrone/Drone.h>
#include "worker_node/drone.h"

class DroneLauncher : public QObject {
  Q_OBJECT

public:
  explicit DroneLauncher(QObject * parent = 0);
  ~DroneLauncher();
  
  void addDrone(const int & id, const QString & program);
  
private:
  QMap<int,Drone*>drones;
  
private slots:
  void removeDrone(Drone * drone);
  
signals:
  void signalTaskCompleted(const int & id);
};