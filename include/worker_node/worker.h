#include <ros/ros.h>

#include <QtSql/QSqlDatabase>

//=====================GET HEADERS CLOUDRONE======================//

#include <cloudrone/Auth.h>

#include <cloudrone/GetDrones.h>
#include <cloudrone/GetMarkers.h>

#include <cloudrone/SetState.h>

#include <cloudrone/KillNodes.h>

#include "worker_node/stateset.h"

//=====================TOPICS==================================//

#define TOPIC_REG "reg"
#define TOPIC_SIGN "sign"

#define TOPIC_GET_INFO "get_info"
#define TOPIC_GET_DRONES "get_drones"
#define TOPIC_GET_MARKERS "get_markers"

#define TOPIC_SET_STATE "set_state"

#define TOPIC_LAUNCH_NODES "launch_nodes"
#define TOPIC_KILL_NODES "kill_nodes"

#define TOPIC_GET_STATE "get_state"

//=======================VIEW POLICY============================//

#define SHOW_ALL 0
#define SHOW_USER 1
#define SHOW_FREE 2

//=======================DRIVERS================================//

#define ARDRONE_DRIVER "ardrone_driver"

//=======================DB STUFF===============================//

#define DEFAULT_DATABASE_DRIVER "QMYSQL"
#define DEFAULT_DATABASE_HOST "localhost"
#define DEFAULT_DATABASE_NAME "cloudronedb"
#define DEFAULT_DATABASE_USER "cloudrone"
#define DEFAULT_DATABASE_PASSWORD "ardrone"

typedef struct _dbstruct {
  std::string driver;
  std::string host;
  std::string name;
  std::string user;
  std::string password;
}dbstruct;

//============================================================//

class Worker : public QObject {
  
  Q_OBJECT

public:
  explicit Worker(QObject * parent = 0);
  ~Worker();

private:
  ros::NodeHandle nh;
  ros::NodeHandle _nh;
  
  dbstruct database;
  
  QSqlDatabase db;
  
  StateSet * stateSet;
   
  ros::ServiceServer registerService;
  ros::ServiceServer signService;
  ros::ServiceServer getMarkerService;
  ros::ServiceServer getDronesService;
  ros::ServiceServer setStateService;
  ros::ServiceServer killNodesService;
  
  ros::Publisher stateChangePublisher;
  
  template <class ResponseT> bool respond(ResponseT & res, const int & error);
  template <class ResponseT> bool checkDB(ResponseT & res);
  
  void setDefaults();
  void fetchServices();
  void fetchPublishers();
  void fetchStateSet();
  void fetchSignals();
  bool fetchDatabase();
  
  void closeDatabase();
  
  bool isIDSigned(std::string id);
  
  bool registerUser(cloudrone::Auth::Request & req, cloudrone::Auth::Response & res);
  bool signUser(cloudrone::Auth::Request & req, cloudrone::Auth::Response & res);
  
  bool getMarkers(cloudrone::GetMarkers::Request & req, cloudrone::GetMarkers::Response & res);
  bool getDrones(cloudrone::GetDrones::Request & req, cloudrone::GetDrones::Response & res);
  
  bool setState(cloudrone::SetState::Request & req, cloudrone::SetState::Response & res);
  
  void notifyStateChanged(const int & id, const int & nstate);
  
  bool ownDrone(cloudrone::SetState::Request & req, cloudrone::SetState::Response & res);
  bool disownDrone(cloudrone::SetState::Request & req, cloudrone::SetState::Response & res);
  
private slots:
  void notifyTaskCompleted(const int & id);
  void notifyTaskPaused(const int & id);
  void notifyTaskResumed(const int & id);
};