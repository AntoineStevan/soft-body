#include "base.h"

char init(Uint8 libs, Uint32 sdl_flags, Uint32 img_flags, Mix_Wrapper* mixer){
	char error_code = 0;
	printf("Initializing:\n");

	if (libs & GRAPHIC_LIB){
		printf("\tthe standard SDL library...");
		if (SDL_Init(sdl_flags) < 0){
			printf("\n\tSDL could not initialize : %s\n", SDL_GetError());
			fprintf(stderr, "\n\tSDL could not initialize : %s\n", SDL_GetError());
			error_code = error_code|ERROR_ON_SDL_LIBRARY_LOAD;
		} else {
			printf(" Done.\n");
		}
	}

	if (libs & IMAGE_LIB){
		printf("\tthe image library...");
		if (!(IMG_Init(img_flags) & img_flags)){
			fprintf(stderr, "\n\tSDL_image could not initialize : %s\n", IMG_GetError());
			error_code = error_code|ERROR_ON_SDL_IMAGE_LIBRARY_LOAD;
		} else {
			printf(" Done.\n");
		}
	}

	if (libs & FONT_LIB){
		printf("\tthe True Type Font library...");
		if (TTF_Init() == -1){
			fprintf(stderr, "\n\tSDL_ttf could not initialize : %s\n", TTF_GetError());
			error_code = error_code|ERROR_ON_SDL_TTF_LOAD;
		} else {
			printf(" Done.\n");
		}
	}
	
	if ((libs & AUDIO_LIB) && (mixer != NULL)){
		printf("\tthe audio library...");
		if (Mix_OpenAudio(mixer->frequency, mixer->flags, mixer->channels, mixer->sample_size) < 0){
			fprintf(stderr, "\n\tSDL_mixer could not initialize : %s\n", TTF_GetError());
			error_code = error_code|ERROR_ON_SDL_MIXER_LOAD;
		} else {
			printf(" Done.\n");
		}
	}

	printf("\n");
	return error_code;
}

char init_SDL_plus_window_and_renderer(
		Uint8 libs, Uint32 sdl_flags, Uint32 img_flags, 
		SDL_Window** dst_window, SDL_Renderer** dst_renderer,
		int w, int h, Uint32 window_flags, Mix_Wrapper* mixer){
	char error_code = init(libs, sdl_flags, img_flags, mixer);

	printf("Creating the window and the renderer...");
	if (SDL_CreateWindowAndRenderer(w, h, window_flags, dst_window, dst_renderer)){
		fprintf(stderr, "\n\tCould not create window and renderer : %s\n", SDL_GetError());
		error_code = error_code|ERROR_ON_WINDOW_OR_RENDERER_CREATION;
	} else {
		printf(" Done.\n");
	}

	return error_code;
}

char create_window(SDL_Window** dst_window, char* title, int x, int y, int w, int h, Uint32 window_flags){
	char error_code = 0;
	printf("Creating window...");
	*dst_window = SDL_CreateWindow(title, x, y, w, h, window_flags);
	if (*dst_window == NULL){
		fprintf(stderr, "\nMain window could not be created : %s\n", SDL_GetError());
		error_code = ERROR_ON_WINDOW_CREATION;
	} else {
		printf(" Done.\n");
	}

	return error_code;
}

char get_surface_from_window(SDL_Window* src_window, SDL_Surface** dst_surface){
	char error_code = 0;
	printf("Creating surface from window...");
	*dst_surface = SDL_GetWindowSurface(src_window);
	if (*dst_surface == NULL){
		fprintf(stderr, "\nMain surface could not be retrieved from main window : %s\n", SDL_GetError());
		error_code = ERROR_ON_MAIN_SURFACE_CREATION;
	} else {
		printf(" Done.\n");
	}

	return error_code;
}

char get_renderer_from_window(SDL_Window* src_window, SDL_Renderer** dst_renderer, Uint32 rend_flags){
	char error_code = 0;
	printf("Creating renderer from window...");
	*dst_renderer = SDL_CreateRenderer(src_window, -1, rend_flags);
	if (*dst_renderer == NULL){
		fprintf(stderr, "\nMain surface could not be retrieved from main window : %s\n", SDL_GetError());
		error_code = ERROR_ON_RENDERER_CREATION;
	} else {
		printf(" Done.\n");
	}

	return error_code;
}

