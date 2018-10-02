//compilar con : g++ main.cpp -o main -lGL -lGLU -lglut
#include <windows.h>
#include <iostream>
#include <GL/glut.h>
#include <vector>
#include <time.h>
#include <math.h>
#include <stdio.h>

using namespace std;

#define XCOR 40
#define YCOR 40
#define VIDA 4

/*
=========================
= CONFIGURACION INICIAL =
=========================
*/


int level = 4;

typedef struct Tanque{
    float  x, y, rot;
    int vida;
    Tanque(float ax, float ay, float arot, int avida ) : x(ax), y(ay), rot(arot), vida(avida) { }
}Tanque;

typedef struct Bala{
    float x,y, rot;
    Bala(float ax, float ay, float arot) : x(ax), y(ay), rot(arot) { }
}Bala;

static int shoot = 0;

float static izq=0,der=0,arr=0,abj=0,disparo=0;
Tanque tanque(0.0, 0.0, 0.0, VIDA);

vector <Bala> balas;
vector <Tanque> tanques_enemigos;
vector <Bala> balas_enemigos;

float tiempo = 0;

float rots[] = { 0,-90, 90,180};


/*
==================================================================
= FUNCION PARA INICIAR EL LEVEL UNO (crear los tanques enemigos) =
==================================================================
*/

void empezarLevelUno(int n){

    float init_coord[12][2] = {{-36,36},{-36,-36},{36,36},{36,-36},{-36,0},{36,0},{-36,36},{-36,-36},{36,36},{36,-36},{-36,0},{36,0}};

    for(int i=0; i < n; i++){

        int r = rand()%4;
        Tanque nuevo(init_coord[i][0], init_coord[i][1], rots[r], VIDA);
        tanques_enemigos.push_back(nuevo);
    }
}

/*
===============================================
= FUNCION PARA LA CONFIGURACION DE LA VENTANA =
===============================================
*/

void IniciarGLUT() {

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(600,600);
    glutInitWindowPosition(50,50);
    glutCreateWindow("SPACE WAR");

}


/*
===================================
= FUNCION PARA GRAFICAR ESCENARIO =
===================================
*/

void dibujarEscenario(){

    GLint factor=1;
    GLushort patron = 0xAAAA;

    glEnable(GL_LINE_STIPPLE);

    glLineStipple(factor,patron);
    glColor3f(0.5,0.5,0.3);
    glLineWidth(1);

    glBegin(GL_LINES);

    for (float i=-XCOR; i<=XCOR; i++){
        glVertex2f(i, XCOR);
        glVertex2f(i,-XCOR);
        glVertex2f(-XCOR,i);
        glVertex2f(XCOR, i);
    }

    glEnd();
    glDisable(GL_LINE_STIPPLE);
}

/*
=================================
= GRAFICA DE LA BALA DEL TANQUE =
=================================
*/

void dibujarBala(Bala *bala){

    glColor3f(0.8F, 0.0f, 0.0f);

    glPushMatrix();
    glTranslatef(bala->x,bala->y,0.0);
    glRotatef(bala->rot,0.0,0.0,1.0);
    glBegin(GL_QUADS);
    glVertex2f(0.5, 3.0);
    glVertex2f(0.5, 2.0);
    glVertex2f(-0.5,2.0);
    glVertex2f(-0.5,3.0);
    glEnd();
    glPopMatrix();

}

/*
=================================================
= FUNCION PARA DISMINUIR LA VIDA DEL ENEMIGO SI =
= LE IMPACTA UNA BALA y ELIMINAR LA BALA        =
=================================================
*/

void revisarImpactoBalasEnemigos(){

    for(int i = 0; i < balas.size(); i++){

        for(int j = 0; j < tanques_enemigos.size(); j++){

            if( balas[i].x < (tanques_enemigos[j].x + 3) &&
                balas[i].x > (tanques_enemigos[j].x - 3) &&
                balas[i].y < (tanques_enemigos[j].y + 3) &&
                balas[i].y > (tanques_enemigos[j].y - 3)  ){

                tanques_enemigos[j].vida--;

                balas.erase(balas.begin() + i);
               }
        }
    }
}

