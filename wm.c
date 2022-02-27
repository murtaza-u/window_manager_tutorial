#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

Display *dpy;

void start();
void grab();
void loop();
void stop();
void die();

int main() {
    start();
    grab();
    loop();
    stop();
    return 0;
}

void die(char *exit_msg) {
    fprintf(stderr, "%s\n", exit_msg); /* write a debug MSG before exiting */
    exit(EXIT_FAILURE);
}

void start() {
    if (!(dpy = XOpenDisplay(0))) /* this will get more clear in the future */
        die("failed to open display");
}

void loop() {
    while(1);
}

void grab() {

}

void stop() {
    XCloseDisplay(dpy);
}
