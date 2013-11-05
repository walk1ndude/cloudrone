#include <iostream>
#include <QtSql/QSqlQuery>

#include <QtCore/QVariant>

#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QTextStream>

#include <QtCore/QThread>

#include <QtCore/QDir>

#include "worker_node/worker.h"
#include <cloudrone/User.h>

Worker::Worker(QObject * parent) :
  QObject(parent),
  _nh("~") {
    
  setDefaults();
  
  if (fetchDatabase()) {
    fetchServices();
    fetchPublishers();
    fetchStateSet();
    fetchSignals();
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
  stateChangePublisher = nh.advertise<cloudrone::State>(TOPIC_GET_STATE, 1);
}

void Worker::fetchServices() {
  registerService = nh.advertiseService(TOPIC_REG, &Worker::registerUser, this);
  signService = nh.advertiseService(TOPIC_SIGN, &Worker::signUser, this);

  getDronesService = nh.advertiseService(TOPIC_GET_DRONES, &Worker::getDrones, this);
  getMarkerService = nh.advertiseService(TOPIC_GET_MARKERS, &Worker::getMarkers, this);

  setStateService = nh.advertiseService(TOPIC_SET_STATE, &Worker::setState, this);

  //killNodesService = nh.advertiseService(TOPIC_KILL_NODES, &Worker::killNodes, this);
}

void Worker::fetchSignals() {
  QObject::connect(stateSet->getDroneLauncher(), &DroneLauncher::signalTaskCompleted,
		   this, &Worker::notifyTaskCompleted, Qt::DirectConnection);
}

void Worker::fetchStateSet() {
  stateSet = new StateSet(this);
  
  QSqlQuery query("SELECT * FROM state_set;");
  
  while (query.next()) {
    stateSet->addRule(query.value(0).toInt(),query.value(1).toInt());
  }
}

template <class ResponseT> bool Worker::respond(ResponseT & res, const int & error) {
  res.error = error;
  return true;
}

template <class ResponseT> bool Worker::checkDB(ResponseT & res) {
  if (!db.open()) {
    respond(res, ERROR_DB_NOT_CONNECTED);
  }
}

bool Worker::isIDSigned(std::string id) {
  
  QSqlQuery query;
  
  query.prepare("SELECT signed FROM users WHERE id=:id");
  query.bindValue(":id", QString::fromStdString(id));
  query.exec();
  query.next();
  
  if (query.size() == 0) {
    return false;
  }
  else {
    return query.value(0).toBool();
  }
}


bool Worker::registerUser(cloudrone::Auth::Request & req, cloudrone::Auth::Response & res) {
  
  if (checkDB(res)) {
    return true;
  }
  
  if (!isIDSigned(req.user.id)) {
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE id=:id;");
    query.bindValue(":id", QString::fromStdString(req.user.id));

    if (!query.exec() || query.size()) {
      return respond(res, ERROR_USER_ALREADY_REGISTERED);
    }

    query.prepare("INSERT INTO users(id, password) VALUES(:id, md5(:password));");
    query.bindValue(":id", QString::fromStdString(req.user.id));
    query.bindValue(":password", QString::fromStdString(req.user.password));

    if (!query.exec()) {
      return respond(res, ERROR_CANT_REGISTER_USER); 
    }
    return respond(res, EVERYTHINGS_FINE);
  }
  else {
    return respond(res, ERROR_USER_ALREADY_SIGNED_ON);
  }
}

bool Worker::signUser(cloudrone::Auth::Request & req, cloudrone::Auth::Response & res) {
  
  if (checkDB(res)) {
    return true;
  }
  
  QSqlQuery query;
  query.prepare("SELECT signUser(:id, :password, :isPageUpdate);");
  query.bindValue(":id", QString::fromStdString(req.user.id));
  query.bindValue(":password", QString::fromStdString(req.user.password));
  query.bindValue(":isPageUpdate", req.isPageUpdate);
  
  query.exec();
  query.next();
  
  // for now 0 - no such user, 1 - signed, 2 - signed off
  int returnCode = query.value(0).toInt();
  
  switch (returnCode) {
    
    case 0 :
      return respond(res, ERROR_CANT_SIGN_USER);
    
    case 1 :
      res.id = req.user.id;
      return respond(res, EVERYTHINGS_FINE);
      
    case 2 :
      return respond(res, ERROR_USER_ALREADY_SIGNED_OFF);
      
  }
}

bool Worker::getMarkers(cloudrone::GetMarkers::Request & req, cloudrone::GetMarkers::Response & res) {
  
  int even = (int)ros::Time::now().toSec() % 2;
  res.type = (even == 0) ? "vj" : "graph";
  res.name = "marker_name";
  
  respond(res, EVERYTHINGS_FINE);
}

bool Worker::getDrones(cloudrone::GetDrones::Request & req, cloudrone::GetDrones::Response & res) {
  
  if (checkDB(res)) {
    return true;
  }
  
  if (isIDSigned(req.user)) {
  
    QSqlQuery query;
    
    switch (req.policy) {
      
      case SHOW_ALL :
	query.prepare("SELECT * FROM show_all ORDER BY (id);");
	break;
	
      case SHOW_FREE :
	query.prepare("SELECT * FROM show_all WHERE state=" + QString::number(STATE_FREE) + " ORDER BY (id);");
	break;
      
      case SHOW_USER :
	query.prepare("SELECT * FROM show_all WHERE user=:user ORDER BY (id);");
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
  else {
    return respond(res, ERROR_USER_ALREADY_SIGNED_OFF);
  }
}

bool Worker::setState(cloudrone::SetState::Request & req, cloudrone::SetState::Response & res) {
  
  if (checkDB(res)) {
    return true;
  }
  
  if (isIDSigned(req.user)) {
  
  QSqlQuery query;
  
  query.prepare("SELECT task FROM state_set WHERE cstate=:cstate AND nstate=:nstate;");
  query.bindValue(":cstate", req.state.state);
  query.bindValue(":nstate", req.nstate);
  
  query.exec();
  query.next();
  
  QString task;
  
  try {
    task = query.value(0).toString();
  }
  catch (...) {
    task = "";
  }
  
  if (stateSet->setState(req, task, res)) {
    
    query.prepare("UPDATE drones SET state=:nstate WHERE id=:id;");
    query.bindValue(":id", req.state.id);
    query.bindValue(":nstate", req.nstate);
    
    query.exec();
    
    int cstate = req.state.state;
    int nstate = req.nstate;
    
    if (cstate == STATE_FREE && nstate == STATE_SELECTED) {
      return ownDrone(req, res);
    }
    
    if (nstate == STATE_FREE) {
      return disownDrone(req, res);
    }
  }
  
  return respond(res, EVERYTHINGS_FINE);
  }
  else {
    return respond(res, ERROR_USER_ALREADY_SIGNED_OFF);
  }
}

bool Worker::ownDrone(cloudrone::SetState::Request & req, cloudrone::SetState::Response & res) {
  
  if (checkDB(res)) {
    return true;
  }
  
  if (isIDSigned(req.user)) {
    
    QSqlQuery query;
    
    query.prepare("INSERT INTO drone_ownership(user,drone) VALUES(:user,:drone);");
    query.bindValue(":user", QString::fromStdString(req.user));
    query.bindValue(":drone", req.state.id);
    
    query.exec();
    
    return respond(res, EVERYTHINGS_FINE);
  }
  else {
    return respond(res, ERROR_USER_ALREADY_SIGNED_OFF);
  }
}

bool Worker::disownDrone(cloudrone::SetState::Request & req, cloudrone::SetState::Response & res) {
  
  if (checkDB(res)) {
    return true;
  }
  
  if (isIDSigned(req.user)) {
    
    QSqlQuery query;
    query.prepare("DELETE FROM drone_ownership WHERE user=:user AND drone=:drone");
    query.bindValue(":user", QString::fromStdString(req.user));
    query.bindValue(":drone", req.state.id);
  
    query.exec();
    
    return respond(res, EVERYTHINGS_FINE);
  }
  else {
    return respond(res, ERROR_USER_ALREADY_SIGNED_OFF);
  }
}

void Worker::notifyStateChanged(const int & id, const int & nstate) {
  cloudrone::State newstate;
  newstate.id = id;
  newstate.state = nstate;
  
  QSqlQuery query;
  query.prepare("UPDATE drones SET state=:nstate WHERE id=:id;");
  query.bindValue(":nstate", nstate);
  query.bindValue(":id", id);
  
  query.exec();
  
  stateChangePublisher.publish(newstate);
}

void Worker::notifyTaskCompleted(const int & id) {
  notifyStateChanged(id, STATE_TASKCOMPLETED);
}

void Worker::notifyTaskPaused(const int & id) {
  notifyStateChanged(id, STATE_TASKPAUSED);
}

void Worker::notifyTaskResumed(const int & id) {
  notifyStateChanged(id, STATE_ONTASK);
}

