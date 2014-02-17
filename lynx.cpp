
#include <iostream>
#include <fcntl.h>   /* File control definitions */
#include "termios.h"
#include "lynx.h"

using namespace std;

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

void LynxSSC::move(int channel, int pos, int speed, int time {
    char cmd[256] = {0};
    char temp[32] = {0};
    
    sprintf(temp, "\#%d ", channel);
    strcat(cmd, temp);

    sprintf(temp, "P%d ", pos);
    strcat(cmd, temp);

    if (speed != -1) {
        sprintf(temp, "S%d ", speed);
        strcat(cmd, temp);
    }

    if (time != -1) {
        sprintf(temp, "T%d ", time);
        strcat(cmd, temp);
    }
}
