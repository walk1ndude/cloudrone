#include <QtCore/QDir>
#include <QtCore/QTextStream>

#include "worker_node/stateset.h"

StateSet::StateSet(QObject * parent) : QObject(parent) {
  droneLauncher = new DroneLauncher(this);
}

StateSet::~StateSet() {

}

DroneLauncher * StateSet::getDroneLauncher() {
  return droneLauncher;
}

bool StateSet::addRule(const int & cstate, const int & nstate) {
  rules.insert(cstate, nstate);
}

bool StateSet::isPossible(const int & cstate, const int & nstate) {

  for(QMultiMap<int, int>::iterator it = rules.find(cstate); it != rules.end() && it.key() == cstate; it ++) {
    if (it.value() == nstate) {
      return true;
    }
  }
  return false;
}

bool StateSet::respond(cloudrone::SetState::Response & res, const int & error) {
  res.error = error;
  return true;
};

bool StateSet::setState(const cloudrone::SetState::Request & req, const QString & task, cloudrone::SetState::Response & res) {
  int cstate = req.state.state;
  int nstate = req.nstate;
  
  res.state = req.state;
  
  if (isPossible(cstate, nstate)) {
    
    res.state.state = nstate;
    
    if (task == "start") {
      return startTaskByID(req.state.id, QString::fromStdString(req.driver), res);
    }
    if (task == "finish") {
      return finishTaskByID(req.state.id, res);
    }
    
    return respond(res, EVERYTHINGS_FINE);
  }
  
  else {
    return (!respond(res, ERROR_CANT_SET_STATE));
  }
}

bool StateSet::startTaskByID(const int & id, const QString & driver, cloudrone::SetState::Response & res) {
  
  QString fileName = QString("%1/.ros/drone%2.launch").arg(QDir::homePath()).arg(QString::number(id));
  QFile file(fileName);
  
  // generate launch file
  
  if (file.open(QIODevice::WriteOnly)) {
    QTextStream stream(&file);
    
    QString drone = "drone" + QString::number(id);
    QString launchDriver;
    
    if (driver == ARDRONE_DRIVER) {
      launchDriver = "<node pkg=\"ardrone_autonomy\" type=\"" + driver
      + "\" name=\"ardrone_driver\" args=\"_navdata_demo:=0 _loop_rate:=200\" />";
    }
    else {
      launchDriver = "<node pkg=\"rosbag\" type=\"play\" name=\"player\" args = \"" + driver + "\" required=\"true\"/>";
    };
    
    //AS OF GROOVY (HYDRO): NEED TO REMAP EACH TOPIC MANUALLY!!!
    // TO DO: add all nodes and remaps to db
      
    stream << "<launch>" << endl
    << "<group ns=\"" + drone + "\">" << endl
    << "<remap from=\"/ardrone/image_raw\" to=\"/" + drone + "/ardrone/image_raw\" />" << endl
    << "<remap from=\"/ardrone/navdata\" to=\"/" + drone + "/ardrone/navdata\" />" << endl
    << "<remap from=\"/cloudrone/image_raw\" to=\"/" + drone + "/cloudrone/image_raw\" />" << endl
    << launchDriver << endl
    << "<node pkg=\"cloudrone\" type=\"marker_detection_node\" name=\"marker_detection_node\" />" << endl
    << "<node pkg=\"tum_ardrone\" type=\"drone_stateestimation\" name=\"drone_stateestimation\" />" << endl
    << "<node pkg=\"tum_ardrone\" type=\"drone_autopilot\" name=\"drone_autopilot\" />" << endl
    << "</group>" << endl
    << "</launch>" << endl;
    
    file.close();
    
    if (droneLauncher->startDrone(id, "roslaunch " + fileName)) {
      return respond(res, EVERYTHINGS_FINE);
    }
    else {
      return respond(res, ERROR_CANT_START_TASK);
    }
  }
  else {
    return respond(res, ERROR_CANT_WRITE_LAUNCH_FILE);
  }
}

bool StateSet::finishTaskByID(const int & id, cloudrone::SetState::Response & res) {
  if (droneLauncher->removeDrone(id)) {
    return respond(res, EVERYTHINGS_FINE);
  }
  else {
    return respond(res, ERROR_CANT_END_TASK);
  }
}