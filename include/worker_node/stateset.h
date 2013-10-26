#include <QtCore/QObject>
#include <QtCore/QMultiMap>

#include <cloudrone/SetState.h>

#include "worker_node/dronelauncher.h"

//=======================STATES=================================//

#define STATE_FREE 0
#define STATE_SELECTED 1
#define STATE_ONTASK 2
#define STATE_TASKCOMPLETED 3
#define STATE_TASKPAUSED 4

//========================DRIVERS==================================//

#define ARDRONE_DRIVER "ardrone_driver"

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
#define ERROR_CANT_START_TASK 10
#define ERROR_CANT_END_TASK 11

//==============================================================//

class StateSet : public QObject {
  
public:
  explicit StateSet(QObject * parent = 0);
  ~StateSet();

  bool addRule(const int & cstate, const int & nstate);
  DroneLauncher * getDroneLauncher();

  bool setState(const cloudrone::SetState::Request & req, cloudrone::SetState::Response & res);

private:
  QMultiMap<int,int>rules;
  DroneLauncher * droneLauncher;

  bool isPossible(const int & cstate, const int & nstate);
  
  static bool respond(cloudrone::SetState::Response & res, const int & error);
  
  bool startTaskByID(const int & id, const QString & driver, cloudrone::SetState::Response & res);
  bool finishTaskByID(const int & id, cloudrone::SetState::Response & res);
  bool pauseTaskByID(const int & id, cloudrone::SetState::Response & res);
  bool resumeTaskByID(const int & id, cloudrone::SetState::Response & res);
};
