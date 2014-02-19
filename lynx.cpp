
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

void LynxSSC::open_port() {
    this->fd = open("/dev/tty.usbmodemfa141", O_RDWR | O_NOCTTY | O_NDELAY);
    if (this->fd == -1) {
        cout << "Unable to open port. \n";
    } else {
        cout << "Port opened.\n";
    }
    cout << "Descriptor in open:";
    cout << this->fd;
    cout << "\n";
}

void LynxSSC::configure_port() {
    struct termios options;

    tcgetattr(this->fd, &options);
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);

    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cflag |= (CLOCAL | CREAD);
    tcsetattr(this->fd, TCSANOW, &options);

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
    strcat(cmd, "\n");

    write(this->fd, cmd, strlen(cmd));
}
