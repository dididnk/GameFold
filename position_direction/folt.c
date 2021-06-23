#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "folt.h"

/*
* The function are_positions_equals returns true if the position_t at the
* adresses p_pos and p_pos2 are equal field by field; false otherwise (even in case of invalid pointer).
*/
bool are_positions_equals(position_t *p_pos, position_t *p_pos2)
{    
    if (p_pos == NULL && p_pos2 == NULL) // Cas des pointeurs NULL
    {
        return true;
    }
    if ((p_pos != NULL && p_pos2 == NULL) || (p_pos == NULL && p_pos2 != NULL)) // Cas l'un des pointeurs est NULL
    {
        return false;
    }
    return ((*p_pos).x == (*p_pos2).x && (*p_pos).y == (*p_pos2).y); // Egalité Normale (avec des pointeurs valides)
}
/*
* The function set_position modify fields x and y of the position_t which
* adresse is given by p_pos according to the values of the parameters x and y.
* Invalid pointer is forbidden since a result as to be set.
*/
void set_position(unsigned int x, unsigned int y, position_t *p_pos)
{
    if (p_pos == NULL) //Test si le pointeur est NULL
    {
        hard_treat_error("Adress can not be NULL !!!\n"); // Message d'erreur et arrrêt de programme
    }
    else
    {
        (*p_pos).x = x; //Modifie l'élément X de la structure Position_t
        (*p_pos).y = y; //Modifie l'élément Y de la structure Position_t
    }
}
/*
* The function are_directions_equals returns true if the direction_t at the
* adresses p_dir and p_dir2 are equal field by field; false otherwise.
*/
bool are_directions_equals(direction_t *p_dir, direction_t *p_dir2)
{
    if (p_dir == NULL && p_dir2 == NULL) // Cas des pointeurs NULL
    {
        return true;
    }
    if ((p_dir != NULL && p_dir2 == NULL) || (p_dir == NULL && p_dir2 != NULL)) // Cas l'un des pointeurs est NULL
    {
        return false;
    }
    return ((*p_dir).deltax == (*p_dir2).deltax && (*p_dir).deltay == (*p_dir2).deltay); // Test Normal
}
/*
* The function translate computes the position_t resulting from the translation
* of the position_t at the adress p_position by the direction_t at the adress p_dir
* and stores the result in the position_t at the adress p_newposition.
* Invalid pointer are forbidden since information have to be read and a result as to be set.
*/
void translate(position_t *p_position, direction_t *p_dir, position_t *p_newposition)
{
    if (p_position == NULL || p_dir == NULL || p_newposition == NULL) //Test si le pointeur est NULL
    {
        hard_treat_error("Adress can not be NULL !!!\n");
    }
    else
    {
        int newp_x; // Variable  pour le stokage d'une nouvelleposition en X
        int newp_y; // Variable  pour le stokage d'une nouvelleposition en Y

        if ((*p_dir).deltax >= 0 && (*p_dir).deltay >= 0) // Si la direction a des coordonnées positifs
        {

            (*p_newposition).x = (*p_position).x + (*p_dir).deltax;
            (*p_newposition).y = (*p_position).y + (*p_dir).deltay;
            if ((*p_newposition).x < (*p_position).x || (*p_newposition).y < (*p_position).y) // Test de a+b= c ---> c >a
            {
                hard_treat_error("Unvalid  new_position!!!\n");
            }
        }
        else if ((*p_dir).deltax < 0 && (*p_dir).deltay < 0) // Si la direction a des coordonnées négatifs
        {
            newp_x = (*p_position).x + (*p_dir).deltax;
            newp_y = (*p_position).y + (*p_dir).deltay;
            if (newp_x < 0 || newp_y < 0) // Test si les nouvelles positions sont inferieures à 0
            {
                hard_treat_error("Unvalid new_position!!!\n");
            }
            (*p_newposition).x = (*p_position).x - (0 - (*p_dir).deltax); // Conversion de INT en UNSIGNED INT et calcul on peut dire aussi
            (*p_newposition).y = (*p_position).y - (0 - (*p_dir).deltay); // que c'est la "Récuperation de la valeur absolue"
            if ((*p_newposition).x > (*p_position).x || (*p_newposition).y > (*p_position).y) // Test de a-b= c ---> c >a
            {
                hard_treat_error("Unvalid  new_position!!!\n");
            }
        }
        else
        {
            // Traitement Cas par cas càd si l'une des coordonnées et soit >= 0 ou < 0
            if ((*p_dir).deltax < 0)
            {
                (*p_newposition).x = (*p_position).x - (0 - (*p_dir).deltax);
                if ((*p_newposition).x > (*p_position).x)
                {
                    hard_treat_error("Unvalid  new_position!!!\n");
                }
            }
            else
            {
                (*p_newposition).x = (*p_position).x + (*p_dir).deltax;
                if ((*p_newposition).x < (*p_position).x)
                {
                    hard_treat_error("Unvalid  new_position!!!\n");
                }
            }
            if ((*p_dir).deltay < 0)
            {
                (*p_newposition).y = (*p_position).y - (0 - (*p_dir).deltay);
                if ((*p_newposition).y > (*p_position).y)
                {
                    hard_treat_error("Unvalid  new_position!!!\n");
                }
            }
            else
            {
                (*p_newposition).y = (*p_position).y + (*p_dir).deltay;
                if ((*p_newposition).y < (*p_position).y)
                {
                    hard_treat_error("Unvalid  new_position!!!\n");
                }
            }
        }
    }
}

int main(void)
{
    position_t p = (position_t){15, 2};
    position_t p2 = (position_t){10, 4};
    position_t p3;
    position_t p4;
    position_t p5;
    direction_t d = (direction_t){5, -2};
    direction_t d2 = (direction_t){-5, 2};
    direction_t d3 = (direction_t){5, -2};
    set_position(15, 2, &(p3));
    translate(&(p2), &(d), &(p4));
    translate(&(p), &(d2), &(p5));
    print_position(&(p));
    print_position(&(p2));
    print_position(&(p3));
    print_position(&(p4));
    print_position(&(p5));
    print_direction(&(d));
    print_direction(&(d2));
    print_direction(&(d3));
    if (!are_positions_equals(&(p), &(p2)))
    {
        printf("p and p2 are different.\n");
    }
    if (are_positions_equals(&(p), &(p3)))
    {
        printf("p and p3 are equal.\n");
    }
    if (are_positions_equals(&(p), &(p4)))
    {
        printf("p and p4 are equal.\n");
    }
    if (are_positions_equals(&(p2), &(p5)))
    {
        printf("p2 and p5 are equal.\n");
    }
    if (!are_directions_equals(&(d), &(d2)))
    {
        printf("d and d2 are different.\n");
    }
    if (are_directions_equals(&(d), &(d3)))
    {
        printf("d and d3 are equal.\n");
    }
    return EXIT_SUCCESS;
}