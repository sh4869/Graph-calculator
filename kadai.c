/* xdraw.c */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define BORDER 2
#define WIDTH  350
#define HIGHT 250

int main(int argc,char **argv)
{
	Display *dpy;
	Window w;
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
	XMapWindow(dpy, w);
	while(1){
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
		 if(e.xbutton.x < 10){
		     if(e.xbutton.y < 10) XSetForeground(dpy,gc,c1[0].pixel);
		     else if(e.xbutton.y < 20) XSetForeground(dpy,gc,c1[1].pixel);
		 }
		 xPoint = e.xbutton.x;
		 yPoint = e.xbutton.y;
		 break;
	      case ButtonRelease :
		 XDrawLine(dpy,w,gc,xPoint,yPoint,e.xbutton.x,e.xbutton.y);
	   }
	}
}
