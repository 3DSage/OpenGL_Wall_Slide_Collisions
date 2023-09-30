//------------------YouTube-3DSage-----------------------------
//-------------------------------------------------------------

#include <math.h>
#include <stdio.h>
#include <GL/glut.h>
#include <windows.h>
#define SW 600
#define SH 600
int KEY_L,KEY_R,KEY_U,KEY_D;   //keyboard keys to remove delay
int FPS;                       //hold tick counter for constant frame rate
int numWalls=1;                //number of walls

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
}Walls; Walls W[50];           //increase if needed

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

void movePlayer()
{
 if(KEY_L==1){ P.a+=0.2; if(P.a>2*M_PI){ P.a-=2*M_PI;} P.dx=sin(P.a); P.dy=cos(P.a);}
 if(KEY_R==1){ P.a-=0.2; if(P.a<     0){ P.a+=2*M_PI;} P.dx=sin(P.a); P.dy=cos(P.a);}
 if(KEY_U==1){ P.x-=P.dx*10; P.y+=P.dy*10;} 
 if(KEY_D==1){ P.x+=P.dx*10; P.y-=P.dy*10;}	
}

void drawWalls() //(xy1, xy2, color)
{int w;
 for(w=0;w<numWalls;w++) 
 { 
  line(W[w].x1,W[w].y1, W[w].x2,W[w].y2, W[w].c);
 } 	
}

void drawPlayer()
{
 line(P.x,P.y, P.x-P.dx*20 ,P.y+P.dy*20, 1);
 line(P.x+P.dy*10,P.y+P.dx*10, P.x-P.dy*10 ,P.y-P.dx*10, 1); //point plus vector line	
}

void display() 
{
 if(GetTickCount()-FPS>50) //1000 ticks a second/50 = 20 fps
 { 
  glClear(GL_COLOR_BUFFER_BIT);

  movePlayer();
  drawWalls();
  drawPlayer();

  FPS=GetTickCount();
  glutSwapBuffers();
 }
 glutPostRedisplay();
}

void init()
{
 W[0].x1= 20; W[0].y1= 20; W[0].x2=200; W[0].y2=200;
 P.x=400; P.y=200; P.dx=sin(P.a); P.dy=cos(P.a);
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
 glutInitWindowPosition(450,150); //adjust initial window position
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

