#include "worker_node/stateset.h"

StateSet::StateSet(QObject * parent) : QObject(parent) {
  
}

StateSet::~StateSet() {

}

void StateSet::fetchRules(){
  rules.insert(STATE_FREE, STATE_SELECTED);
  rules.insert(STATE_SELECTED, STATE_ONTASK);
  rules.insert(STATE_SELECTED, STATE_FREE);
  rules.insert(STATE_ONTASK, STATE_FREE);
  rules.insert(STATE_ONTASK, STATE_TASKCOMPLETED);
  rules.insert(STATE_TASKCOMPLETED, STATE_FREE);
  rules.insert(STATE_TASKCOMPLETED, STATE_SELECTED);
}

bool StateSet::isApplicable(int pstate, int nstate) {

  for(QMultiMap<int, int>::iterator it = rules.find(pstate); it != rules.end() && it.key() == pstate; it ++) {
    if (it.value() == nstate) {
      return true;
    }
  }
  return false;
}
