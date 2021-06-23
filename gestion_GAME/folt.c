#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "folt.h"
#include <time.h>

/*
* The function update_next_colors select a new random color in the defined range
* of colors of the game (which adress is provided) and add it to the the next_colors.
*/
void update_next_colors(game_t *p_game)
{
    if (p_game == NULL || (*p_game).p_actual_position == NULL || (*p_game).p_map == NULL) // Eventuelle erreur
    {
        hard_treat_error("Error !!!\n");
    }
    else
    {
        uint start = 0; // Debut de l'intervalle qui va permettre en suite d'avoir un chiffre(ici considéré comme une couleur) aleatoire
        uint end = (*(*p_game).p_map).nb_colors - 1; // Cette variable permet de simplifier l'écriture de code
        for (uint i = 0; i < (*(*p_game).p_map).nb_next_colors - 1; i++)
        {
            (*p_game).next_colors[i] = (*p_game).next_colors[i + 1]; // On fait un deplacement de couple de la cellule précedente à l'actuelle
        }
        (*p_game).next_colors[(*(*p_game).p_map).nb_next_colors - 1] = get_random_integer(start, end); // On affecte la couleur aléatoire
    }
}
/*
* The function is_wining determines if a game (which adress is provided) ended 
* on a victory or not. It is typically used in the draw_awards function to adapt
* the ending message.
*/
bool is_wining(game_t *p_game)
{
    if (p_game == NULL || (*p_game).p_map == NULL) // Eventuelle erreur
    {
        hard_treat_error("Error !!!\n");
    }
    if ((*p_game).score < (*(*p_game).p_map).goal) // Test si le joueur à depassé ou atteint le score cible
    {
        return false;
    }
    return true;
}
/*
* The function apply_move applies a movement corresponding to the direction at 
* adress p_next_direction to the current state of the game (which adress is provided).
* The function assumes that the next direction is valid and has alreay been checked.
*/
void apply_move(game_t *p_game)
{
    if (p_game == NULL || (*p_game).p_map == NULL) // Eventuelle erreur
    {
        hard_treat_error("Error !!!\n");
    }
    // Création d'une nouvelle positionn
    position_t emplacement;
    // Initialisation
    emplacement.x = (*(*p_game).p_actual_position).x;
    emplacement.y = (*(*p_game).p_actual_position).y;
    translate(&emplacement, (*p_game).p_next_direction, (*p_game).p_actual_position);
    set_color_cell((*p_game).p_map, (*p_game).p_actual_position, (*p_game).next_colors[0]);
    update_next_colors(p_game);
}
/*
* The function can_move determines if any direction is available in the current
* state of a game (which adress is provided) ended. It is typically used to 
* determine the end of the game.
*/
bool can_move(game_t *p_game)
{
    if (p_game == NULL || (*p_game).p_map == NULL) // Eventuelle erreur
    {
        hard_treat_error("Error !!!\n");
    }
    // Test pour chaque constante qui permet de faire un mouvement d'une cellule à une autre
    if (movement_is_allowed((*p_game).p_map, (*p_game).p_actual_position, ZERO) == true ||
        movement_is_allowed((*p_game).p_map, (*p_game).p_actual_position, LEFT) == true ||
        movement_is_allowed((*p_game).p_map, (*p_game).p_actual_position, RIGHT) == true ||
        movement_is_allowed((*p_game).p_map, (*p_game).p_actual_position, UP) == true ||
        movement_is_allowed((*p_game).p_map, (*p_game).p_actual_position, DOWN) == true)
    {
        return true;
    }
    return false;
}
/*
* The function end_of_program determines if a game should be stop considering its
* current state.
*/
bool end_of_program(game_t *p_game)
{
    if (p_game == NULL) // Eventuelle erreur
    {
        hard_treat_error("Error !!!\n");
    }
    if (can_move(p_game) == false || is_wining(p_game) == true) // test si on ne peut plus faire de mouvement ou si le joueur à gagné
    {
        return true;
    }

    return false;
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