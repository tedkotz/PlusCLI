/**
 * @file    [FileName.c]
 * @author  Ted Kotz <ted@kotz.us>
 * @version 0.1
 *
 * [Description]
 *
 */
#ifndef   MINESWP_H
#define   MINESWP_H

/* Includes ******************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
/* Defines *******************************************************************/
/* Types *********************************************************************/
/* Interfaces ****************************************************************/
/* Data **********************************************************************/
/* Functions *****************************************************************/

/**
 * A simple console mine sweeper type game
 *
 * @param argc number of arguments
 * @param argv a list of string arguments
 * @return Error Code, 0 means success
 */
int minesweep_main (int argc, char** argv);

#ifdef  __cplusplus
}
#endif

#endif // MINESWP_H
