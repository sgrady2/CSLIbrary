/*
	SDL Test
	Demonstrates SDL being initialized and shut down.
	written by Sayuri L. Kitsune (2012)
*/

/* Includes */
#include <SDL.h>
#include <stdio.h>

/* Main */
int main(int argn,char **argv)
{
	/* Init */
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
		fprintf(stderr,"Could not initialize SDL: %s\n",SDL_GetError()); /* Error? */
	printf("SDL Initialized\n");
	/* Exit */
	SDL_Quit();
	printf("SDL Shutdown\n");
	/* Done */
	return 0;
}

