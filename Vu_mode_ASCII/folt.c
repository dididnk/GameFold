#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "folt.h"

/*
* Given a potential color_t c, the function returns a character corresponding to 
* the color code if this last is a valid color (less than 10); if not, it should 
* print out an error message and quit the program.
* Examples : 
*   + get_ascii_color(6) should return '6'
*   + get_ascii_color(3) should return '3'
*   + get_ascii_color(-1) should print out an error and quit the program
*   + get_ascii_color(10) should print out an error and quit the program
*/
char get_ascii_color(color_t c)
{
    char color; // Variable temporaire
    if (c >= 0 && c < 10)
    {
        color = c + '0'; // Convertion de INT en CHAR
        return color;
    }
    else
    {
        hard_treat_error("Unvalid color !!!\n");
        exit(EXIT_FAILURE);
    }
}
/*
* The function print a message depending on the state of the game. If the user
* won the game (its score is greater than the goal score), it prints out "YOU WIN !".
* If not, it prints out "GAME OVER !".
*/
void draw_ascii_awards(game_t *p_game)
{
    if (p_game == NULL) // Pointeur NULL
    {
        hard_treat_error("Error !!!\n");
        exit(EXIT_FAILURE);
    }

    if (is_wining(p_game) == true) // appelle à la fonction qui gère si l'utilisateur à gagner
    {
        printf("YOU WIN !\n");
    }
    else
    {
        printf("GAME OVER !\n");
    }
}
/*
* The function prints a message displaying the colors to come, the actual score 
* and the goal score. 
* Example: 
* Next colors : 2 3 1
* 3 / 30
* where 2, 3 and 1 are the next colors to come in appearing order
* and 3 is the actual score and 30 the goal score.
*/
void draw_ascii_menu(game_t *p_game)
{
    if (p_game == NULL) // Pointeur NULL
    {
        hard_treat_error("Error !!!\n");
    }
    printf("Next colors : ");
    for (uint i = 0; i < NB_MAX_NEXT_COLORS - 1; i++)
    {
        printf(" %u", (*p_game).next_colors[i]);
    }
    printf("\n %u / %u\n", (*p_game).score, (*(*p_game).p_map).goal); // Une double pointeur pour recupérer le score final
}
/*
* The function draw_ascii_tiles prints the game map.
* Each cell of the map is printed using three characters : a space, the color, and
* if the cell is the one corresponding to the position p_actual_pos, a '!' is added
* (otherwise a space is added).
* An empty cell is printed as a space followed by an 'X' and another space.
* Cells belonging to the border are not printed out.
* In the folowwing example, spaces are replaced by '_'
* Example:
*_X__X__X__X__X_
*_X__X__X__X__X_
*_X__2__3__1__2_
*_X__0__1__1__3_
*_X__1__X__2__0!
*_X__3__2__1__X_
*_X__X__X__X__X_
* We advise that you read carefully the documentation of the functions related to
* position_t and get_cell function.
*/
void draw_ascii_tiles(map_t *p_map, position_t *p_actual_pos)
{
    if (p_map == NULL || p_actual_pos == NULL) // Eventuelles erreurs
    {
        hard_treat_error("Error !!!\n");
    }

    position_t emplacement;                       //On crée une position
    for (uint x = 1; x <= (*p_map).nb_lines; x++) //on parcours les lignes
    {
        for (uint y = 1; y <= (*p_map).nb_columns; y++) //On pacours les colonnes
        {
            set_position(y, x, &emplacement); //
            get_cell(p_map, &emplacement);
            if (get_cell(p_map, &emplacement) == EMPTY_CELL) // Test si la cellule est vide
            {
                printf(" X");
            }
            else
            {
                printf(" %d", get_cell(p_map, &emplacement));
            }
            if (emplacement.x == (*p_actual_pos).x && emplacement.y == (*p_actual_pos).y) // Test si c'est la position actuelle
            {
                printf("!");
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n"); // Pour chaque tour on revient à la ligne
    }
}

/*
* The update_ascii_display function only prints all the graphical elements of the game.
* That is the map and the menu (colors to come, the actual score and the goal score).
* It should not imply duplicated code.
*/
void update_ascii_display(game_t *p_game)
{
    if (p_game == NULL) // POinteur NULL
    {
        hard_treat_error("Error :::\n");
    }
    draw_ascii_tiles((*p_game).p_map, (*p_game).p_actual_position); //on fait appel à la fonction DA_title
    draw_ascii_menu(p_game);                                        //On fait appel à la fonction DA_menu
}

int main(void)
{
    position_t p;
    map_t m;
    game_t g;
    init_game(&(g), &(m), &(p), "carte.xml");
    update_display(NULL, &(g));
    while (!end_of_program(&(g)))
    {
        g.p_next_direction = read_direction(NULL);
        update_game(&(g));
        update_display(NULL, &(g));
    }
    draw_awards(NULL, &(g));
    delete_display(NULL);
    return EXIT_SUCCESS;
}