#include <SDL.h>
#include "constants.h"
#include "environment.h"

int*	Environment_CreateState()
{
	int* state = malloc(FLAT_ENVIRONMENT_LENGTH * sizeof(int));	
	
	if (state == NULL)
	{
		printf("Failed to create environment state.\n");
		exit(1);
	}

	return state;
}

void	Environment_Empty(Environment *environment)
{
	for(int y = 0; y < W_ROWS; y++)
		for(int x = 0; x < W_COLS; x++)
			environment->state[x + y * W_COLS] = 0;
}

void	Environment_Flush(SDL_Surface *surface, Environment *environment)
{
	for(int y = 0; y < W_ROWS; y++)
		for(int x = 0; x < W_COLS; x++)
		{
			int relative_pos_x, relative_pos_y;
			Environment_RemapPosition(environment, x, y, &relative_pos_x, &relative_pos_y);
			
			int cell_size = Environment_GetCellSize(environment);
			int cell_state = Environment_GetCellState(environment, x, y, 0);
	
			SDL_Rect border = { relative_pos_x - 2, relative_pos_y - 2, cell_size + 4, cell_size + 4 };
			SDL_FillRect(surface, &border, COLOR_GRAY_DARK);

			SDL_Rect rect = { relative_pos_x, relative_pos_y, cell_size, cell_size };

			if (cell_state == 1)
				SDL_FillRect(surface, &rect, COLOR_WHITE);
			else
				SDL_FillRect(surface, &rect, COLOR_BLACK);
		}
}

void	Environment_Free(Environment *environment)
{
	if (environment->state != NULL)
		free(environment->state);
}

void	Environment_ToggleCell(Environment *environment, int x, int y, _Bool remap)
{
	if (x < 0 || x >= W_COLS || y < 0 || y >= W_ROWS)
		return;

	int remap_x = x, remap_y = y;

	if (remap == 1)
		Environment_RemapIndex(environment, x, y, &remap_x, &remap_y);

	environment->state[remap_x + remap_y * W_COLS] = !environment->state[remap_x + remap_y * W_COLS];
}

char	Environment_GetCellState(Environment *environment, int x, int y, _Bool remap)
{
	if (x < 0 || x >= W_COLS || y < 0 || y >= W_ROWS)
		return -1;

	int remap_x = x;
	int remap_y = y;
	
	if (remap == 1)
		Environment_RemapIndex(environment, x, y, &remap_x, &remap_y);

	return environment->state[remap_x + remap_y * W_COLS];
}

int	Environment_GetCellSize(Environment *environment)
{
	return CELL_SIZE * environment->zoom;
}

_Bool	Environment_CheckCellIsAlive(Environment *environment, int x, int y)
{
	int alive_around = 0;
	int cell_state = Environment_GetCellState(environment, x, y, 0);

	for(int around_cell_y = y - 1; around_cell_y < W_ROWS && around_cell_y <= y + 1; around_cell_y++)
		for(int around_cell_x = x - 1; around_cell_x < W_COLS && around_cell_x <= x + 1; around_cell_x++)
		{
			if (around_cell_x == x && around_cell_y == y)
				continue;

			int cell_around_state = Environment_GetCellState(environment, around_cell_x, around_cell_y, 0);

			if (cell_around_state == -1)
				continue;

			alive_around += cell_around_state;

			if (alive_around > (int)fmax(RULE_DEAD_TO_ALIVE_REQUIRES_ALIVE, RULE_STAYING_ALIVE_EXPECTS_MAX_ALIVE))
				break;
		}

	if (cell_state == 1 && alive_around >= RULE_STAYING_ALIVE_EXPECTS_MIN_ALIVE && alive_around <= RULE_STAYING_ALIVE_EXPECTS_MAX_ALIVE)
		return 1;
	else if (cell_state == 0 && alive_around == RULE_DEAD_TO_ALIVE_REQUIRES_ALIVE)
		return 1;
	return 0;
}

void	Environment_RunStep(Environment *environment)
{
	int	*next_state = Environment_CreateState();

	for(int y = 0; y < W_ROWS; y++)
		for(int x = 0; x < W_COLS; x++)
			next_state[x + y * W_COLS] = Environment_CheckCellIsAlive(environment, x, y);
	
	for(int y = 0; y < W_ROWS; y++)
		for(int x = 0; x < W_COLS; x++)
			environment->state[x + y * W_COLS] = next_state[x + y * W_COLS];
	
	free(next_state);
}

void	Environment_RemapIndex(Environment *environment, int x, int y, int *out_x, int *out_y)
{
	int cell_size = Environment_GetCellSize(environment);
	
	*out_x = (x / cell_size) + environment->padding_x;
	*out_y = (y / cell_size) + environment->padding_y;
}

void	Environment_RemapPosition(Environment *environment, int x, int y, int *out_x, int *out_y)
{
	int cell_size = Environment_GetCellSize(environment);
	
	*out_x = ((x - environment->padding_x) * cell_size);
	*out_y = ((y - environment->padding_y) * cell_size);
}

