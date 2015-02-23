/* kadai.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <math.h>

#define BORDER 2
#define WIDTH  1000
#define HIGHT 500

float cast(char *equ,int length);
int main(int argc,char **argv)
{
	Display *dpy;
	Window w,quit,input,buttons[16],draw,clear;
	Window sinW,cosW,tanW;
	Window graph_list;
	Window root;
	int    screen,xPoint,yPoint,l = 0,count = 0,yoko = 0,tate = 0,equCount = 0,equFlag = 0,value,equLength;
	float coef[10];
	int coefCount = 0;
	int mustOpe = 0,forceDraw = 0;
	int colorCount = 0;
	int coefX,coefY;
	// For Graph List
	int graph_number = 0,graph_draw = 0,graph_equ_length = 0;
	char graph_equ[100];

	char formula[255],string[255];
	char buttonString[16] = {'1','2','3','+','4','5','6','-','7','8','9','*','0','x','=','/'};
	char equ[255];
	char text[100];
	char color_name[9][15] = {"red","green","blue","yellow","magenta","cyan","SeaGreen","PowderBlue","black"};
	unsigned long black, white;
	GC       gc;
	KeySym key;
	XColor c0[9],c1[9];
	XEvent e;
	Colormap cmap;

	dpy = XOpenDisplay("");
	root = DefaultRootWindow (dpy);
	screen = DefaultScreen (dpy);
	white = WhitePixel (dpy, screen);
	black = BlackPixel (dpy, screen);
	cmap = DefaultColormap(dpy,0);
	for(count = 0;count < 9;count++)
		XAllocNamedColor(dpy,cmap,&color_name[count][0],&c1[count],&c0[count]); 


	/*------------- Window Setting -------------------*/

	w = XCreateSimpleWindow(dpy, root, 100, 100, WIDTH, HIGHT, BORDER, black, white);
	quit = XCreateSimpleWindow(dpy,w,10,3,30,12,BORDER,black,white);
	input = XCreateSimpleWindow(dpy,w,510,10,260,480,BORDER,black,white);
	draw = XCreateSimpleWindow(dpy,w,520,30,40,20,BORDER,black,white);
	clear = XCreateSimpleWindow(dpy,w,580,30,40,20,BORDER,black,white);
	sinW = XCreateSimpleWindow(dpy,w,630,30,20,10,BORDER,black,white);
	cosW = XCreateSimpleWindow(dpy,w,660,30,20,10,BORDER,black,white);
	tanW = XCreateSimpleWindow(dpy,w,690,30,20,10,BORDER,black,white);
	graph_list = XCreateSimpleWindow(dpy,w,800,10,170,480,BORDER,black,white);
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
	XSelectInput(dpy,sinW,ButtonPressMask);
	XSelectInput(dpy,cosW,ButtonPressMask);
	XSelectInput(dpy,tanW,ButtonPressMask);
	for(count = 0;count < 16;count++){
		XSelectInput(dpy,buttons[count],ButtonPressMask);
	}

	for(count = 0;count < 10;count++) coef[count] = 0;
	count = 0;
	XMapWindow(dpy, w);
	XMapSubwindows(dpy,w);

	while(1){
		//Event
		XNextEvent(dpy,&e);
		switch (e.type){
			case KeyPress:
				printf("keycode=%d \n",e.xkey.keycode);
				if(e.xkey.keycode == 24) return 0;

			case ButtonPress :
				while(count < 16){
					if(e.xany.window == buttons[count]){
						if(mustOpe == 1 && (count+1) % 4 != 0){
							forceDraw = 1;
							printf("Force!\n");
							break;
						}else{
							mustOpe = 0;
						}
						XDrawString(dpy,input,gc,35+l,80,&buttonString[count],1);
						l += 10;
						equ[equCount] = buttonString[count];
						equCount++;
						if((count+1) % 4 == 0){
							equFlag = 1;
						}
						if(count == 13){
							coef[coefCount] = cast(equ,strlen(equ));
							equFlag = 2;
							forceDraw = 1;
						}else if(count == 14){
							forceDraw = 1;
						}
					}
					count++;
				}
				if(e.xany.window == sinW || e.xany.window == cosW || e.xany.window == tanW){
					if(e.xany.window == sinW){
						XSetForeground(dpy,gc,c1[colorCount].pixel);
						colorCount++;
						graph_draw = 1;
						for(count = 0;count < 480;count++){
							XDrawPoint(dpy,w,gc,(count+10),-50*sin((count-240)/50.0) + 250);
						}
						printf("Sin\n");
						sprintf(graph_equ,"%d : Y = sin(x)",graph_number + 1);
					}else if(e.xany.window == cosW){
						XSetForeground(dpy,gc,c1[colorCount].pixel);
						colorCount++;
						graph_draw = 1;
						for(count = 0;count < 480;count++){
							XDrawPoint(dpy,w,gc,(count+10),-100*cos((count-240)/100.0) + 250);
						}
						sprintf(graph_equ,"%d : Y = cos(x)",graph_number + 1);
					}else if(e.xany.window == tanW){
						XSetForeground(dpy,gc,c1[colorCount].pixel);
						colorCount++;
						graph_draw = 1;
						for(count = 0;count < 480;count++){
							XDrawPoint(dpy,w,gc,(count+10),-100*tan((count-240)/100.0) + 250);
						}  
						sprintf(graph_equ,"%d : Y = tan(x)",graph_number + 1);
					}
					XDrawString(dpy,graph_list,gc,10,(graph_number+1)*10,graph_equ,strlen(graph_equ));
					graph_number++;
				}

				XSetForeground(dpy,gc,c1[8].pixel);

				if(e.xany.window == quit){ 
					printf("Exit!\n");
					return 0;
				}
				if(e.xany.window == clear){
					printf("Clear!\n");
					XClearWindow(dpy,w);
					XClearWindow(dpy,input);
					XClearWindow(dpy,graph_list);
					graph_number = 0;
					l = 0;
					equCount = 0;
					equLength = strlen(equ);
					memset(equ,'\0',equLength);
				}
				if(e.xany.window == draw || forceDraw == 1){
					XSetForeground(dpy,gc,c1[colorCount].pixel);
					colorCount++;
					graph_draw = 1;
					printf("Draw!\n");
					switch(equFlag){
						case 0:
							value = atoi(equ);
							XClearWindow(dpy,input);
							sprintf(text,"%d",value);
							XDrawLine(dpy,w,gc,10,250-value,490,250-value);
							sprintf(graph_equ,"%d : Y = %d",graph_number + 1,value);
							break;
						case 1:
							value = cast(equ,strlen(equ));
							XClearWindow(dpy,input);	
							sprintf(text,"%d",value);
							XDrawLine(dpy,w,gc,10,250-value,490,250-value);
							sprintf(graph_equ,"%d : Y = %d",graph_number + 1,value);
							break;
						case 2:
							XClearWindow(dpy,input);
							sprintf(text,"%fx",coef[coefCount]);
							for(count = 0;count < 480;count++){
								coefX = count + 10;
								coefY = -((count-240) * coef[coefCount]) + 250;
								XDrawPoint(dpy,w,gc,coefX,coefY);
								coefX = coefY = 0;
							}
							forceDraw = mustOpe = 0;
							sprintf(graph_equ,"%d : Y = %fx",graph_number + 1,coef[coefCount]);
							for(count = 0;count < 10;count++) coef[count] = 0;
							break;
						default:
							break;
					}
					equFlag = l = equCount = mustOpe = 0;
					equLength = strlen(equ);
					memset(equ,'\0',equLength);

					if(graph_draw){
						XDrawString(dpy,graph_list,gc,10,(graph_number+1)*10,graph_equ,strlen(graph_equ));
						graph_number++;
					}
					XSetForeground(dpy,gc,c1[8].pixel);
					XDrawString(dpy,input,gc,160,140,text,strlen(text));
				}

				XSetForeground(dpy,gc,c1[8].pixel);

				/*----------- Basic Drawing -------------*/

				XDrawString(dpy,quit,gc,4,10,"Exit",4);
				XDrawString(dpy,draw,gc,4,10,"Draw!",5);
				XDrawString(dpy,clear,gc,4,10,"Clear!",6);
				XDrawString(dpy,sinW,gc,2,8,"sin",3);
				XDrawString(dpy,cosW,gc,2,8,"cos",3);
				XDrawString(dpy,tanW,gc,2,8,"tan",3);
				XDrawString(dpy,w,gc,491,250,"X",1);
				XDrawString(dpy,w,gc,250,8,"Y",1);
				XDrawString(dpy,input,gc,15,80,"Y =",3);
				XDrawString(dpy,input,gc,70,140,"Last Answer:",12);
				XDrawLine(dpy,w,gc,10,250,490,250);
				XDrawLine(dpy,w,gc,250,10,250,490);
				tate = yoko = 0;
				graph_draw = 0;
				graph_equ_length = strlen(graph_equ);
				memset(graph_equ,"\0",graph_equ_length);
				if(colorCount == 8) colorCount = 0; 
				for(count = 0;count < 16;count++){
					XDrawString(dpy,buttons[count],gc,13,20,&buttonString[count],1);
					tate++;
					if((count+1) % 4 == 0){
						yoko++;
						tate = 0;
					}
				}
				count = forceDraw = 0;
				/*-----------------------------------------*/
				break;
		}
	}
}

/*
   @opeFlag 0:計算しなくていい 1:計算する
   @ope   0: +   1: -   2: *   3: /      
   */

float cast(char *equ,int length){
	int opeFlag = 0;
	int count = 0,valueCharCount = 0,valuesCount = 0,ope = 0,oldOpe = 0,minus_first = 0;
	float result = 0;
	int values[10];
	char valuesChar[10];
	// 長さの文だけequの中身を見ていく
	while(count < length){
		switch(equ[count]){
			case '+': 
				// 今までvaluesCharに入っていた数字をvaluesのintに変換する
				values[valuesCount] = atoi(valuesChar);
				memset(valuesChar,'\0',valueCharCount); //初期化
				if(valueCharCount != 0){ //もし何も入っていなかったらカウントする必要がない
					valuesCount++;
				}
				valueCharCount = 0;
				ope = 0;
				opeFlag = 1;
				break;
			case '-':
				if(count == 0){
					minus_first = 1;
				}
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
			if(valuesCount == 1){
				if(minus_first){
					result -= values[valuesCount - 1];
				}else{
					result = values[valuesCount - 1];
				}
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
		}
	}
	return result;
}
