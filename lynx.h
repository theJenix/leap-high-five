
#ifndef __LYNX_H
#define __LYNX_H

#include <string>
#include <vector>

struct LynxMovement {
    int channel;
    int pos;
    int speed;
    int time;
}

class LynxMoveGroup {
    std::vector<LynxMovement> moves;
    LynxMoveGroup& move(int channel, int pos, int speed, int time);
    void send();
}

class LynxSSC {

    int fd;
    
    void configure_port();
    void open_port();
    
public:
    LynxSSC(string& port);
    void move(int channel, int pos, int speed, int time);
    LynxMoveGroup startGroup();

    
};

#endif //__LYNX_H
