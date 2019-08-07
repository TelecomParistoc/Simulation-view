#include "ActionQueue.hpp"

ActionQueue::ActionQueue() {
    number_of_movements = 0;
}

char ActionQueue::getCurrentMovement() {
    if (number_of_movements == 0)
        return 'n';
    else
        return currentAction.movement;
}

void ActionQueue::pushMovement(const char& movement, const int& value) {
    if (number_of_movements == 0) {
        currentAction = {movement, value};
    } else {
        actions.push({movement, value});
    }
    number_of_movements ++;
    return;
}

int ActionQueue::getGoalValue() {
    return currentAction.value;
}

void ActionQueue::setGoalValue(const int& value) {
    currentAction.value = value;
    return;
}

void ActionQueue::popMovement () {
    number_of_movements -=1;
    if(number_of_movements == 0) {
        kill(ppid, SIGUSR1);
    } else {
        currentAction = actions.front();
        actions.pop();
    }
}

void ActionQueue::clear(){
    while(!actions.empty())
        actions.pop();
    number_of_movements = 0;
}

void ActionQueue::setPpid(const pid_t& ppid) {
    this->ppid = ppid;
}

