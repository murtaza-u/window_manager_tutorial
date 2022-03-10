#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

Display *dpy;
int screen;

struct Root {
    Window win;
    int x, y;
    unsigned int width, height;
    unsigned int layout;
} root;

void start();
void grab();
void loop();
void stop();
void die();

/* events */
/* all handlers must take in XEvent as an argument */
void maprequest(XEvent *event);


/* The event handlers are organized in an array and are accessed whenever a new
 * event has been fetched by the `loop` function.
 * This allows event dispatching in O(1) time.

 * All events in Xlib are define by a special unique integer
 * Example:

#define MapRequest		20
#define KeyPress		2
#define KeyRelease		3
#define ButtonPress		4
#define ButtonRelease	5
#define DestroyNotify	17

 */

void (*handle_events[LASTEvent])(XEvent *event) = {
    [MapRequest] = maprequest,
};

/* If the above syntax is unknow to you, please refer to this simple example:
 * https://gist.github.com/Murtaza-Udaipurwala/fea3611b7ddb8a378223364f9cf5c726
 */

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

    /* `screen` is the term used by many X applications including `xrandr`
     * Each of your monitor is equal to 1 `screen`
     * */

    /* returns the default screen(nothing but an int) */
    screen = DefaultScreen(dpy);

    /* root window
    (0,0)                (width, 0)
        |----------------|
        |                |
        |                |
        |                |
        |                |
        |                |
        |                |
        |----------------|
    (0, height)          (width, height)
    */

    root.win = XDefaultRootWindow(dpy);
    root.width = XDisplayWidth(dpy, screen);
    root.height = XDisplayHeight(dpy, screen);

    /* get MapRequest events */
    XSelectInput(dpy, root.win, SubstructureRedirectMask);
}

void loop() {
    /* XEvent is a generic X event structures */
    XEvent ev;

    /* `XNextEvent` is a blocking call.
     * This means this code blocks until an event occurs.
     * Incase of an internal error, XNextEvent returns NULL and the loop
     * terminates.
     */

    while (!XNextEvent(dpy, &ev)) {
        /* handle only those events for which we have handlers defined */
        if (handle_events[ev.type]) {
            handle_events[ev.type](&ev);
        }
    }
}

void grab() {

}

void stop() {
    XCloseDisplay(dpy);
}

/* fired whenever a new window needs to be mapped(shown) to the screen */
void maprequest(XEvent *event) {
    /* XMapRequestEvent is the MapRequest event structure
     * Below, we are getting the specialised MapRequest event structure from
     * the generic event structure
     */
    XMapRequestEvent *ev = &event -> xmaprequest;

    /* To fullscreen a window
     * - Move the origin of the window to (0, 0).
     * - Set width and height of that window equal to root.width and
     *   root.height respectively.
     */
    XMoveResizeWindow(dpy, ev -> window, 0, 0, root.width, root.height);

    /* map the window to the screen */
    XMapWindow(dpy, ev -> window);

    /* there is also the `XUnmapWindow` function which unmaps(hides) the window
     * from the `screen`
     */
}
