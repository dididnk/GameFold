#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "folt.h"

/*
* The function is_border_cell determines if a given position index corresponds to a
* position of a border cell regarding the properties of the map (mainly its dimensions). 
* CAUTION : this function is called by the initiate_map_cells function in order to 
* determine if a given cell should be initiate to BORDER_CELL or EMPTY_CELL value.
*/
bool is_border_cell(map_t *p_map, uint pos_index)
{
    if (p_map == NULL || pos_index >= NB_MAX_CELLS) // Evantuelles erreurs
    {
        hard_treat_error("Error !!!\n");
    }
    else
    {
        uint bd_ligne = (*p_map).nb_lines + 2; //Nombre de ligne de jeu
        uint bd_col = (*p_map).nb_columns + 2; //Nombre de colonne de jeu
        //                                                                                               __
        if (pos_index <= bd_col || pos_index % bd_col == 0) //On verifie la première ligne et colonne : |
        {
            return true;
        }

        if (pos_index >= (bd_col * (bd_ligne - 1)) && pos_index <= bd_col * bd_ligne) // On verifie la dernière ligne : __
        {
            return true;
        }

        for (uint i = (*p_map).nb_columns + 1; i <= bd_col * bd_ligne; i = i + bd_col) //on parcours la dernière colonne : |
        {
            if (pos_index == i)
            {
                return true;
            }
        }
        //                                                                           __
        if (pos_index > bd_col * bd_ligne) // On teste si c'est hors le tableau :   |__|
        {
            return true;
        }
    }
    return false;
}
/*
* The function is_position_valid determines if a given position which adress is given
* can belong to the map (i.e. can be part of cells array).
*/
bool is_position_valid(position_t *p_position)
{
    if (p_position == NULL) // Evantuelle erreur
    {
        hard_treat_error("Error !!!\n");
    }
    else
    {
        if (((*p_position).x + 1) * ((*p_position).y + 1) > NB_MAX_CELLS) // Vérifie si les coordonnées sont valides
        {
            return false;
        }
    }
    return true;
}
/*
* The function is_position_valid_and_visible determines if a given position which 
* adress is given belongs to the visible part of the map.
*/
bool is_position_valid_and_visible(map_t *p_map, position_t *p_position)
{
    if (p_map == NULL || p_position == NULL) // Eventuelles erreurs
    {
        hard_treat_error("Error !!!\n");
    }
    else
    {
        if ((*p_position).x > 0 && (*p_position).y > 0 &&
            is_position_valid(p_position) == true)
        {
            if ((*p_position).x <= (*p_map).nb_columns &&
                (*p_position).y <= (*p_map).nb_lines)
            {
                return true;
            }
        }
    }
    return false;
}
/*
* The function valid_position_to_index transforms a given position (supposed to be valid)
* which adress is given into the corresponding index in the cells field of the map
* at the adresse provided by p_map.
* In case of an invalid position, the result of the function is unpredictable.
*/
uint valid_position_to_index(map_t *p_map, position_t *p_position)
{
    if (p_map == NULL || p_position == NULL || is_position_valid(p_position) == false) // Eventuelles erreurs
    {
        hard_treat_error("Error !!!\n");
    }
    uint resultat;
    resultat = (*p_position).x + ((*p_position).y * ((*p_map).nb_columns + 2));
    return resultat;
}
/*
* The function valid_index_to_position transforms a given index (supposed to be valid)
* into the corresponding position which is assigned to the position which adress is given.
* In case of an invalid index, the result of the function is unpredictable.
*/
void valid_index_to_position(map_t *p_map, uint i, position_t *p_position)
{
    if (p_map == NULL || p_position == NULL) // Eventuelles erreurs
    {
        exit(EXIT_FAILURE);
    }
    uint bd_col = (*p_map).nb_columns + 2; // Colonne entière
    (*p_position).x = i % bd_col;
    (*p_position).y = (i - (*p_position).x) / bd_col;
}
/*
* The function get_cell returns the color_t of the cell at a given position which
* adress is given. In case of an invalid position, the function quit the program
* with an error message. This function can be applied to both unvisible and visible
* cell of the map.
*/
color_t get_cell(map_t *p_map, position_t *p_position)
{
    if (p_map == NULL || p_position == NULL) // Eventuelles erreurs
    {
        hard_treat_error("Error !!!\n");
    }
    return (*p_map).cells[valid_position_to_index(p_map, p_position)];
}
/*
* The function set_color_cell modifies the color_t of a visible cell at a given position which
* adress is given. In case of an invalid position or invalid color_id, the function quit
* the program with an error message.
*/
void set_color_cell(map_t *p_map, position_t *p_position, color_t color_id)
{
    if (p_map == NULL || p_position == NULL || color_id >= (*p_map).nb_colors || color_id < 0 ||
        is_position_valid_and_visible(p_map, p_position) == false) // Eventuelles erreurs
    {
        hard_treat_error("Error !!!\n");
    }
    (*p_map).cells[valid_position_to_index(p_map, p_position)] = color_id;
}
/*
* The function set_empty_cell remove any color_t of the visible cell at a given position which
* adress is given. In case of an invalid position, the function quit the program with 
* an error message.
*/
void set_empty_cell(map_t *p_map, position_t *p_position)
{
    if (p_map == NULL || p_position == NULL || is_position_valid_and_visible(p_map, p_position) == false) // Eventuelles erreurs
    {
        hard_treat_error("Error !!!\n");
    }
    (*p_map).cells[valid_position_to_index(p_map, p_position)] = EMPTY_CELL;
}
/*
* The function movement_is_allowed determines if, in a map which adress is given, starting
* from a given position (which adress is given), adding a color in a given direction
* (which adress is given) is possible. In case of at least an invalid arguement, the function 
* quit the program with an error message.
*/
bool movement_is_allowed(map_t *p_map, position_t *p_position, direction_t *p_direction)
{
    position_t emplacement;
    if (p_map == NULL || p_position == NULL || p_direction == NULL) // Eventuelles erreurs
    {
        hard_treat_error("Error !!!\n");
    }
    translate(p_position, p_direction, &emplacement);
    if (is_position_valid_and_visible(p_map, &emplacement) == true &&
        (*p_map).cells[valid_position_to_index(p_map, &emplacement)] == EMPTY_CELL)
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