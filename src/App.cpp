#include "App.hpp"

App::App() : window(800, 600, "ja nie komar") {}

// event while loop
int App::go() {
    while(true) {
        if(const auto ecode = Window::processMessages()) {
            return *ecode;
        }

        doFrame();
    }
    
}

void App::doFrame() {
    float rand = (std::sin(window.timer.peek()) + 1) / 2;
    window.gfx().clearBuffer(rand, rand / 2, rand / 8);
    window.gfx().endFrame();
}