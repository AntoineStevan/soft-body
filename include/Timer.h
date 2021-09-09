#ifndef LIB_TIMER_H
#define LIB_TIMER_H

#include <SDL2/SDL.h>

/***********************************************************************************************************************
 * @brief The Timer structure

 * This structure allows the user to easily mesure time intervals between events, e.g. the time elapsed during one frame
 * to cap the number of FPS inside a game.
 **********************************************************************************************************************/
typedef struct Timer{
	/** This field is the number of ticks from the beginning of the program, given by the SDL library, when the timer
	 * starts or is unpaused. */
	Uint32 start_ticks;
	/** This field is used internaly to freeze the timer held by the timer when on pause.*/
	Uint32 paused_ticks;

	/** This field tells whether the timer has started or is stopped.*/
	char started;
	/** This field tells whether the timer has been paused or not.*/
	char paused;
} Timer;

/***********************************************************************************************************************
 * @brief Gives a newly initialized timer.

 * @return timer (Timer*): a pointer to the timer to be initialized.
 **********************************************************************************************************************/
extern Timer Timer_init();

/***********************************************************************************************************************
 * @brief Tells whether a timer has started or is stopped.

 * @param timer (Timer): an instance of Timer to know the status of a timer, namely started/stopped.

 * @return a char that indicates whether the timer has started or is stopped.
 **********************************************************************************************************************/
extern char Timer_is_started(Timer timer);
/***********************************************************************************************************************
 * @brief Starts a timer.

 * @param timer (Timer*): a pointer to the timer to be started.
 **********************************************************************************************************************/
extern void Timer_start(Timer* timer);
/***********************************************************************************************************************
 * @brief Stops a timer.

 * @param timer (Timer*): a pointer to the timer to be stopped.
 **********************************************************************************************************************/
extern void Timer_stop(Timer* timer);

/***********************************************************************************************************************
 * @brief Tells whether a timer is paused or unpaused.

 * @param timer (Timer): an instance of Timer to know the status of a timer, namely paused/unpaused.

 * @return a char that indicates whether the timer is paused or unpaused.
 **********************************************************************************************************************/
extern char Timer_is_paused(Timer timer);
/***********************************************************************************************************************
 * @brief Pauses a timer.

 * @param timer (Timer*): a pointer to the timer to be paused.
 **********************************************************************************************************************/
extern void Timer_pause(Timer* timer);
/***********************************************************************************************************************
 * @brief Unpauses a timer.

 * @param timer (Timer*): a pointer to the timer to be unpaused.
 **********************************************************************************************************************/
extern void Timer_unpause(Timer* timer);

/***********************************************************************************************************************
 * @brief Gives the time registered inside a timer.

 * @param timer (Timer): a ninstance of Timer one wants to know the number of ticks elapsed since beginning.

 * @return an unsiqned 32 bit integer representing the number of ticks since the beginning of the timer
 * returns 0 if the timer is stopped.
 **********************************************************************************************************************/
extern Uint32 Timer_get_ticks(Timer timer);

#endif
