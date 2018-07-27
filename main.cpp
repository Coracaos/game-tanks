#include <GL/glut.h>

void display_screen(void){

	// Set clear color 
    glClearColor(0,1,0,1);  
	// Clear screen
    glClear(GL_COLOR_BUFFER_BIT);


	// Change buffer	
	glutSwapBuffers();
	// Render buffer in the screen
    glFlush();
}

int main(int argc, char **argv){

	// Init library glut
    glutInit(&argc, argv);
	// Initial set mode display	
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowPosition(50,25);
    glutInitWindowSize(500,250);

    glutCreateWindow("Title Window");

	//display screen
    glutDisplayFunc(display_screen);
		
	//Main Loop - call glutDisplayFunc  
    glutMainLoop();
	
    return 0;
}

