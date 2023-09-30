//------------------YouTube-3DSage-----------------------------
//-------------------------------------------------------------

#include <math.h>
#include <stdio.h>
#include <GL/glut.h>
#include <windows.h>
#define SW 600
#define SH 600
int KEY_L,KEY_R,KEY_U,KEY_D;   //keyboard keys
int FPS;                       //hold tick start

int numWalls=21;               //total number of walls

typedef struct
{
 float a;                      //rotation angle 0-2PI
 int x,y;                      //position
 float dx,dy;                  //vector angle
}Player; Player P;

 typedef struct
{
 int x1,y1, x2,y2;             //2 points per wall
 int c;                        //color
 float dx,dy;                  //vector angle
 int door;                     //is this wall a door
 int dt;                       //door timer
}Walls; Walls W[50];

void line(int x1,int y1, int x2,int y2, int c)
{
 if(c==0){  glColor3f(0,0,0);} //black
 if(c==1){  glColor3f(0,0,1);} //blue
 if(c==2){  glColor3f(1,0,0);} //red
 glBegin(GL_LINES);
 glVertex2i(x1,y1);
 glVertex2i(x2,y2);
 glEnd(); 
}

void collisions()
{
 int w;
 for(w=0;w<numWalls;w++)
 {
  int x1=W[w].x1, y1=W[w].y1, x2=W[w].x2, y2=W[w].y2, dx=x2-x1, dy=y2-y1;
  int wallOff=32; //amount the wall pushes you back
   
  //vertical line
  if(dx==0)  
  {
   int d=abs(x1-P.x);                                   //distance to wall
   if(d<wallOff)                                        //inside that distance
   {
	if(y1>y2){ int t=y1; y1=y2; y2=t;}                  //order y bottom to top
    if(P.y<y1-wallOff/2 || P.y>y2+wallOff/2){ continue;}//out of edges
    if(P.x<x1){ P.x-=wallOff-d;}                        //push player backwards
    else      { P.x+=wallOff-d;}                        //push player backwards
   }
   continue;                                            //done with this wall!
  }  

  //horizontal line
  if(dy==0)  
  {
   int d=abs(y1-P.y);                                   //distance to wall
   if(d<wallOff)                                        //inside that distance
   {
	if(x1>x2){ int t=x1; x1=x2; x2=t;}                  //order x left to right
    if(P.x<x1-wallOff/2 || P.x>x2+wallOff/2){ continue;}//out of edges
    if(W[w].door==1){ W[w].x1=W[w].x2; W[w].dt=20;}     //door
    if(P.y<y1){ P.y-=wallOff-d;}                        //push player backwards
    else      { P.y+=wallOff-d;}                        //push player backwards
   }
   continue;                                            //done with this wall!
  } 

  //angled line
  int pdx=P.x-x1, pdy=P.y-y1;                           //player and wall difference 
  float d=dx*dx+dy*dy; if(d==0){ d=0.0001;}             //closest distance percentage
  d=(dx*pdx+dy*pdy)/d;                                  //percentage on line 0-1
  float lineD=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));    //line length
  float ends=(wallOff/2)/lineD;                         //half bounding out of 256
  if(d<0-ends || d>1+ends){ continue;}                  //out of edges 
  dx=d*dx+x1;                                           //closest x position
  dy=d*dy+y1;                                           //closest y position
  d=sqrt((dx-P.x)*(dx-P.x)+(dy-P.y)*(dy-P.y));          //distance player center point

  if(d<wallOff)                                         //inside bounding distance                
  {
   int crossP=(x2-x1)*pdy-(y2-y1)*pdx;                  //cross product for normal
   int xo=W[w].dx*wallOff;                              //x offset
   int yo=W[w].dy*wallOff;                              //y offset
   if(crossP>0){ P.x-=P.x-(dx-xo); P.y-=P.y-(dy-yo);}   //normal side, push back
   else        { P.x-=P.x-(xo+dx); P.y-=P.y-(yo+dy);}   //opposite side of normal 
  }
 }
}

void movePlayer()
{
 if(KEY_L==1){ P.a+=0.2; if(P.a>2*M_PI){ P.a-=2*M_PI;} P.dx=sin(P.a); P.dy=cos(P.a);}
 if(KEY_R==1){ P.a-=0.2; if(P.a<     0){ P.a+=2*M_PI;} P.dx=sin(P.a); P.dy=cos(P.a);}
 if(KEY_U==1){ P.x-=P.dx*10; P.y+=P.dy*10; collisions();}
 if(KEY_D==1){ P.x+=P.dx*10; P.y-=P.dy*10; collisions();}
 if(P.x<20){ P.x=20;} if(P.x>SW-20){ P.x=SW-20;}
 if(P.y<20){ P.y=20;} if(P.y>SH-20){ P.y=SH-20;}
}

void drawWalls()
{int w;
 for(w=0;w<numWalls;w++) //(xy1, xy2, color)
 { 
  line(W[w].x1,W[w].y1, W[w].x2,W[w].y2, W[w].c);
  int mx= (W[w].x1+W[w].x2)/2; //middle x
  int my= (W[w].y1+W[w].y2)/2; //middle y
  line(mx,my, mx+W[w].dx*10, my+W[w].dy*10, 0);
 } 	
}

