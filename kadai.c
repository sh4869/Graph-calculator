/* kadai.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <math.h>

#define BORDER 2
#define WIDTH  800
#define HIGHT 500

int cast(char *equ,int length);
int main(int argc,char **argv)
{
  Display *dpy;
  Window w,quit,input,buttons[16],draw,clear;
  Window root;
  int    screen,xPoint,yPoint,l = 0,count = 0,yoko = 0,tate = 0,equCount = 0,equFlag = 0,value,equLength;
  char formula[255],string[255];
  char buttonString[16] = {'1','2','3','+','4','5','6','-','7','8','9','*','0','x','=','/'};
  char equ[255];
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

  
  /*------------- Window Setting -------------------*/

  w = XCreateSimpleWindow(dpy, root, 100, 100, WIDTH, HIGHT, BORDER, black, white);
  quit = XCreateSimpleWindow(dpy,w,10,3,30,12,BORDER,black,white);
  input = XCreateSimpleWindow(dpy,w,510,10,260,480,BORDER,black,white);
  draw = XCreateSimpleWindow(dpy,w,520,30,40,20,BORDER,black,white);
  clear = XCreateSimpleWindow(dpy,w,580,30,40,20,BORDER,black,white);
  for(count = 0;count < 16;count++){
	buttons[count] = XCreateSimpleWindow(dpy,w,550+tate*50,200+yoko*80,30,30,BORDER,black,white);
	tate++;
	if((count+1) % 4 == 0){
	  yoko++;
	  tate = 0;
	}
  }

  gc = XCreateGC(dpy, w, 0, NULL);

  /* -------  Select Input --------------*/
  XSelectInput(dpy,w,ButtonPressMask | ButtonReleaseMask | KeyPressMask);
  XSelectInput(dpy,quit,ButtonPressMask);
  XSelectInput(dpy,input,ButtonPressMask);
  XSelectInput(dpy,draw,ButtonPressMask);
  XSelectInput(dpy,clear,ButtonPressMask);
  for(count = 0;count < 16;count++){
  	XSelectInput(dpy,buttons[count],ButtonPressMask);
  }

  XMapWindow(dpy, w);
  XMapSubwindows(dpy,w);
  while(1){
	//Event
	XNextEvent(dpy,&e);
	switch (e.type){
	  case KeyPress:
		printf("keycode=%d \n",e.xkey.keycode);
		if(e.xkey.keycode == 24) return 0;
		break;

	  case ButtonPress :
		printf("x=%d y=%d button=%d \n",e.xbutton.x,e.xbutton.y,e.xbutton.button);
		
		if(e.xany.window == quit){ 
		  printf("Exit!\n");
		  return 0;
		}
		if(e.xany.window == clear){
			printf("Clear!\n");
			XClearWindow(dpy,w);
			XClearWindow(dpy,input);
			l = 0;
			equCount = 0;
			equLength = strlen(equ);
			memset(equ,'\0',equLength);
		}
		if(e.xany.window == draw){
			printf("Draw!\n");
			switch(equFlag){
				case 0:
			    	value = atoi(equ);
					memset(equ,'\0',equLength);
					break;
				case 1:
					value = cast(equ,strlen(equ));
					printf("%d\n",cast(equ,strlen(equ)));
					break;
				default:
					break;
			}
			if(equFlag == 0 || equFlag == 1){
				XDrawLine(dpy,w,gc,10,250-value,490,250-value);
				XClearWindow(dpy,input);
				XDrawString(dpy,input,gc,160,140,equ,strlen(equ));
				l = 0;
				equCount = 0;
				equLength = strlen(equ);
				memset(equ,'\0',equLength);
			}

		}
		
		while(count < 16){
		  if(e.xany.window == buttons[count]){
		  	XDrawString(dpy,input,gc,35+l,80,&buttonString[count],1);
		  	l += 10;
		  	equ[equCount] = buttonString[count];
		  	equCount++;
		  	if((count+1) % 4 == 0){
		  		equFlag = 1;
		  	}
		  }
		  count++;
		}
		
		/*----------- Basic Drawing -------------*/
		
		XDrawString(dpy,quit,gc,4,10,"Exit",4);
		XDrawString(dpy,draw,gc,4,10,"Draw!",5);
		XDrawString(dpy,clear,gc,4,10,"Clear!",6);
		XDrawString(dpy,w,gc,491,250,"X",1);
		XDrawString(dpy,w,gc,250,8,"Y",1);
		XDrawString(dpy,input,gc,15,80,"Y =",3);
		XDrawString(dpy,input,gc,70,140,"Last Answer:",12);
		XDrawLine(dpy,w,gc,10,250,490,250);
		XDrawLine(dpy,w,gc,250,10,250,490);
		tate = yoko = 0;
		for(count = 0;count < 16;count++){
		  XDrawString(dpy,buttons[count],gc,13,20,&buttonString[count],1);
	      tate++;
	      if((count+1) % 4 == 0){
	        yoko++;
	  		tate = 0;
		  }
	    }
	    count = 0;
	    /*-----------------------------------------*/
		break;
	}
  }
}

int cast(char *equ,int length){
	int opeFlag = 0;
	int count = 0,valueCharCount = 0,valuesCount = 0,result = 0,ope = 0,oldOpe = 0;
	int values[10];
	char valuesChar[10];
	while(count < length){
		switch(equ[count]){
			case '+':
				values[valuesCount] = atoi(valuesChar);
				memset(valuesChar,'\0',valueCharCount);
				if(valueCharCount != 0){
					valuesCount++;
				}
				valueCharCount = 0;
				ope = 0;
				opeFlag = 1;
				break;
			case '-':
				values[valuesCount] = atoi(valuesChar);
				memset(valuesChar,'\0',valueCharCount);
				if(valueCharCount != 0){
					valuesCount++;
				}
				valueCharCount = 0;
				ope = 1;
				opeFlag = 1;
				break;
			case '*':
				values[valuesCount] = atoi(valuesChar);
				memset(valuesChar,'\0',valueCharCount);
				if(valueCharCount != 0){
					valuesCount++;
				}
				valueCharCount = 0;
				ope = 2;
				opeFlag = 1;
				break;
			case '/':
				values[valuesCount] = atoi(valuesChar);
				memset(valuesChar,'\0',valueCharCount);
				if(valueCharCount != 0){
					valuesCount++;
				}
				valueCharCount = 0;
				ope = 3;
				opeFlag = 1;
				break;
			default:
				valuesChar[valueCharCount] = equ[count];
				valueCharCount++;
				opeFlag = 0;	
				break;
		}
		count++;
		if(count == length){
			values[valuesCount] = atoi(valuesChar);
			memset(valuesChar,'\0',valueCharCount);
			if(valueCharCount != 0){
				valuesCount++;
			}
		}
		if(opeFlag || count == length){
			printf("Values Count - 1:  %d\n",values[valuesCount - 1]);
			if(valuesCount == 1){
				result = values[valuesCount - 1];
			}else if(valuesCount != 0){
				switch(oldOpe){
					case 0:
						result += values[valuesCount - 1];
						break;
					case 1:
						result -= values[valuesCount - 1];
						break;
					case 2:
						result *= values[valuesCount - 1];
						break;
					case 3:
						result /= values[valuesCount - 1];
						break;
				}
			}
			oldOpe = ope;
			printf("Result:%d\n",result);
		}
	}
	return result;
}