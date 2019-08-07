#ifndef ACTIONQUEUE_H
#define ACTIONQUEUE_H
#include <queue>
#include <csignal>

using namespace std;

struct actionElement {
    char movement;
    int value;
};

class ActionQueue {
    private:

    int number_of_movements;
    queue<actionElement> actions;
    actionElement currentAction;
    pid_t ppid;

    public:

    ActionQueue();
    char getCurrentMovement();
    void pushMovement(const char& movement, const int& value);
    int getGoalValue();
    void setGoalValue(const int& value);
    void popMovement ();
    void clear();
    void setPpid(const pid_t& ppid);
};
#endif
