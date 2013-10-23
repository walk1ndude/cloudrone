#include <qt/QtCore/QObject>
#include <qt/QtCore/QMultiMap>

//=======================STATES=================================//

#define STATE_FREE 0
#define STATE_SELECTED 1
#define STATE_ONTASK 2
#define STATE_TASKCOMPLETED 3

//===============================================================//

class StateSet : public QObject {
  
public:
  explicit StateSet(QObject * parent = 0);
  ~StateSet();

  void fetchRules();
  bool isApplicable(int pstate, int nstate);

private:
  QMultiMap<int,int>rules;
 
};
