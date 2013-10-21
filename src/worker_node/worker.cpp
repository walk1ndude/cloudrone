#include <iostream>
#include <qt/QtSql/QSqlQuery>

#include <qt/QtCore/QVariant>

#include <qt/QtCore/QFile>
#include <qt/QtCore/QIODevice>
#include <qt/QtCore/QTextStream>

#include <qt/QtCore/QThread>

#include <qt/QtCore/QDir>

#include "worker_node/worker.h"
#include <cloudrone/User.h>
#include <sstream>

Worker::Worker(QObject * parent) :
  QObject(parent),
  _nh("~") {
    
  setDefaults();
  
  if (fetchDatabase()) {
    fetchServices();
    fetchFSM();
  }
}

Worker::~Worker() {

}

void Worker::setDefaults() {
  _nh.param("database_driver", database.driver, std::string(DEFAULT_DATABASE_DRIVER));
  _nh.param("database_host", database.host, std::string(DEFAULT_DATABASE_HOST));
  _nh.param("database_name", database.name, std::string(DEFAULT_DATABASE_NAME));
  _nh.param("database_user", database.user, std::string(DEFAULT_DATABASE_USER));
  _nh.param("database_password", database.password, std::string(DEFAULT_DATABASE_PASSWORD));
}

bool Worker::fetchDatabase() {
  db = QSqlDatabase::addDatabase(QString::fromStdString(database.driver));
  db.setHostName(QString::fromStdString(database.host));
  db.setDatabaseName(QString::fromStdString(database.name));
  db.setUserName(QString::fromStdString(database.user));
  db.setPassword(QString::fromStdString(database.password));
  
  return (db.open()) ? true : false;
}

void Worker::closeDatabase() {
  db.close();
}

void Worker::fetchPublishers() {
  killNotifier = nh.advertise<cloudrone::Drone>(TOPIC_NOTIFY_KILL, 1);
}

void Worker::fetchServices() {
  registerService = nh.advertiseService(TOPIC_REG, &Worker::registerUser, this);
  signService = nh.advertiseService(TOPIC_SIGN, &Worker::signUser, this);

  getDronesService = nh.advertiseService(TOPIC_GET_DRONES, &Worker::getDrones, this);
  getMarkerService = nh.advertiseService(TOPIC_GET_MARKERS, &Worker::getMarkers, this);

  setStateService = nh.advertiseService(TOPIC_SET_STATE, &Worker::setState, this);

  killNodesService = nh.advertiseService(TOPIC_KILL_NODES, &Worker::killNodes, this);
}

template <class ResponseT> bool Worker::respond(ResponseT & res, int error) {
  res.error = error;
  return (error == EVERYTHINGS_FINE) ? true : false;
};

template <class ResponseT> bool Worker::checkDB(ResponseT & res) {
  if (!db.open()) {
    respond(res, ERROR_DB_NOT_CONNECTED);
  }
};

void Worker::fetchFSM() {
  fsm.fetchRules();
}

bool Worker::registerUser(cloudrone::Auth::Request & req, cloudrone::Auth::Response & res) {
  
  if (checkDB(res)) {
    return false;
  }
  
  QSqlQuery query;
  query.prepare("SELECT * FROM users WHERE id=:id");
  query.bindValue(":id", QString::fromStdString(req.user.id));
  
  if (!query.exec() || query.size()) {
    return respond(res, ERROR_USER_ALREADY_REGISTERED);
  }
  
  query.prepare("INSERT INTO users(id, password) VALUES(:id, md5(:password))");
  query.bindValue(":id", QString::fromStdString(req.user.id));
  query.bindValue(":password", QString::fromStdString(req.user.password));
  
  if (!query.exec()) {
    return respond(res, ERROR_CANT_REGISTER_USER); 
  }
  
  return respond(res, EVERYTHINGS_FINE);
}

bool Worker::signUser(cloudrone::Auth::Request & req, cloudrone::Auth::Response & res) {
  
  if (checkDB(res)) {
    return false;
  }
  
  QSqlQuery query;
  query.prepare("SELECT * FROM users WHERE id=:id AND (password=md5(:password) OR signed=true);");
  query.bindValue(":id", QString::fromStdString(req.user.id));
  query.bindValue(":password", QString::fromStdString(req.user.password));
  
  if (!query.exec() || !query.size()) {
    return respond(res, ERROR_NO_SUCH_USER);
  }
  
  query.prepare("UPDATE users SET signed=true WHERE id=:id;");
  query.bindValue(":id", QString::fromStdString(req.user.id));
  
  if (!query.exec()) {
    return respond(res, ERROR_CANT_SIGN_USER);
  }
  
  res.id = req.user.id;
  return respond(res, EVERYTHINGS_FINE);
}



bool Worker::getMarkers(cloudrone::GetMarkers::Request & req, cloudrone::GetMarkers::Response & res) {
  
  int even = (int)ros::Time::now().toSec() % 2;
  res.type = (even == 0) ? "vj" : "graph";
  res.name = "marker_name";
  
  respond(res, EVERYTHINGS_FINE);
}