void drawPlayer()
{
 line(P.x,P.y, P.x-P.dx*20 ,P.y+P.dy*20, 1);
 line(P.x+P.dy*10,P.y+P.dx*10, P.x-P.dy*10 ,P.y-P.dx*10, 1);	
}

void display() 
{
 if(GetTickCount()-FPS>50) //1000 ticks a second/50 = 20 fps
 { 
  glClear(GL_COLOR_BUFFER_BIT);

  movePlayer();
  drawWalls();
  drawPlayer();

  //(I decided to add automatic doors just before uploading this code)
  //door timer
  if(W[20].dt>0){ W[20].dt-=1;}         //door timer 
  if(W[20].dt==0){ W[20].x1=196;}       //door close

  FPS=GetTickCount();
  glutSwapBuffers();
 }
 glutPostRedisplay();
}


void init()
{
 W[ 0].x1= 64; W[ 0].y1= 96; W[ 0].x2= 64; W[ 0].y2=192; W[ 0].c=0;
 W[ 1].x1= 64; W[ 1].y1=192; W[ 1].x2=160; W[ 1].y2=256; W[ 1].c=0;
 W[ 2].x1=160; W[ 2].y1=256; W[ 2].x2=160; W[ 2].y2=384; W[ 2].c=0;
 W[ 3].x1=160; W[ 3].y1=384; W[ 3].x2= 64; W[ 3].y2=448; W[ 3].c=0;
 W[ 4].x1= 64; W[ 4].y1=448; W[ 4].x2= 64; W[ 4].y2=512; W[ 4].c=0;
 W[ 5].x1= 64; W[ 5].y1=512; W[ 5].x2=224; W[ 5].y2=512; W[ 5].c=0;
 W[ 6].x1=224; W[ 6].y1=512; W[ 6].x2=288; W[ 6].y2=416; W[ 6].c=0;
 W[ 7].x1=288; W[ 7].y1=416; W[ 7].x2=352; W[ 7].y2=416; W[ 7].c=0;
 W[ 8].x1=352; W[ 8].y1=416; W[ 8].x2=416; W[ 8].y2=512; W[ 8].c=0;
 W[ 9].x1=416; W[ 9].y1=512; W[ 9].x2=512; W[ 9].y2=512; W[ 9].c=0;
 W[10].x1=512; W[10].y1=512; W[10].x2=512; W[10].y2=320; W[10].c=0;
 W[11].x1=512; W[11].y1=320; W[11].x2=352; W[11].y2=320; W[11].c=0;
 W[12].x1=352; W[12].y1=320; W[12].x2=352; W[12].y2=224; W[12].c=0;
 W[13].x1=352; W[13].y1=224; W[13].x2=512; W[13].y2=224; W[13].c=0;
 W[14].x1=512; W[14].y1=224; W[14].x2=512; W[14].y2=160; W[14].c=0;
 W[15].x1=512; W[15].y1=160; W[15].x2=448; W[15].y2= 96; W[15].c=0;
 W[16].x1=448; W[16].y1= 96; W[16].x2=320; W[16].y2= 96; W[16].c=0;
 W[17].x1=320; W[17].y1= 96; W[17].x2=256; W[17].y2=192; W[17].c=0;
 W[18].x1=256; W[18].y1=192; W[18].x2=192; W[18].y2=192; W[18].c=0;
 W[19].x1=192; W[19].y1=192; W[19].x2=192; W[19].y2= 96; W[19].c=0;
 //door
 W[20].x1=192; W[20].y1= 96; W[20].x2= 64; W[20].y2= 96; W[20].c=2; W[20].door=1;

  //precalculate wall vector
 int w;
 for(w=0;w<numWalls;w++)
 {
  int dx=W[w].x2-W[w].x1; 
  int dy=W[w].y2-W[w].y1;
  float ln=sqrt(dx*dx+dy*dy); //length of line
  if(ln==0){ ln=0.00001;}
  W[w].dx= dy/ln; 
  W[w].dy=-dx/ln;
 }

 P.x=300; P.y=300; P.dx=sin(P.a); P.dy=cos(P.a);
}

void KeyDown(unsigned char key,int x,int y)   
{ 
 if(key=='a'){ KEY_L=1;}
 if(key=='d'){ KEY_R=1;}
 if(key=='w'){ KEY_U=1;}
 if(key=='s'){ KEY_D=1;}
}
void KeyUp(unsigned char key,int x,int y)
{
 if(key=='a'){ KEY_L=0;}
 if(key=='d'){ KEY_R=0;}
 if(key=='w'){ KEY_U=0;}
 if(key=='s'){ KEY_D=0;}
}

int main(int argc, char** argv)
{ 
 glutInit(&argc, argv);
 glutInitDisplayMode(GLUT_DOUBLE);
 glutInitWindowPosition(450,150);
 glutInitWindowSize(SW,SH);
 glutCreateWindow("YouTube-3DSage");
 glutDisplayFunc(display);
 gluOrtho2D(0,SW,0,SH);    //origin bottom left corner
 glClearColor(0.8,0.9,0.8,0);
 glLineWidth(4);
 init();
 glutKeyboardFunc(KeyDown);     
 glutKeyboardUpFunc(KeyUp);
 glutMainLoop();
 return 0;
}

