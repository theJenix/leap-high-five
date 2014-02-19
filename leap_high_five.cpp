
#include <iostream>
#include <Leap.h>

#include "lynx.h"
#include <unistd.h>

using namespace std;

struct Vector3 {
    double v[3];

    Vector3(double v1, double v2, double v3) {
        v[0] = v1;
        v[1] = v2;
        v[2] = v3;
    }
};

struct ArmAngles {
    double rotation;
    Vector3 q;
    double effector;

    ArmAngles(double rotation, double q1, double q2, double q3, double effector) 
        : rotation(rotation), q(q1, q2, q3), effector(effector) {}
};

struct WorldVector : public Vector3 {

    Vector3 transformLeapVector(Leap::Vector& leapVec, WorldVector leapPos) {
        //TODO: transform the leapVec into world coordinates, based on how far away
        //the leap sensor is
        return Vector3(0,0,0);
    }

    WorldVector(Leap::Vector& leapVec, WorldVector leapPos) : Vector3(transformLeapVector(leapVec, leapPos)) {
    }
    WorldVector(double v1, double v2, double v3) : Vector3(v1, v2, v3) {}
};
/*

struct Jacobian3 {
    Vector3 data[3];

    Jacobian3(const Vector3& q, const Vector3& ql) {
        double a1   = q.v[0],
               s1   = sin(a1),
               c1   = cos(a1),
               a12  = a1 + q.v[1],
               s12  = sin(a12),
               c12  = cos(a12),
               a123 = a12 + q.v[2],
               s123 = sin(a123),
               c123 = cos(a123);
        
        data[0] = Vector3(-ql.v[0]*s1 - ql.v[1]*s12 - ql.v[2]*s123, -ql.v[1]*s12 - ql.v[2]*s123, -ql.v[2]*s123);
        data[1] = Vector3( ql.v[0]*c1 + ql.v[1]*c12 + ql.v[2]*c123,  ql.v[1]*c12 + ql.v[2]*c123,  ql.v[2]*c123);
        data[2] = Vector3( 1,                                        1,                           1);
    }
};
*/
bool inMotion = false;

ArmAngles armInit(0.0, 0.0, 0.0, 0.0, 0.0);
ArmAngles armCurrent(0.0, 0.0, 0.0, 0.0, 0.0);
Vector3 armLengths(0.0, 0.0, 0.0);

WorldVector currentPosition(0.0, 0.0, 0.0);
WorldVector currentTarget(0.0, 0.0, 0.0);
WorldVector leapPos(0.0, 0.0, 0.0);

class FrameListener : public Leap::Listener {
    
    virtual void onFrame(const Leap::Controller& controller) {
        Leap::Frame frame = controller.frame();
        const Leap::HandList& hands = frame.hands();
        if (hands.count() > 0) {
            Leap::Vector newTarget = hands[0].stabilizedPalmPosition();
            cout << "New Target: " << newTarget << endl;
            if (inMotion) {
                //TODO: must merge with currentTarget, to account for if the
                //target hand moved...for now, do nothing
            } else {
                inMotion = true;
                currentTarget = WorldVector(newTarget, leapPos);
            }
        }
    }
};

int main(int argc, const char *argv[]) {

    if (argc < 2) {
        cout << "Usage: leap_high_five /path/to/serial\n";
        return 1;
    }

    LynxSSC lynx(argv[1], 115200);
//    lynx.move(3, 1500, -1, -1);
    lynx.move(3, 1500, -1, -1);
    sleep(2);
    lynx.move(4, 500, -1, 1000);
    sleep(2);
    lynx.move(
        LynxMoveGroup().move(3,1000,-1,500)
                       .move(4,1500,-1,1000));

return 0;
    cout << "Initializing controller";
    static Leap::Controller s_controller;
    FrameListener listener;
    cout << "Adding listener to controller";
    s_controller.addListener(listener);

    
    // while (running) {
    // 
    //     while (inMotion) {
    //         Jacobian3 J(armCurrent.q, armLengths); 
    //     }
    // }

    cin.get();
    return 0;
}
