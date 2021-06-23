#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "folt.h"

/*
* The function compute_connected_component computes the maximal combination (that
* is a connected component of cells of similar color) involving the current position.
* The function returns the size (i.e. the number of cells) of the connected component.
*/
uint compute_connected_component(map_t *p_map, position_t *p_pos)
{
    if (p_map == NULL || p_pos == NULL || is_position_valid_and_visible(p_map, p_pos) == false)
    {
        hard_treat_error("Error !!!\n");
    }
    color_t c = get_cell(p_map, p_pos); // Couleur de la position passée en paramètre
    uint cpt = 1;                       // Compteur
    position_t emplacement;
    uint bd_col = (*p_map).nb_columns + 2; // Nombre des colonnes

    for (uint i = 0; i < NB_MAX_CELLS; i++) // Initialisation du tableau
    {
        (*p_map).connected_component[i] = false;
    }
    (*p_map).connected_component[valid_position_to_index(p_map, p_pos)] = true; // Marque la cellule
    for (uint x = 0; x < (*p_map).nb_lines; x++)                                /* GB: ce n'est pas plus simple d'utiliser un indice i pour parcourir le tableau p_map->cells 
                                                   (dont chaque case sorrespond à une case dans le tableau des composantes connexes) en utilisant 
                                                   validIndexToPosition pour convertir en position?
                                                   Car vous allez avoir besoin, pour chaque case i qui a la même couleur de la position courante,
                                                   de tester si parmi les 4 cases à coté il y en a qui sont dans la position courante. Dans ce cas,
                                                   si la case i n'est pas dans la composante connexe, il faut l'ajouter */
    {
        for (uint y = 0; y < (*p_map).nb_columns; y++)
        {
            emplacement.x = x;
            emplacement.y = y;
            uint index = valid_position_to_index(p_map, &emplacement); // récupère l'indice de chaque case
            color_t color = get_cell(p_map, &emplacement);             // Couleur de la position chaque cellule
            if ((*p_map).connected_component[index] == true)
            {
                if ((*p_map).cells[index + 1] == color &&
                    (*p_map).connected_component[index + 1] == false)
                {
                    (*p_map).connected_component[index + 1] = true;
                    cpt = cpt + 1;
                }
                if ((*p_map).cells[index - 1] == color &&
                    (*p_map).connected_component[index - 1] == false)
                {
                    (*p_map).connected_component[index - 1] = true;
                    cpt = cpt + 1;
                }
                if ((*p_map).cells[index - bd_col] == color &&
                    (*p_map).connected_component[index - bd_col] == false)
                {
                    (*p_map).connected_component[index - bd_col] = true;
                    cpt = cpt + 1;
                }
                if ((*p_map).cells[index + bd_col] == color &&
                    (*p_map).connected_component[index + bd_col] == false)
                {
                    (*p_map).connected_component[index + bd_col] = true;
                    cpt = cpt + 1;
                }
            }
        }
    }
    return cpt;
}
/*
* The function erase_connected_component replace any cell of the connected component
* by an empty cell.
*/
void erase_connected_component(map_t *p_map)
{
    if (p_map == NULL)
    {
        hard_treat_error("Error !!!\n");
    }
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