void RevisarImpactoBalasTanquePeruano(){

    for(int i = 0; i < balas_enemigos.size(); i++){
         if( balas_enemigos[i].x < (tanque.x + 3) &&
             balas_enemigos[i].x > (tanque.x - 3) &&
             balas_enemigos[i].y < (tanque.y + 3) &&
             balas_enemigos[i].y > (tanque.y - 3)  ){

             tanque.vida--;

            }
    }

}

void revisarVidaTanquesEnemigos(){

    for(int i = 0; i < tanques_enemigos.size(); i++){
        if(tanques_enemigos[i].vida < 0){
            tanques_enemigos.erase(tanques_enemigos.begin() + i);
        }
    }
}

void movimientoBalaEnemiga(){

    for(int i = 0; i < tanques_enemigos.size(); i++){

        if((tanque.x < (tanques_enemigos[i].x + 1)  &&
            tanque.x > (tanques_enemigos[i].x - 1)) ||
           (tanque.y < (tanques_enemigos[i].y + 1)  &&
            tanque.y > (tanques_enemigos[i].y - 1))){

                Bala nuevo(tanques_enemigos[i].x, tanques_enemigos[i].y, tanques_enemigos[i].rot);
                balas_enemigos.push_back(nuevo);
            }
    }

    for(int i = 0; i< balas_enemigos.size(); i++){
        balas_enemigos[i].x = balas_enemigos[i].x - 1.5*sin(balas_enemigos[i].rot*M_PI/180);
        balas_enemigos[i].y = balas_enemigos[i].y + 1.5*cos(balas_enemigos[i].rot*M_PI/180);

        if(balas_enemigos[i].x > 39 || balas_enemigos[i].x < -39 || balas_enemigos[i].y > 39 || balas_enemigos[i].y < -39){
            balas_enemigos.erase(balas_enemigos.begin() + i);
        }
    }

}

/*
==========================================
= GRAFICA DEL PERSONAJE PRINCIPAL TANQUE =
==========================================
*/
void dibujarTanque(Tanque *tanque, float color){

    glPushMatrix();

    glTranslatef(tanque->x,tanque->y,0.0);
    glRotatef(tanque->rot,0.0,0.0,1.0);

    glColor3f(0.7,0.7,0.0);
    glBegin(GL_QUADS);
    glVertex2f(1.0, 3.0);
    glVertex2f(1.0, 2.0);
    glVertex2f(-1.0,2.0);
    glVertex2f(-1.0,3.0);

    if(color == 1) glColor3f((float) (tanque->vida/5.0), 0.0, 0.0 );
    else           glColor3f(0.0 , 0.0, (float) (tanque->vida/5.0));

    glBegin(GL_QUADS);
    glVertex2f(2.0, 2.0);
    glVertex2f(2.0,-2.0);
    glVertex2f(-2.0,-2.0);
    glVertex2f(-2.0, 2.0);
    glEnd();

    glPopMatrix();

}

/*
==========================================================
= FUNCION PRINCIPAL QUE MUESTRA LOS GRAFICOS EN PANTALLA =
==========================================================
*/
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    dibujarEscenario();

    if(tanque.vida >= 0) dibujarTanque(&tanque, 0);

    for(int i=0; i<tanques_enemigos.size(); i++){
        dibujarTanque(&tanques_enemigos[i], 1);
    }

    for(int i = 0; i < balas.size(); i++){
        dibujarBala(&balas[i]);
    }

    for(int i = 0; i < balas_enemigos.size(); i++){
        dibujarBala(&balas_enemigos[i]);
    }

    glutSwapBuffers();
    glFlush();
}

/*
=====================================
= FUNCION PARA CONTROLAR EL TECLADO =
=====================================
*/


void keyPress(unsigned char key, int x, int y){

    switch(key){

        case 'a':
            izq = 1;
            break;

        case 'd':
            der = 1;
            break;

        case 'w':
            arr = 1;
            break;

        case 's':
            abj = 1;
            break;

        case  32:
            disparo = 1;
            break;

        default:
            break;
    }
}

