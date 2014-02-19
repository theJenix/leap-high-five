
#include <iostream>
#include <fcntl.h>   /* File control definitions */
#include "unistd.h"
#include "termios.h"
#include "lynx.h"

using namespace std;

LynxMoveGroup& LynxMoveGroup::move(int channel, int pos, int speed, int time) {
    LynxMovement mv = {channel, pos, speed, time};
    moves.push_back(mv);
    return *this;
}

LynxSSC::LynxSSC(const std::string& path, int baud) {
    open_port(path);
    configure_port(baud);
}

void LynxSSC::open_port(const std::string& path) {
    cout << "Opening " << path.data() << endl;
    this->fd = open(path.data(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (this->fd == -1) {
        cout << "Unable to open port. \n";
    } else {
        cout << "Port opened.\n";
    }
    cout << "Descriptor in open:";
    cout << this->fd;
    cout << "\n";
}

void LynxSSC::configure_port(int baud) {
    struct termios options;

    tcgetattr(this->fd, &options);
    cfsetispeed(&options, baud);
    cfsetospeed(&options, baud);

    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cflag |= (CLOCAL | CREAD);
    tcsetattr(this->fd, TCSANOW, &options);

    //send a command to establish a connection
    //NOTE: this appears to be required, or else the first move command is
    //ignored
    write(this->fd, "VER\r", 4);
    cout << "Port configured.\n";
}

// int LynxSCC::write_port()

void LynxSSC::move(int channel, int pos, int speed, int time) {
    LynxMoveGroup group;
    group.move(channel, pos, speed, time);
    move(group);
}

void LynxSSC::move(const LynxMoveGroup& group) {
    char cmd[256] = {0};
    char temp[32] = {0};
    
    for (std::vector<LynxMovement>::const_iterator it = group.moves.begin();
         it != group.moves.end();
         it++) {

        sprintf(temp, "#%d ", it->channel);
        strcat(cmd, temp);

        sprintf(temp, "P%d ", it->pos);
        strcat(cmd, temp);

        if (it->speed != -1) {
            sprintf(temp, "S%d ", it->speed);
            strcat(cmd, temp);
        }

        if (it->time != -1) {
            sprintf(temp, "T%d ", it->time);
            strcat(cmd, temp);
        }
    }
    strcat(cmd, "\r");
    cout << "Writing " << cmd << endl;
    write(this->fd, cmd, strlen(cmd));
    tcdrain(this->fd);
}
