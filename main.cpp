#include "window.h" // include our window class header

int main() {
    // Create an instance of our Window class
    Window mainWindow(640, 480, "Hello Triangle");

    // Run our program
    if (mainWindow.initialize()) {
        mainWindow.loop();
    }

    return 0;
}
