#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "base.h"
#include "Timer.h"

#include "config.h"

void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius);

typedef struct Node{
	float x, y, vx, vy, ax, ay;
	char locked, active;
} Node;

#define NB_NODES 10

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

float DT = 1./MAX_FPS;

	Node nodes[NB_NODES];
	for (int i = 0; i < NB_NODES; i++){
		nodes[i].active = 0;
	}
	char connections[NB_NODES][NB_NODES];
	for (int i = 0; i < NB_NODES; i++){
		for (int j = 0; j < NB_NODES; j++){
			connections[i][j] = 0;
		}
	}

	nodes[0].x = WINDOW_W/2-100;
	nodes[0].y = WINDOW_H/4-100;
	nodes[0].vx = 0;
	nodes[0].vy = 0;
	nodes[0].ax = 0;
	nodes[0].ay = 0;
	nodes[0].locked = 0;
	nodes[0].active = 1;
	nodes[1].x = WINDOW_W/2+100;
	nodes[1].y = WINDOW_H/4-100;
	nodes[1].vx = 0;
	nodes[1].vy = 0;
	nodes[1].ax = 0;
	nodes[1].ay = 0;
	nodes[1].locked = 0;
	nodes[1].active = 1;
	nodes[2].x = WINDOW_W/2-100;
	nodes[2].y = WINDOW_H/4+100;
	nodes[2].vx = 0;
	nodes[2].vy = 0;
	nodes[2].ax = 0;
	nodes[2].ay = 0;
	nodes[2].locked = 0;
	nodes[2].active = 1;
	nodes[3].x = WINDOW_W/2+100;
	nodes[3].y = WINDOW_H/4+100;
	nodes[3].vx = 0;
	nodes[3].vy = 0;
	nodes[3].ax = 0;
	nodes[3].ay = 0;
	nodes[3].locked = 0;
	nodes[3].active = 1;
	connections[0][1] = 1;
	connections[0][2] = 1;
	connections[0][3] = 1;
	connections[1][2] = 1;
	connections[1][3] = 1;
	connections[2][3] = 1;

	int mouse_x, mouse_y;
	Uint32 mouse_buttons;
	char simulate = 0;

	float K = 10;
	float Kd = 1;
	float L0 = 200;
	float GRAVITY = 200;
	float DRAG = 0.99;

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
		SDL_PumpEvents();
		while (SDL_PollEvent(&e) != 0){
			if (e.type == SDL_QUIT){
				loop = 0;
			} else if (e.type == SDL_MOUSEMOTION){
			}
//			mouse_buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
//			if ((mouse_buttons & SDL_BUTTON_LMASK) != 0){
//				printf("%ld\n", time(NULL));
//				printf("Mouse Button 1 (left) is pressed.\n");
//				char found = 0;
//				for (int i = 0; i < NB_NODES; i++){
//					if (!nodes[i].active){
//							found = 1;
//							nodes[i].x = mouse_x;
//							nodes[i].y = mouse_y;
//							nodes[i].vx = 0;
//							nodes[i].vy = 0;
//							nodes[i].ax = 0;
//							nodes[i].ay = 0;
//							nodes[i].locked = 0;
//							nodes[i].active = 1;
//							break;
//						}
//				}
//				if (found == 0){
//					printf("all nodes are created!\n");
//				}
//			}
//			if ((mouse_buttons & SDL_BUTTON_RMASK) != 0){
//				printf("Mouse Button 2 (right) is pressed.\n");
//				char found = 0;
//				for (int i = 0; i < NB_NODES; i++){
//					if (!nodes[i].active){
//							found = 1;
//							nodes[i].x = mouse_x;
//							nodes[i].y = mouse_y;
//							nodes[i].vx = 0;
//							nodes[i].vy = 0;
//							nodes[i].ax = 0;
//							nodes[i].ay = 0;
//							nodes[i].locked = 1;
//							nodes[i].active = 1;
//							break;
//						}
//				}
//				if (found == 0){
//					printf("all nodes are created!\n");}
//				}
		}
		const Uint8* current_key_states = SDL_GetKeyboardState(NULL);
		if (current_key_states[SDL_SCANCODE_ESCAPE]){
			loop = 0;
		}
