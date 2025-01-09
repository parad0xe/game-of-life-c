#ifndef H_GAME_OF_LIFE_ENVIRONMENT
#define H_GAME_OF_LIFE_ENVIRONMENT

#include <SDL.h>

typedef struct	Environment
{
	int *state;
	int padding_x;
	int padding_y;
	double zoom;
} Environment;

int*	Environment_CreateState();
void	Environment_Empty(Environment *environment);
void	Environment_RunStep(Environment *environment);
void	Environment_Flush(SDL_Surface *surface, Environment *environment);
void	Environment_Free(Environment *environment);

void	Environment_ToggleCell(Environment *environment, int x, int y, _Bool remap);
char	Environment_GetCellState(Environment *environment, int x, int y, _Bool remap);
int	Environment_GetCellSize(Environment *environment);

_Bool	Environment_CheckCellIsAlive(Environment *environment, int x, int y);
void	Environment_RemapIndex(Environment *environment, int x, int y, int *out_x, int *out_y);
void	Environment_RemapPosition(Environment *environment, int x, int y, int *out_x, int *out_y);

#endif