char get_font_from_ttf_file(TTF_Font** dst_font, char* font_path, int font_size){
	char error_code = 0;
	printf("Collecting font...");
	*dst_font = TTF_OpenFont(font_path, font_size);
	if (*dst_font == NULL){
		fprintf(stderr, "\nFont could not be loaded from file %s : %s\n", font_path, SDL_GetError());
		error_code = ERROR_ON_FONT_LOAD;
	} else {
		printf(" Done.\n");
	}

	return error_code;
}

void set_background_color(SDL_Renderer* renderer, Uint32 bg_color){
	SDL_SetRenderDrawColor(renderer, (bg_color>>24)&0xff, (bg_color>>16)&0xff, (bg_color>>8)&0xff, bg_color&0xff);
	SDL_RenderClear(renderer);
}

SDL_Surface* load_bmp(char* path, SDL_Surface* gSurface){
	SDL_Surface* loaded_surface = SDL_LoadBMP(path);
	if (loaded_surface == NULL){
		fprintf(stderr, "Could not load the surface at %s : %s\n", path, SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_Surface* optimized_surface = SDL_ConvertSurface(loaded_surface, gSurface->format, 0);
	if (optimized_surface == NULL){
		fprintf(stderr, "Unable to optimize the surface at %s : %s\n", path, SDL_GetError());
	}

	SDL_FreeSurface(loaded_surface);
	return optimized_surface;
}

SDL_Surface* load_image(char* path, SDL_Surface* gSurface){
	SDL_Surface* loaded_surface = IMG_Load(path);
	if (loaded_surface == NULL){
		fprintf(stderr, "Could not load the surface at %s : %s\n", path, SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_Surface* optimized_surface = SDL_ConvertSurface(loaded_surface, gSurface->format, 0);
	if (optimized_surface == NULL){
		fprintf(stderr, "Unable to optimize the surface at %s : %s\n", path, SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_FreeSurface(loaded_surface);
	return optimized_surface;
}

char load_texture(SDL_Renderer* renderer, SDL_Texture** dst_texture, char* path, Uint32 color_key){
	char error_code = 0;
	SDL_Surface* loaded_surface = IMG_Load(path);
	if (loaded_surface == NULL){
		fprintf(stderr, "Could not load the surface at %s: %s\n", path, SDL_GetError());
		error_code = 1;
	} else {
		SDL_SetColorKey(
			loaded_surface, SDL_TRUE,
			SDL_MapRGB(loaded_surface->format, (color_key>>16)&0xff, (color_key>>8)&0xff, color_key&0xff)
		);
		*dst_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
		if (*dst_texture == NULL){
			fprintf(stderr, "Unable to create texture from surface at %s: %s\n", path, SDL_GetError());
			error_code = 1;
		}
		SDL_FreeSurface(loaded_surface);
	}

	return error_code;
}

void destroy_texture(SDL_Texture** texture){
	SDL_DestroyTexture(*texture);
	*texture = NULL;
}

void close_window(SDL_Window** window){
	printf("Destroying the window...");
	SDL_DestroyWindow(*window);
	*window = NULL;
	printf(" Done.\n");
}

void close_renderer(SDL_Renderer** renderer){
	printf("Destroying the renderer...");
	SDL_DestroyRenderer(*renderer);
	*renderer = NULL;
	printf(" Done.\n");
}

void close_font(TTF_Font** font){
	printf("Closing font...");
	TTF_CloseFont(*font);
	*font = NULL;
	printf(" Done.\n");
}

void quit(Uint8 libs){
	printf("\nQuitting:\n");
	if (libs & AUDIO_LIB){
		printf("\tthe audio library...");
		Mix_Quit();
		printf(" Done.\n");
	}
	if (libs & FONT_LIB){
		printf("\tthe True Type Font library...");
		TTF_Quit();
		printf(" Done.\n");
	}
	if (libs & IMAGE_LIB){
		printf("\tthe image library...");
		IMG_Quit();
		printf(" Done.\n");
	}
	if (libs & GRAPHIC_LIB){
		printf("\tthe standard SDL library...");
		SDL_Quit();
		printf(" Done.\n");
	}
}

/*SDL_BlitSurface(gCurrentSurface, NULL, gSurface, NULL);*/
/*SDL_BlitScaled(gCurrentSurface, NULL, gSurface, &stretched_rect);*/
/*SDL_UpdateWindowSurface(gWindow);*/
