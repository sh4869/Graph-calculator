/* kadai.c */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <math.h>

#define BORDER 2
#define WIDTH  500
#define HIGHT 500

int main(int argc,char **argv)
{
	Display *dpy;
	Window w,quit;
	Window root;
	int    screen,xPoint,yPoint;
	unsigned long black, white;
	GC       gc;
	XColor c0[2],c1[2];
	XEvent e;

	Colormap cmap;


	dpy = XOpenDisplay("");
	root = DefaultRootWindow (dpy);
	screen = DefaultScreen (dpy);
	white = WhitePixel (dpy, screen);
	black = BlackPixel (dpy, screen);
	cmap = DefaultColormap(dpy,0);
	XAllocNamedColor(dpy,cmap,"red",&c1[0],&c0[0]);
	XAllocNamedColor(dpy,cmap,"blue",&c1[1],&c0[1]);

	w = XCreateSimpleWindow(dpy, root, 100, 100, WIDTH, HIGHT, BORDER, black, white);
	quit = XCreateSimpleWindow(dpy,w,10,3,30,12,BORDER,black,white);
	gc = XCreateGC(dpy, w, 0, NULL);
	XSelectInput(dpy,w,ButtonPressMask | ButtonReleaseMask | KeyPressMask);
	XSelectInput(dpy,quit,ButtonPressMask);

	XMapWindow(dpy, w);
	XMapSubwindows(dpy,w);
	XDrawLine(dpy,w,gc,xPoint,yPoint,e.xbutton.x,e.xbutton.y);
	while(1){
		if(XEventsQueued(dpy,QueuedAfterReading)){
			XNextEvent(dpy,&e);
			switch (e.type){
				case KeyPress:
					printf("keycode=%d \n",e.xkey.keycode);
					if(e.xkey.keycode == 38){
						XSetForeground(dpy,gc,c1[0].pixel);
					}else if(e.xkey.keycode == 56){
						XSetForeground(dpy,gc,c1[1].pixel);
					}			
					break;
				case ButtonPress :
					printf("x=%d y=%d button=%d \n",e.xbutton.x,e.xbutton.y,e.xbutton.button);
					if(e.xany.window == quit){ 
                        printf("Exit!\n");
                        return;
                    }
                    XDrawString(dpy,quit,gc,4,10,"Exit",4);
					break;
			}
		}else{
            XDrawLine(dpy,w,gc,10,250,490,250);
            XDrawLine(dpy,w,gc,250,10,250,490);
		}
	}
}
