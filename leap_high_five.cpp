
#include <iostream>
#include <Leap.h>

using namespace std;

class FrameListener : public Leap::Listener {
    
    virtual void onFrame(const Leap::Controller& controller) {
        cout << "New Frame" << endl;
        Leap::Frame frame = controller.frame();
        const Leap::HandList& hands = frame.hands();
        for (size_t i = 0; i < hands.count(); i++) {
            cout << hands[i].stabilizedPalmPosition();
        }
        cout << endl;
    }
};


int main(int argc, const char *argv[]) {

    cout << "Initializing controller";
    static Leap::Controller s_controller;
    FrameListener listener;
    cout << "Adding listener to controller";
    s_controller.addListener(listener);


    cin.get();
}
