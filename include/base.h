#ifndef LIB_BASE_H
#define LIB_BASE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

/**
 * @brief Use this flag in the definition of sdl_flags to load and unload the standard library.
 */
#define GRAPHIC_LIB 1<<0
/**
 * @brief Use this flag in the definition of sdl_flags to load and unload the image library.
 */
#define IMAGE_LIB   1<<1
/**
 * @brief Use this flag in the definition of sdl_flags to load and unload the TTF library.
 */
#define FONT_LIB    1<<2
/**
 * @brief Use this flag in the definition of sdl_flags to load and unload the mixer library.
 */
#define AUDIO_LIB   1<<3

/**
 * @brief This flag is returned when an error occured whilst loading the SDL library.
 */
#define ERROR_ON_SDL_LIBRARY_LOAD            1<<0
/**
 * @brief This flag is returned when an error occured whilst loading the image part of the SDL.
 */
#define ERROR_ON_SDL_IMAGE_LIBRARY_LOAD      1<<1
/**
 * @brief This flag is returned when an error occured whilst loading the font library.
 */
#define ERROR_ON_SDL_TTF_LOAD                1<<2
/**
 * @brief This flag is returned when an error occured whilst loading a font file.
 */
#define ERROR_ON_FONT_LOAD                   1<<3
/**
 * @brief This flag is returned when an error occured whilst  loading the audio part of the SDL.
 */
#define ERROR_ON_SDL_MIXER_LOAD              1<<4
/**
 * @brief This flag is returned when an error occured whilst creating the window and the renderer.
 */
#define ERROR_ON_WINDOW_OR_RENDERER_CREATION 1<<0

/**
 * @brief This flag is returned when an error occured whilst creating the window.
 */
#define ERROR_ON_WINDOW_CREATION       1<<1
/**
 * @brief This flag is returned when an error occured whilst getting the main surface from the window.
 */
#define ERROR_ON_MAIN_SURFACE_CREATION 1<<2
/**
 * @brief This flag is returned when an error occured whilst creating the renderer from the window.
 */
#define ERROR_ON_RENDERER_CREATION     1<<3

/**
 * @brief A simple structure that allows the use of sound.
 *
 * Give an instance of the Mix_Wrapper structure to the init function to initialize the sound in SDL.
 *****************************/
typedef struct Mix_Wrapper {
	int frequency;
	Uint16 flags;
	int channels;
	int sample_size;
} Mix_Wrapper;

/**
 * @brief Libraries initialization based on flags.

 * Initializes the SDL library and requested sub libraries. If something goes wrong, the error code is returned.

 * @param libs all the libraries to be loaded encoded in one integer, i.e. by combining hard coded flags, e.g.
 * GRAPHIC_LIB | IMAGE_LIB to load the standard SDL.library and the image part of SDL.
 * @param sdl_flags the flags regarding the SDL library.
 * @param img_flags the flags regarding the SDL_image library, not used if IMAGE_LIB is not in the libs argument.
 * @param mixer a wrapper that stores the useful parameters to initialize the audio extension of the SDL library.

 * @return a value which tells whether the libraries could load without any error, non zero if some errors occured.
**************************************************************************************************/
extern char init(Uint8 libs, Uint32 sdl_flags, Uint32 img_flags, Mix_Wrapper* mixer);

/**
 * @brief Libraries initialization based on flags and window + renderer creation.

 * The following function initializes the SDL library with given flags and gives back a window and a renderer
 * with a certain size and flags. Error code is returned whenever an error occur during library initializations or
 * window/renderer creation.

 * @param libs all the libraries to be loaded encoded in one integer.
 * @param sdl_flags the flags regarding the SDL library.
 * @param img_flags the flags regarding the SDL_image library.
 * @param dst_window this variable will store a pointer to the window.
 * @param dst_renderer this variable will store a pointer to the renderer attached to the above window.
 * @param w the width of the window, in pixels.
 * @param h the height of the window, in pixels.
 * @param window_flags the flags regarding the window itself and the associated renderer.
 * @param mixer a wrapper that stores the useful parameters to initialize the audio extension of the SDL library.

 * @return the error code, non zero if an error occured.
**************************************************************************************************/
extern char init_SDL_plus_window_and_renderer(
		Uint8 libs, Uint32 sdl_flags, Uint32 img_flags, 
		SDL_Window** dst_window, SDL_Renderer** dst_renderer,
		int w, int h, Uint32 window_flags, Mix_Wrapper* mixer);
