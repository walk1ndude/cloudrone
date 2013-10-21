#include <ros/ros.h>

#include <qt/QtCore/QMap>

#include <cloudrone/Drone.h>
#include "worker_node/dronethread.h"

class DroneLauncher : public QObject {
  Q_OBJECT

public:
  explicit DroneLauncher(QObject * parent = 0);
  ~DroneLauncher();
  
  void addDrone(const int & id, const QString & program);
  
private:
  QMap<int,DroneThread*>drones;
  
private slots:
  void removeDrone(int exitCode);
};