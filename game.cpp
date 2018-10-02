#include "game.h"

void Game::update_screen(){
	// Set clear color 
    glClearColor(0,1,0,1);  
	// Clear screen
    glClear(GL_COLOR_BUFFER_BIT);
	
	
	// Change buffer	
	glutSwapBuffers();
	// Render buffer in the screen
    glFlush();	
}


