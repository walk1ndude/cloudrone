#include "worker_node/FSM.h"

FSM::FSM(QObject * parent) :
  QObject(parent) {
    
}

FSM::~FSM() {

}

void FSM::fetchRules(){
  rules.insert(STATE_FREE, STATE_SELECTED);
  rules.insert(STATE_SELECTED, STATE_ONTASK);
  rules.insert(STATE_SELECTED, STATE_FREE);
  rules.insert(STATE_ONTASK, STATE_FREE);
}

bool FSM::isApplicable(int pstate, int nstate) {

  for(QMultiMap<int, int>::iterator it = rules.find(pstate); it != rules.end() && it.key() == pstate; it ++) {
    if (it.value() == nstate) {
      return true;
    }
  }
  return false;
}
