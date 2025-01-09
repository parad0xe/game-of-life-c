#include <stdio.h>
#include <SDL.h>
#include "constants.h"
#include "game.h"

void	clear_surface(SDL_Surface *surface)
{
	SDL_Rect rect = { 0, 0, W_COLS, W_ROWS };
	SDL_FillRect(surface, &rect, COLOR_BLACK);
}

int	main()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Failed to initialize SDL.\n");
		exit(1);
	}
	else
		printf("SDL initialized\n");

	SDL_Window	*window = SDL_CreateWindow("Game of life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W_COLS, W_ROWS, 0);
	
	if (window == NULL)
	{
		printf("Failed to initialize Window.\n");
		exit(1);
	}
	else
		printf("Window initialized\n");

	SDL_SetWindowResizable(window, SDL_FALSE);

	SDL_Surface	*surface = SDL_GetWindowSurface(window);
	
	if (surface == NULL)
	{
		printf("Failed to initialize Surface.\n");
		exit(1);
	}
	else
		printf("Surface initialized\n");

	GameState game = { 0 };
	Game_Init(&game);

	SDL_Event event;

	while (game.is_running)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type) 
			{
				case SDL_QUIT:
					Game_Stop(&game);
					break;

				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_SPACE)
						Game_ToggleSimulation(&game);
					else if (event.key.keysym.sym == SDLK_TAB)
						Game_SwitchEditorMode(&game);
					else if (event.key.keysym.sym == SDLK_r)
						Game_Reset(&game);
					else if (event.key.keysym.sym == SDLK_n && !game.is_simulating)
						Game_RunStep(&game);


					Game_SetMoveState(&game, event.key.keysym.sym, 1);
					break;

				case SDL_KEYUP:
					Game_SetMoveState(&game, event.key.keysym.sym, 0);
					break;

				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						Game_ToggleCell(&game, event.button.x, event.button.y);
						game.editor.active = 1;
					}
					break;
				
				case SDL_MOUSEWHEEL:
					Game_SetZoom(&game, 0.1, event.wheel.y);
					break;

				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT)
						game.editor.active = 0;
					break;

				case SDL_MOUSEMOTION:
					if (game.editor.active == 0)
						break;

					Game_ToggleCell(&game, event.motion.x, event.motion.y);
					break;
			}
		}

		clear_surface(surface);

		Game_Move(&game);
		
		if (game.is_simulating == 1)
			Game_RunStep(&game);

		Game_Flush(surface, &game);

		SDL_UpdateWindowSurface(window);
		SDL_Delay(30);
	}

	Game_Free(&game);
	SDL_Quit();

	return 0;
}
