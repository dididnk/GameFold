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
    color_t color = get_cell(p_map, p_pos); // Couleur de la position passée en paramètre
    uint cpt = 1, fin_boucle = 0;           // Compteur
    position_t emplacement;
    emplacement.x = 0;
    emplacement.y = 0;
    uint bd_col = (*p_map).nb_columns + 2; // Nombre des colonnes

    for (uint i = 0; i < NB_MAX_CELLS; i++) // Initialisation du tableau
    {
        (*p_map).connected_component[i] = false;
    }
    (*p_map).connected_component[valid_position_to_index(p_map, p_pos)] = true; // Marque la cellule
    while (fin_boucle != 1)                                                     // Permet de parcourir toutes les cellules du tableau
    {
        fin_boucle = 1;
        for (uint i = 0; i < NB_MAX_CELLS; i++)
        {
            if ((*p_map).connected_component[i] == true)
            {
                // A chaque appel de cette fonction le paramètre du milieu (qui représente une indice) sera transformer en coordonnée qui sera dans EMPLACEMENT
                valid_index_to_position(p_map, i + 1, &emplacement);

                if (is_position_valid_and_visible(p_map, &emplacement) == true && (*p_map).cells[i + 1] == color && (*p_map).connected_component[i + 1] == false) // Voisin de droite
                {
                    (*p_map).connected_component[i + 1] = true;
                    cpt = cpt + 1;
                    fin_boucle = 0;
                }
                valid_index_to_position(p_map, i - 1, &emplacement);
                if (is_position_valid_and_visible(p_map, &emplacement) == true && (*p_map).cells[i - 1] == color && (*p_map).connected_component[i - 1] == false) // Voisin de gauche
                {
                    (*p_map).connected_component[i - 1] = true;
                    cpt = cpt + 1;
                    fin_boucle = 0;
                }
                valid_index_to_position(p_map, i - bd_col, &emplacement);
                if (is_position_valid_and_visible(p_map, &emplacement) == true && (*p_map).cells[i - bd_col] == color && (*p_map).connected_component[i - bd_col] == false) // Voisin de haut
                {
                    (*p_map).connected_component[i - bd_col] = true;
                    cpt = cpt + 1;
                    fin_boucle = 0;
                }
                valid_index_to_position(p_map, i + bd_col, &emplacement);
                if (is_position_valid_and_visible(p_map, &emplacement) == true && (*p_map).cells[i + bd_col] == color && (*p_map).connected_component[i + bd_col] == false) // Voisin de bas
                {
                    (*p_map).connected_component[i + bd_col] = true;
                    cpt = cpt + 1;
                    fin_boucle = 0;
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
    if (p_map == NULL) // Eventuelle erreur
    {
        hard_treat_error("Error !!!\n");
    }
    for (uint i = 0; i < NB_MAX_CELLS; i++) // A chaque case du tableau (chaque cellule)
    {
        if ((*p_map).connected_component[i] == true) // Pour toute cellule qui est marqué (TRUE)
        {
            (*p_map).connected_component[i] = false; // On reinitialise la cellule
            (*p_map).cells[i] = EMPTY_CELL;          //  On supprime la couleur
        }
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
// NGBAME KOYAPOLO Emmanuel :)