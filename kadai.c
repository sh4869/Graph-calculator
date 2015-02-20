/* kadai.c */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <math.h>

#define BORDER 2
#define WIDTH  800
#define HIGHT 500

int main(int argc,char **argv)
{
  Display *dpy;
  Window w,quit,input,buttons[16];
  Window root;
  int    screen,xPoint,yPoint,l = 0,count = 0,yoko = 0,tate = 0;
  char formula[255],string[255];
  char buttonString[2][16] = {'1','2','3','+','4','5','6','-','7','8','9','*','0','x','=','/'};
  unsigned long black, white;
  GC       gc;
  KeySym key;
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
  input = XCreateSimpleWindow(dpy,w,510,10,260,480,BORDER,black,white);
  for(count = 0;count < 16;count++){
	buttons[count] = XCreateSimpleWindow(dpy,w,550+tate*50,200+yoko*80,30,30,BORDER,black,white);
	tate++;
	if((count+1) % 4 == 0){
	  yoko++;
	  tate = 0;
	}
  }

  gc = XCreateGC(dpy, w, 0, NULL);
  XSelectInput(dpy,w,ButtonPressMask | ButtonReleaseMask | KeyPressMask);
  XSelectInput(dpy,quit,ButtonPressMask);
  XSelectInput(dpy,input,ButtonPressMask);

  XMapWindow(dpy, w);
  XMapSubwindows(dpy,w);
  while(1){
	//Event
	XNextEvent(dpy,&e);
	switch (e.type){
	  case KeyPress:
		printf("keycode=%d \n",e.xkey.keycode);
		if(input && XLookupString(&e,string,9,&key,NULL) == 1){
		  formula[0] = string[0];
		  formula[1] = '\0';
		  XDrawString(dpy,input,gc,4+l,10,formula,strlen(formula));
		  l += 10;
		}
		if(e.xkey.keycode == 24) return;
		break;
	  case ButtonPress :
		printf("x=%d y=%d button=%d \n",e.xbutton.x,e.xbutton.y,e.xbutton.button);
		if(e.xany.window == quit){ 
		  printf("Exit!\n");
		  return;
		}
		XDrawString(dpy,quit,gc,4,10,"Exit",4);
		XDrawString(dpy,w,gc,491,250,"X",1);
		XDrawString(dpy,w,gc,250,8,"Y",1);
		XDrawLine(dpy,w,gc,10,250,490,250);
		XDrawLine(dpy,w,gc,250,10,250,490);
		break;
	}
  }
}