bool Worker::getDrones(cloudrone::GetDrones::Request & req, cloudrone::GetDrones::Response & res) {
  
  if (checkDB(res)) {
    return false;
  }
  
  QSqlQuery query;
  
  switch (req.policy) {
    
    case SHOW_ALL :
      query.prepare("SELECT * FROM show_all;");
      break;
      
    case SHOW_FREE :
      query.prepare("SELECT * FROM show_all WHERE state=" + QString::number(STATE_FREE) + ";");
      break;
     
    case SHOW_USER :
      query.prepare("SELECT * FROM show_all WHERE user=:user");
      query.bindValue(":user", QString::fromStdString(req.user));
      break;
  }
  
  if (!query.exec()) {
    return respond(res, ERROR_CANT_SHOW_DRONES);
  }
  
  cloudrone::Drone drone;
 
  while (query.next()) {
    
    drone.id = query.value(0).toInt();
    drone.name = query.value(1).toString().toStdString();
    drone.model = query.value(2).toString().toStdString();
    drone.location = query.value(3).toString().toStdString();
    drone.driver = query.value(4).toString().toStdString();
    drone.map = query.value(5).toString().toStdString();
    drone.state = query.value(6).toInt();
    drone.user = query.value(7).toString().toStdString();
    
    res.drones.push_back(drone);
  }
  
  res.refresh = (req.policy == SHOW_ALL) ? true : false;
  
  return respond(res, EVERYTHINGS_FINE);
}

bool Worker::setState(cloudrone::SetState::Request & req, cloudrone::SetState::Response & res) {
  
  if (checkDB(res)) {
    return false;
  }
  
  QSqlQuery query;
  res.id = req.id;
  
  if (fsm.isApplicable(req.pstate, req.nstate)) {
    
    query.prepare("UPDATE drones SET state=:nstate WHERE id=:id;");
    query.bindValue(":id", req.id);
    query.bindValue(":nstate", req.nstate);
    
    res.sstate = req.nstate;
  
    if (!query.exec()) {
      return respond(res, ERROR_CANT_SET_STATE);
    }
    
    if (req.pstate == STATE_SELECTED && req.nstate == STATE_ONTASK) {
      return launchNodesByID(req.id, res);
    }
    else if (req.pstate == STATE_ONTASK && (req.nstate == STATE_FREE || req.nstate == STATE_SELECTED)) {
      return killNodesByID(req.id, res);
    }
    
    return respond(res, EVERYTHINGS_FINE);
  }

  else {
    res.sstate = req.pstate;
    return respond(res, ERROR_CANT_SET_STATE);
  }

}

QString Worker::getDriver(int id) {
  
  QSqlQuery query;
  
  query.prepare("SELECT driver FROM drones WHERE id=:id");
  query.bindValue(":id", id);
  query.exec();
  query.next();
  
  std::cout << query.value(0).toString().toStdString() << std::endl;
  
  return query.value(0).toString();
}

template <class ResponseT> bool Worker::killNodesByID(int id, ResponseT & res) {
  /*
  QProcess * procToKill = drones.find(id).value();
  procToKill->kill();
  
  procToKill->waitForFinished();
  
  //free resources
  delete procToKill;
  drones.remove(id);
 */
  return respond(res, EVERYTHINGS_FINE);
}

template <class ResponseT> bool Worker::launchNodesByID(int id, ResponseT & res) {
  
  QString fileName = QDir::homePath() + "/.ros/drone" + QString::number(id) + ".launch";
  QFile file(fileName);
  
  // generate launch file
  
  if (file.open(QIODevice::WriteOnly)) {
    QTextStream stream(&file);
    
    QString drone = "drone" + QString::number(id);
    QString launchDriver;
    
    QString driver = getDriver(id);
    
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
    //<< "<node pkg=\"tum_ardrone\" type=\"drone_stateestimation\" name=\"drone_stateestimation\" />" << endl
    << "<node pkg=\"tum_ardrone\" type=\"drone_autopilot\" name=\"drone_autopilot\" />" << endl
    << "</group>" << endl
    << "</launch>" << endl;
    
    file.close();
    
    droneLauncher.addDrone(id, "roslaunch " + fileName);
   
    return respond(res, EVERYTHINGS_FINE);
  }
  else {
    return respond(res, ERROR_CANT_WRITE_LAUNCH_FILE);
  }
}

void Worker::notifyKillID(int exitCode, QProcess::ExitStatus exitStatus) {
  
 /* QProcess * process = qobject_cast<QProcess*>(QObject::sender());
  int id = drones.key(process);
  
  std::cout << "HERRREEE" << std::endl;
  
  QThread * thread = process->thread();
  
  process->deleteLater();
  thread->deleteLater();
  drones.remove(id);
  
  /*
  cloudrone::Drone killedDrone;
  killedDrone.id = id;
  killNotifier.publish(killedDrone);
  */
}


bool Worker::killNodes (cloudrone::KillNodes::Request& req, cloudrone::KillNodes::Response& res) {

}
