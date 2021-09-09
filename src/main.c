#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "base.h"
#include "Timer.h"

#include "config.h"

int main(int argc, char** argv){
	srandom(time(NULL));

/*## SDL INITIALIZATION ##########################################################################*/
	SDL_Window* window;
	SDL_Renderer* renderer;
	if (init(LIBS, SDL_FLAGS, IMG_FLAGS, NULL) != 0){
		quit(LIBS);
		return 1;
	}
	if (create_window(&window, WINDOW_TITLE,WINDOW_X, WINDOW_Y, WINDOW_W, WINDOW_H, WINDOW_FLAGS)){
		quit(LIBS);
		return 1;
	}
	if (get_renderer_from_window(window, &renderer, REND_FLAGS)){
		close_window(&window);
		quit(LIBS);
		return 1;
	}

/*## MAIN LOOP PREPARATION #######################################################################*/
	char loop = 1;
	SDL_Event e;

/*## VARIABLES USED FOR FRAME RATE MANAGEMENT ####################################################*/
	Timer fps_timer = Timer_init();
	Timer cap_timer = Timer_init();
	Uint8 frames = 1;
	Timer_start(&fps_timer);
	Timer_start(&cap_timer);

/*##################################################################################################
## MAIN LOOP #######################################################################################
##################################################################################################*/
	while (loop){
/*## EVENT HANDLING ##############################################################################*/
		while (SDL_PollEvent(&e) != 0){
			if (e.type == SDL_QUIT){
				loop = 0;
			} else if (e.type == SDL_MOUSEMOTION){
			}
		}
		const Uint8* current_key_states = SDL_GetKeyboardState(NULL);
		if (current_key_states[SDL_SCANCODE_ESCAPE]){
			loop = 0;
		}

/*## RENDERING ###################################################################################*/
		set_background_color(renderer, 0x000000ff);

		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);

		SDL_RenderPresent(renderer);

/*## FRAME RATE SHOWING AND CAPPING ##############################################################*/
		frames++;
		if (frames%MAX_FPS == 0){
			printf("%f\n", frames*1000./(double)Timer_get_ticks(fps_timer));
			frames = 0;
			Timer_start(&fps_timer);
		}
		if (Timer_get_ticks(cap_timer) < TICKS_PER_FRAME){
			SDL_Delay(TICKS_PER_FRAME-Timer_get_ticks(cap_timer));
		}
		Timer_start(&cap_timer);
	}

/*##################################################################################################
## CLOSING EVERYTHING###############################################################################
##################################################################################################*/
	close_renderer(&renderer);
	close_window(&window);
	quit(LIBS);

	return 0;
}
