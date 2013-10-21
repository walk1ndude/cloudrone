#include <qt/QtCore/QObject>
#include <qt/QtCore/QMultiMap>

//=======================STATES=================================//

#define STATE_FREE 0
#define STATE_SELECTED 1
#define STATE_ONTASK 2

//===============================================================//

class FSM : public QObject {
  
public:
  explicit FSM(QObject * parent = 0);
  ~FSM();

  void fetchRules();
  bool isApplicable(int pstate, int nstate);

private:
  QMultiMap<int,int>rules;
 
};
