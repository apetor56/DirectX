#ifndef APP_HPP
#define APP_HPP

#include "Window.hpp"

class App {
public:
    App();

    int go();

private:
    Window window;
    void doFrame();
};

#endif // APP_HPP