//		if (current_key_states[SDL_SCANCODE_R]){
//			for (int i = 0; i < NB_NODES; i++){
//				nodes[i].active = 0;
//			}
//		}
		if (current_key_states[SDL_SCANCODE_P]){
			simulate ^= 1;
		}

/*## UPDATING THE OBJECTS. #######################################################################*/
		float dx, dy, d, fs, fd, force;
		if (simulate){
			for (int i = 0; i < NB_NODES; i++){
				if (nodes[i].active){
					nodes[i].ax = 0;
					nodes[i].ay = GRAVITY;
				}
			}
			for (int i = 0; i < NB_NODES; i++){
				if (nodes[i].active){
					for (int j = i+1; j < NB_NODES; j++){
						if (nodes[j].active){
							if (connections[i][j]){
								dx = nodes[i].x - nodes[j].x;
								dy = nodes[i].y - nodes[j].y;
								d = sqrt(dx*dx + dy*dy);
								fs = K * (d - L0);
								fd = (dx/d * (nodes[i].vx - nodes[j].vx) + dy/d * (nodes[i].vy - nodes[j].vy)) * Kd;
								force = fs + fd;
								
								nodes[i].ax += - force * dx / d;
								nodes[i].ay += - force * dy / d;
								nodes[j].ax += + force * dx / d;
								nodes[j].ay += + force * dy / d;
							}
						}
					}
				}
			}

			for (int i = 0; i < NB_NODES; i++){
				if ((nodes[i].active) && (!nodes[i].locked)){
					nodes[i].vx += DT * nodes[i].ax;
					nodes[i].vy += DT * nodes[i].ay;
					nodes[i].vx *= DRAG;
					nodes[i].vy *= DRAG;
					nodes[i].x += DT * nodes[i].vx;
					nodes[i].y += DT * nodes[i].vy;
					char  left = nodes[i].x < 0;
					char right = nodes[i].x > WINDOW_W;
					char    up = nodes[i].y < 0;
					char  down = nodes[i].y > WINDOW_H;
					char outside = left || right || up || down;
					if (outside){
						nodes[i].vx *= -1;
						nodes[i].vy *= -1;
					}
					if  (left){ nodes[i].x = 0; }
					if (right){ nodes[i].x = WINDOW_W; }
					if    (up){ nodes[i].y = 0; }
					if  (down){ nodes[i].y = WINDOW_H; }
				}
			}
		}

/*## RENDERING ###################################################################################*/
		set_background_color(renderer, 0x333333ff);

		float c;
		for (int i = 0; i < NB_NODES; i++){
			if (nodes[i].active){
				for (int j = i+1; j < NB_NODES; j++){
					if (nodes[j].active){
						if (connections[i][j]){
							dx = nodes[i].x - nodes[j].x;
							dy = nodes[i].y - nodes[j].y;
							d = sqrt(dx*dx + dy*dy);
							c = exp(-d/300) * 255;
							SDL_SetRenderDrawColor(renderer, c, c, c, 0xff);
							SDL_RenderDrawLine(renderer, nodes[i].x, nodes[i].y, nodes[j].x, nodes[j].y);
						}
					}
				}
			}
		}
		for (int i = 0; i < NB_NODES; i++){
			if (nodes[i].active){
				float dx = mouse_x - nodes[i].x;
				float dy = mouse_y - nodes[i].y;
				float dist_to_mouse = dx*dx + dy*dy;;
				SDL_SetRenderDrawColor(renderer, 0xff, (nodes[i].locked)?0x00:0xff, (dist_to_mouse < 400)?0x00:0xff, 0xff);
				SDL_Rect rect = {nodes[i].x - 10, nodes[i].y - 10, 20, 20};
				SDL_RenderFillRect(renderer, &rect);
			}
		}

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

void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}