/**
 * @brief Window creation.

 * After the SDL library is initialized, one would like to have a window to work with. The following function
 * creates a window, i.e. by giving it a name, an initial location on the screen, a size and special flags, and make
 * sure everything went well. Error code is returned if something occured during window creation.

 * @param dst_window this variable will store a pointer to the window.
 * @param title the title of the window, that is the string of characters printed onto the window.
 * @param x the x coordinate where the window is created on the computer screen.
 * @param y the y coordinate where the window is created on the computer screen.
 * @param w the width of the window, in pixels.
 * @param h the height of the window, in pixels.
 * @param  the flags to be applied to the future window.

 * @return the error code, non zero if an error occured.
**************************************************************************************************/
extern char create_window(SDL_Window** dst_window, char* title, int x, int y, int w, int h, Uint32 window_flags);
/**
 * @brief Main surface creation.

 * When one has a brand new window, one would like to have the associated surface to display sprites, draw shapes
 * or textures ! The following function gives the surface associated with a given window. It takes care of possible
 * errors by returning an appropriate error code.

 * @param src_window this is the window one has created beforehand, e.g. with create_window from base.c
 * @param dst_surface this variable will hold  a pointer to the surface.

 * @return the error code, non zero if an error occured.
**************************************************************************************************/
extern char get_surface_from_window(SDL_Window* src_window, SDL_Surface** dst_surface);
/**
 * @brief Renderer creation.

 * When one has a brand new window, one would like to have the associated renderer to display sprites, draw
 * shapes or textures ! The following function gives the renderer associated with a given window. It takes care of
 * possible errors by returning an appropriate error code.

 * @param src_window this is the window one has created beforehand, e.g. with create_window from base.c
 * @param dst_renderer this variable will hold  a pointer to the renderer.
 * @param rend_flags the flags to apply onto the renderer.

 * @return the error code, non zero if an error occured.
**************************************************************************************************/
extern char get_renderer_from_window(SDL_Window* src_window, SDL_Renderer** dst_renderer, Uint32 rend_flags);
/**
 * @brief Font creation from a font (.ttf) file.

 * One could want to load fonts in order to print strings on the screen The following function The following
 * function loads a font file in the root directory and store it inside the program. It takes care of possible errors
 * by returning an appropriate error code.

 * @param dst_font this variable will store a pointer to the loaded font.
 * @param font_path the location of the ttf font file.
 * @param font_size the font size.

 * @return the error code, non zero if an error occured.
**************************************************************************************************/
extern char get_font_from_ttf_file(TTF_Font** dst_font, char* font_path, int font_size);

/**
 * @brief Background color.

 * Sets the background color of the window to whatever color is given as a parameter.

 * @param renderer the renderer to help rendering objects on the window.
 * @param bg_color the color in following hexadecimal format color~0xRRGGBBAA
**************************************************************************************************/
extern void set_background_color(SDL_Renderer* renderer, Uint32 bg_color);

/**
 * @brief .bmp image loading.

 * Now that on has initialized the SDL library, a window and its associated surface, one way to use this graphic
 * library is the manipultate surface. The function below enables the user to load image (.bmp format) in an easy and
 * efficient manner. The image needs to be under the root directory and error are handled safely.
 * Future use of the loaded image is made easier and more efficient by using some SDL image format conversions.

 * @param path to load a .bmp image, one need to give the path to access the above mentionned image
 * @param gSurface the surface used for optimization of sub surfaces

 * @return loaded_surface the loaded surface located at given path.
**************************************************************************************************/
extern SDL_Surface* load_bmp(char* path, SDL_Surface* gSurface);
/**
 * @brief Any image loading.

 * Now that on has initialized the SDL library, a window and its associated surface, one way to use this graphic
 * library is the manipultate surface. The function below enables the user to load image (any image format supported by
 * the SDL_image library, including BMP, PNG, JPEG...) in an easy and efficient manner. The image needs to be under the
 * root directory and error are handled safely.
 * Future use of the loaded image is made easier and more efficient by using some SDL image format conversions.

 * @param path to load a .bmp image, one need to give the path to access the above mentionned image
 * @param gSurface the surface used for optimization of sub surfaces

 * @return loaded_surface the loaded surface located at given path.
**************************************************************************************************/
extern SDL_Surface* load_image(char* path, SDL_Surface* gSurface);

/**
 * @brief Texture loading.

 * In order to do fast rendering, one could want to use textures. The following functions takes care of the
 * loading and the errors.

 * @param renderer the renderer used to optimize of sub surfaces.
 * @param dst_texture this variable will store a pointer to the loaded and optimized texture.
 * @param path to load a texture one need to give the path to access the above mentionned image.
 * @param color_key the color to be removed from the texture.

 * @return the error code, non zero if an error occured.
**************************************************************************************************/
extern char load_texture(SDL_Renderer* renderer, SDL_Texture** dst_texture, char* path, Uint32 color_key);
/**
 * @brief Texture destruction.

 * After finishing to use the SDL library, one needs not to forget to free the memory.

 * @param texture a texture to be destroyed.
**************************************************************************************************/
extern void destroy_texture(SDL_Texture** texture);

/**
 * @brief Close the window to free the memory.

 * After finishing to use the SDL library, one needs not to forget to close the window.

 * @param window the window, used all over the program, to be destroyed.
**************************************************************************************************/
extern void close_window(SDL_Window** window);
/**
 * @brief Close the renderer to free the memory.

 * After finishing to use the SDL library, one needs not to forget to close the renderer.

 * @param renderer the renderer, used all over the program, to be destroyed.
**************************************************************************************************/
extern void close_renderer(SDL_Renderer** renderer);
/**
 * @brief Close the font to free the memory.

 * After finishing to use the SDL font library, one needs not to forget to close the font.

 * @param font the font, used all over the program, to be closed.
**************************************************************************************************/
extern void close_font(TTF_Font** font);
/**
 * @brief Quit every loaded SDL library.

 * @param libs all the libraries to be closed encoded in one integer (see the init function for more infos).
**************************************************************************************************/
extern void quit(Uint8 libs);

#endif
