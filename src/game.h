#ifndef H_GAME_OF_LIFE_GAME
#define H_GAME_OF_LIFE_GAME

#include <SDL.h>
#include "environment.h"

typedef enum	GameEditorMode 
{
	GAME_EDITOR_MODE_ADD,
	GAME_EDITOR_MODE_REMOVE
} GameEditorMode;

typedef struct	GameEditorState
{
	GameEditorMode mode;
	_Bool active;
} GameEditorState;

typedef struct	GameMoveState
{
	_Bool up;
	_Bool down;
	_Bool left;
	_Bool right;

	SDL_Keycode up_key;
	SDL_Keycode down_key;
	SDL_Keycode left_key;
	SDL_Keycode right_key;
} GameMoveState;

typedef struct	GameState
{
	_Bool is_running;
	_Bool is_simulating;
	Environment environment;
	GameEditorState editor;
	GameMoveState moves;
} GameState;

void	Game_Init(GameState *game);
void	Game_Stop(GameState *game);
void	Game_Reset(GameState *game);
void	Game_RunStep(GameState *game);
void	Game_Move(GameState *game);
void	Game_Free(GameState *game);
void	Game_Flush(SDL_Surface *surface, GameState *game);

void	Game_SwitchEditorMode(GameState *game);
void	Game_ToggleSimulation(GameState *game);
void	Game_ToggleCell(GameState *game, int x, int y);

void	Game_SetZoom(GameState *game, double scale_factor, char direction);
void	Game_SetMoveState(GameState *game, SDL_Keycode key, _Bool active);

#endif
