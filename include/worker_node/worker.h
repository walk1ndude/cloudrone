#include <ros/ros.h>

#include <qt/QtSql/QSqlDatabase>

//=====================GET HEADERS CLOUDRONE======================//

#include "FSM.h"

#include <cloudrone/Auth.h>

#include <cloudrone/GetDrones.h>
#include <cloudrone/GetMarkers.h>

#include <cloudrone/SetState.h>

#include <cloudrone/KillNodes.h>

#include <worker_node/dronelauncher.h>

//=====================TOPICS==================================//

#define TOPIC_REG "reg"
#define TOPIC_SIGN "sign"

#define TOPIC_GET_INFO "get_info"
#define TOPIC_GET_DRONES "get_drones"
#define TOPIC_GET_MARKERS "get_markers"

#define TOPIC_SET_STATE "set_state"

#define TOPIC_LAUNCH_NODES "launch_nodes"
#define TOPIC_KILL_NODES "kill_nodes"

#define TOPIC_NOTIFY_KILL "notify_kill"

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

//=========================ERROR CODES=========================//

#define EVERYTHINGS_FINE 0
#define ERROR_DB_NOT_CONNECTED 1
#define ERROR_USER_ALREADY_REGISTERED 2
#define ERROR_CANT_REGISTER_USER 3
#define ERROR_NO_SUCH_USER 4
#define ERROR_CANT_SIGN_USER 5
#define ERROR_CANT_SHOW_DRONES 6
#define ERROR_CANT_SET_STATE 7
#define ERROR_CANT_WRITE_LAUNCH_FILE 8
#define ERROR_CANT_LAUNCH_NODES 9

//=============================================================//

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
  
  FSM fsm;
  
  DroneLauncher droneLauncher;
   
  ros::ServiceServer registerService;
  ros::ServiceServer signService;
  
  ros::ServiceServer getMarkerService;
  ros::ServiceServer getDronesService;
  
  ros::ServiceServer setStateService;
  
  ros::ServiceServer killNodesService;
  
  ros::Publisher killNotifier;
   
  template <class ResponseT> bool respond(ResponseT & res, int error);
  template <class ResponseT> bool checkDB(ResponseT & res);
  
  void setDefaults();
  void fetchServices();
  void fetchPublishers();
  void fetchFSM();
  bool fetchDatabase();
  
  void closeDatabase();
   
  bool registerUser(cloudrone::Auth::Request & req, cloudrone::Auth::Response & res);
  bool signUser(cloudrone::Auth::Request & req, cloudrone::Auth::Response & res);
  
  QString getDriver(int id);
  bool getMarkers(cloudrone::GetMarkers::Request & req, cloudrone::GetMarkers::Response & res);
  bool getDrones(cloudrone::GetDrones::Request & req, cloudrone::GetDrones::Response & res);
  
  bool setState(cloudrone::SetState::Request & req, cloudrone::SetState::Response & res);
  
  template <class ResponseT> bool killNodesByID(int id, ResponseT & res);
  template <class ResponseT> bool launchNodesByID(int id, ResponseT & res);
  
  bool killNodes(cloudrone::KillNodes::Request & req, cloudrone::KillNodes::Response & res);

private slots:
  void notifyKillID(int exitCode, QProcess::ExitStatus exitStatus);
};
