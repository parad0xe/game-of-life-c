#include <SDL.h>
#include "game.h"
#include "environment.h"
#include "constants.h"

void    Game_Init(GameState *game)
{	
	game->is_running = 1;
	game->environment.zoom = 1;

	game->moves.up_key = SDLK_z;
	game->moves.right_key = SDLK_d;
	game->moves.down_key = SDLK_s;
	game->moves.left_key = SDLK_q;

	game->environment.state = Environment_CreateState();
	Environment_Empty(&game->environment);
}

void    Game_Stop(GameState *game)
{
	game->is_running = 0;
}

void    Game_Reset(GameState *game)
{
	game->is_simulating = 0;
	Environment_Empty(&game->environment);	
}

void    Game_RunStep(GameState *game)
{
	Environment_RunStep(&game->environment);
}

void    Game_Move(GameState *game)
{
	int speed = 1.5 * (1 / game->environment.zoom * 1.8);
	if (game->moves.left) game->environment.padding_x -= speed;
	if (game->moves.up) game->environment.padding_y -= speed;
	if (game->moves.right) game->environment.padding_x += speed;
	if (game->moves.down) game->environment.padding_y += speed;
}

void	Game_Free(GameState *game)
{
	Environment_Free(&game->environment);
}

void Game_Flush(SDL_Surface *surface, GameState *game)
{
	Environment_Flush(surface, &game->environment);

	if (game->is_simulating == 1)
	{
		SDL_Rect simulation_indicator = { W_COLS - 15, 5, 10, 10 };
		SDL_FillRect(surface, &simulation_indicator, COLOR_RED);
	}
	
	SDL_Rect mode_indicator = { 5, 5, 10, 10 };
	switch (game->editor.mode)
	{
		case GAME_EDITOR_MODE_ADD:
			SDL_FillRect(surface, &mode_indicator, COLOR_GREEN);
			break;
		case GAME_EDITOR_MODE_REMOVE:
			SDL_FillRect(surface, &mode_indicator, COLOR_BLUE);
			break;
	}
}

void    Game_SwitchEditorMode(GameState *game)
{
	game->editor.mode = 1 - ((game->editor.mode) % 2);
}

void    Game_ToggleSimulation(GameState *game)
{
	game->is_simulating = !game->is_simulating;
}

void	Game_ToggleCell(GameState *game, int x, int y)
{
	int cell_state = Environment_GetCellState(&game->environment, x, y, 1);

	if (
		(game->editor.mode == GAME_EDITOR_MODE_REMOVE && cell_state == 0) || 
		(game->editor.mode == GAME_EDITOR_MODE_ADD && cell_state == 1))
		return;

	Environment_ToggleCell(&game->environment, x, y, 1);
}


void	Game_SetZoom(GameState *game, double scale_factor, char direction)
{
	direction = (direction > 0) - (direction < 0);
	game->environment.zoom += scale_factor * direction;

	if (game->environment.zoom < 0.3)
		game->environment.zoom = 0.3;
	else if(game->environment.zoom > 2.5)
		game->environment.zoom = 2.5;
}

void	Game_SetMoveState(GameState *game, SDL_Keycode key, _Bool active)
{
	if (key == game->moves.up_key) 
		game->moves.up = active;
	else if (key == game->moves.down_key) 
		game->moves.down = active;	
	else if (key == game->moves.left_key) 
		game->moves.left = active;	
	else if (key == game->moves.right_key) 
		game->moves.right = active;
}