void keyRelease(unsigned char key, int x, int y) {

    switch(key){

        case 'a':
            izq = 0;
            break;

        case 'd':
            der = 0;
            break;

        case 'w':
            arr = 0;
            break;

        case 's':
            abj = 0;
            break;
        case 32 :
            disparo = 0;
        default:
            break;
    }
}

void movimientoTanqueEnemigo(){

    static int op[4];

    for(int i=0; i<tanques_enemigos.size(); i++){

        if (tiempo <= 0 ){
            op[i] = rand() % 4;
        }

        switch(op[i]) {
            case 0 : {
                tanques_enemigos[i].x+=0.6;
                tanques_enemigos[i].rot = -90;
                if(tanques_enemigos[i].x > 38) op[i] = 1;
                break;
            }
            case 1 : {
                tanques_enemigos[i].x-=0.6;
                tanques_enemigos[i].rot = 90;
                if(tanques_enemigos[i].x < -38) op[i] = 0;
                break;
            }
            case 2 : {
                tanques_enemigos[i].y+=0.6;
                tanques_enemigos[i].rot = 0;
                if(tanques_enemigos[i].y > 38) op[i] = 3;
                break;
            }
            case 3 : {
                tanques_enemigos[i].y-=0.6;
                tanques_enemigos[i].rot = 180;
                if(tanques_enemigos[i].y < -38) op[i] = 2;
                break;
            }
        }
    }

    if(tiempo < 36) tiempo ++;
    else            tiempo = 0;
}

void movimientoTanque(){

    if(der && izq){

    }else if(der == 1){
        tanque.x++;
        tanque.rot = -90;
    }else if(izq == 1){
        tanque.x--;
        tanque.rot = 90;
    }

    if(arr && abj){

    }else if(arr == 1){
        tanque.y++;
        tanque.rot = 0;
    }else if(abj == 1){
        tanque.y--;
        tanque.rot = 180;
    }
}

void movimientoBala(){

    if(disparo == 1){
        Bala nuevo(tanque.x, tanque.y, tanque.rot);
        balas.push_back(nuevo);
    }


    for(int i = 0; i< balas.size(); i++){
        balas[i].x = balas[i].x - 1.5*sin(balas[i].rot*M_PI/180);
        balas[i].y = balas[i].y + 1.5*cos(balas[i].rot*M_PI/180);

        if(balas[i].x > 39 || balas[i].x < -39 || balas[i].y > 39 || balas[i].y < -39){
            balas.erase(balas.begin() + i);
        }
    }

    disparo = 0;
}

void delimitador(){

    if(tanque.x >  38) tanque.x = 38;
    if(tanque.x < -38) tanque.x =-38;
    if(tanque.y >  38) tanque.y = 38;
    if(tanque.y < -38) tanque.y =-38;

}

void temporizador(int value){

    movimientoTanque();
    movimientoTanqueEnemigo();
    movimientoBala();
    movimientoBalaEnemiga();
    delimitador();
    revisarImpactoBalasEnemigos();
    RevisarImpactoBalasTanquePeruano();
    revisarVidaTanquesEnemigos();
    movimientoTanqueEnemigo();

    glutPostRedisplay();
    glutTimerFunc(60, temporizador, value);      /* 30 frames per second */

}

/*
==================================================
= FUNCION PARA CONTROLAR EL TAMAÑO DE LA VENTANA =
==================================================
*/
void ReProyectar(int w, int h) {
    GLfloat formato;
    if(h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    formato = (GLfloat)w / (GLfloat)h;
    if (w <= h) glOrtho (-40.0f, 40.0f, -40.0f / formato, 40.0f / formato, 1.0f, -1.0f);
    else glOrtho (-40.0f * formato, 40.0f * formato, -40.0f, 40.0f, 1.0f, -1.0f);
}


int main(int argc, char **argv)
{
    srand(time(NULL));
    glutInit(&argc,argv);
    IniciarGLUT();

    empezarLevelUno(6);

    glutDisplayFunc(display);
    glutIdleFunc(display);

    glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyRelease);
    glutReshapeFunc(ReProyectar);
    glutTimerFunc(60, temporizador, 0);

    glutMainLoop();
    return 0;
}
