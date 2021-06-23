#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include "folt.h"

uint get_random_integer(uint start, uint stop)
{
    return start + rand() % (stop - start);
}

/*Vector and Position usefool functions*/

void set_position(uint x, uint y, position_t *p_pos);
void translate(position_t *p_position, direction_t *p_dir, position_t *p_newposition);
bool are_directions_equals(direction_t *p_dir, direction_t *p_dir2);
bool are_positions_equals(position_t *p_pos, position_t *p_pos2);

/*Error treating functions*/

void hard_treat_error(char *error_message)
{
    fprintf(stderr, "%s\n", error_message);
    exit(EXIT_FAILURE);
}

/*Functions related to UI View*/

void init_display(display_t *p_display, uint width, uint height)
{
    if (p_display == NULL)
    {
        hard_treat_error("init_display should not be called on null display adress\n");
    }
    (*p_display).background_color = MLV_COLOR_BLACK;
    (*p_display).current_position_color = MLV_COLOR_WHITE;
    (*p_display).text_color = MLV_COLOR_WHITE;
    (*p_display).colors[0] = MLV_COLOR_RED;
    (*p_display).colors[1] = MLV_COLOR_BLUE;
    (*p_display).colors[2] = MLV_COLOR_GREEN;
    (*p_display).colors[3] = MLV_COLOR_YELLOW;
    (*p_display).colors[4] = MLV_COLOR_PINK;
    (*p_display).colors[5] = MLV_COLOR_ORANGE;
    (*p_display).colors[6] = MLV_COLOR_ALICE_BLUE;
    (*p_display).colors[7] = MLV_COLOR_LINEN;
    (*p_display).colors[8] = MLV_COLOR_YELLOW_GREEN;
    (*p_display).colors[9] = MLV_COLOR_BISQUE1;
    if (SCREEN_HEIGHT / (height + 2) < SCREEN_WIDTH / width)
    {
        (*p_display).tile_size = SCREEN_HEIGHT / (height + 2);
    }
    else
    {
        (*p_display).tile_size = SCREEN_WIDTH / width;
    }
    (*p_display).width = width * (*p_display).tile_size;
    (*p_display).height = (2 + height) * (*p_display).tile_size;
    MLV_create_window("UBFolt", "UBFolt", (*p_display).width, (*p_display).height);
    MLV_update_window();
}

void update_display(display_t *p_display, game_t *p_game)
{
    if (p_display == NULL)
    {
        return update_ascii_display(p_game);
    }
    if (p_game == NULL)
    {
        hard_treat_error("update_display should not be called on null game adress\n");
    }
    map_t *p_map = (*p_game).p_map;
    MLV_clear_window(MLV_COLOR_BLACK);
    draw_tiles(p_display, p_map, (*p_game).p_actual_position);
    draw_menu(p_display, p_game);
    MLV_actualise_window();
}

void draw_tiles(display_t *p_display, map_t *p_map, position_t *p_actu_pos)
{
    if (p_display == NULL)
    {
        return draw_ascii_tiles(p_map, p_actu_pos);
    }
    if (p_map == NULL)
    {
        hard_treat_error("draw_tiles should not be called on null map adress\n");
    }
    if (p_actu_pos == NULL)
    {
        hard_treat_error("draw_tiles should not be called on null position adress\n");
    }
    uint tile_size = (*p_display).tile_size;
    position_t pos;
    position_t *p_pos = &(pos);
    for (uint x = 1; x < (*p_map).nb_columns + 1; x = x + 1)
    {
        for (uint y = 1; y < (*p_map).nb_lines + 1; y = y + 1)
        {
            set_position(x, y, p_pos);
            color_t i = get_cell(p_map, p_pos);
            if ((i != BORDER_CELL) && (i != EMPTY_CELL))
            {
                MLV_draw_filled_rectangle((x - 1) * tile_size, (y - 1) * tile_size, tile_size, tile_size, (*p_display).colors[i]);
            }
            if ((*p_map).connected_component[valid_position_to_index(p_map, p_pos)])
            {
                MLV_draw_line((x - 1) * tile_size, (y - 1) * tile_size, x * tile_size, y * tile_size, MLV_COLOR_PURPLE);
                MLV_draw_line(x * tile_size, (y - 1) * tile_size, (x - 1) * tile_size, y * tile_size, MLV_COLOR_PURPLE);
            }
        }
    }
    draw_current_position(p_display, p_actu_pos);
}

void draw_current_position(display_t *p_display, position_t *p_pos)
{
    if (p_display == NULL)
    {
        hard_treat_error("draw_current_position should not be called on null display adress\n");
    }
    if (p_pos == NULL)
    {
        hard_treat_error("draw_current_position should not be called on null position adress\n");
    }
    uint tile_size = (*p_display).tile_size;
    MLV_draw_filled_circle(((*p_pos).x - 1 + .5) * tile_size, ((*p_pos).y - 1 + .5) * tile_size, tile_size / 4.0, (*p_display).current_position_color);
}

void draw_menu(display_t *p_display, game_t *p_game)
{
    if (p_display == NULL)
    {
        return draw_ascii_menu(p_game);
    }
    if (p_game == NULL)
    {
        hard_treat_error("draw_menu should not be called on null game adress\n");
    }
    map_t *p_map = (*p_game).p_map;
    uint tile_size = (*p_display).tile_size;
    for (uint i = 0; i < (*p_map).nb_next_colors; i = i + 1)
    {
        MLV_draw_filled_rectangle(i * tile_size, ((*p_display).height) - tile_size, tile_size, tile_size, (*p_display).colors[(*p_game).next_colors[i]]);
    }
    MLV_draw_text((1 + (*p_map).nb_next_colors) * tile_size, ((*p_display).height) - tile_size, "%d / %d", (*p_display).text_color, (*p_game).score, (*p_map).goal);
}

void draw_awards(display_t *p_display, game_t *p_game)
{
    if (p_display == NULL)
    {
        return draw_ascii_awards(p_game);
    }
    if (p_game == NULL)
    {
        hard_treat_error("draw_awards should not be called on null game adress\n");
    }
    int width_text, height_text;
    char *text;
    if (is_wining(p_game))
    {
        text = "YOU WIN !";
    }
    else
    {
        text = "GAME OVER !";
    }
    MLV_get_size_of_text(text, &(width_text), &(height_text));
    int positionX = ((*p_display).width - width_text) / 2;
    int positionY = (*p_display).height - 2 * (*p_display).tile_size;
    MLV_draw_text(positionX, positionY, text, (*p_display).text_color);
    MLV_update_window();
    MLV_wait_seconds(2);
}

void delete_display(display_t *p_display)
{
    if (p_display != NULL)
    {
        MLV_free_window();
    }
}

/*Functions related to Model*/

void update_next_colors(game_t *p_game);

void initiate_next_colors(game_t *p_game)
{
    if (p_game == NULL)
    {
        hard_treat_error("initiate_next_colors should not be called on null game instance\n");
    }
    map_t *p_map = (*p_game).p_map;
    if (p_map == NULL)
    {
        hard_treat_error("initiate_next_colors should not be called on game instance containing a NULL map\n");
    }
    for (uint i = 0; i < (*p_map).nb_next_colors; i++)
    {
        (*p_game).next_colors[i] = get_random_integer(0, (*p_map).nb_colors - 1);
    }
}

bool is_border_cell(map_t *p_map, uint pos);

void initiate_map_cells(map_t *p_map)
{
    for (uint i = 0; i < NB_MAX_CELLS; i = i + 1)
    {
        if (is_border_cell(p_map, i))
        {
            (*p_map).cells[i] = BORDER_CELL;
        }
        else
        {
            (*p_map).cells[i] = EMPTY_CELL;
        }
        (*p_map).connected_component[i] = false;
    }
}

xml_t *load_xml(const char *xml_file_path)
{
    return MLV_load_xml(xml_file_path);
}

bool get_uint_value_from_xml(const xml_t *xml_data, uint *p_result, const char *xpath)
{
    int v = 0;
    if ((MLV_get_integer_value_from_xml(xml_data, &(v), xpath) != MLV_XML_NONE) || (v < 0))
    {
        return false;
    }
    (*p_result) = (uint)v;
    return true;
}

void unload_xml(xml_t *xml_data)
{
    MLV_free_xml(xml_data);
}

void init_game(game_t *p_game, map_t *p_map, position_t *p_pos, char *config_filename)
{
    if (p_game == NULL)
    {
        hard_treat_error("init_game should not be called on null game instance\n");
    }
    if (p_map == NULL)
    {
        hard_treat_error("init_game should not be called on null map adress\n");
    }
    if (p_pos == NULL)
    {
        hard_treat_error("init_game should not be called on null position adress\n");
    }
    xml_t *p_xml = load_xml(config_filename);
    if (p_xml == NULL)
    {
        hard_treat_error("File does not seems to be in xml format.");
    }
    uint v = 0;
    if ((get_uint_value_from_xml(p_xml, &(v), "/level/nb_colors") == false) || (v > NB_MAX_COLORS))
    {
        hard_treat_error("Unvalid XML file : /level/nb_colors is not defined or out of [0,NB_MAX_COLORS-1].");
    }
    (*p_map).nb_colors = v;
    if ((get_uint_value_from_xml(p_xml, &(v), "/level/nb_next_colors") == false) || (v > NB_MAX_NEXT_COLORS))
    {
        hard_treat_error("Unvalid XML file : /level/nb_next_colors is not defined or out of [0,NB_MAX_NEXT_COLORS-1]");
    }
    (*p_map).nb_next_colors = v;
    if ((get_uint_value_from_xml(p_xml, &(v), "/level/min_nb_cells_in_cc") == false) || (v < 2))
    {
        hard_treat_error("Unvalid XML file : /level/min_nb_cells_in_cc is not defined or > 2.");
    }
    (*p_map).min_nb_cells_in_cc = v;
    if ((get_uint_value_from_xml(p_xml, &(v), "/level/width") == false) || (v < 1))
    {
        hard_treat_error("Unvalid XML file : /level/width is not defined or not an unsigned integer greater than one.");
    }
    (*p_map).nb_columns = v;
    if ((get_uint_value_from_xml(p_xml, &(v), "/level/height") == false) || (v < 1))
    {
        hard_treat_error("Unvalid XML file : /level/height is not defined or not an unsigned integer greater than one.");
    }
    (*p_map).nb_lines = v;
    if ((*p_map).nb_columns * (*p_map).nb_lines > NB_MAX_CELLS)
    {
        hard_treat_error("The size of the map is bigger than expected");
    }
    if (get_uint_value_from_xml(p_xml, &(v), "/level/goal") == false)
    {
        hard_treat_error("Unvalid XML file : /level/goal element is not defined or not an unsigned integer.");
    }
    (*p_map).goal = v;
    if ((get_uint_value_from_xml(p_xml, &(v), "/level/starting_position/x") == false) || (v > (*p_map).nb_columns))
    {
        hard_treat_error("Unvalid XML file : /level/starting_position/x element is not defined or out of bound.");
    }
    (*p_pos).x = v;
    if ((get_uint_value_from_xml(p_xml, &(v), "/level/starting_position/y") == false) || (v > (*p_map).nb_lines))
    {
        hard_treat_error("Unvalid XML file : /level/starting_position/y element is not defined or out of bound.");
    }
    (*p_pos).y = v;
    if ((get_uint_value_from_xml(p_xml, &(v), "/level/starting_position/color_index") == false) || (v > (*p_map).nb_colors))
    {
        hard_treat_error("Unvalid XML file : /level/starting_position/color_index element is not defined or out of bound.");
    }
    unload_xml(p_xml);
    (*p_game).p_actual_position = p_pos;
    initiate_map_cells(p_map);
    (*p_map).cells[valid_position_to_index(p_map, p_pos)] = v;
    (*p_game).p_map = p_map;
    (*p_game).score = 0;
    initiate_next_colors(p_game);
    (*p_game).p_next_direction = ZERO;
}

bool is_position_valid_and_visible(map_t *p_map, position_t *p_position);

bool is_position_valid(position_t *p_position);

uint valid_position_to_index(map_t *p_map, position_t *p_position);

void valid_index_to_position(map_t *p_map, uint i, position_t *p_position);

color_t get_cell(map_t *p_map, position_t *p_position);

void set_color_cell(map_t *p_map, position_t *p_position, color_t color_id);

void set_empty_cell(map_t *p_map, position_t *p_position);

bool is_wining(game_t *p_game);

bool movement_is_allowed(map_t *p_map, position_t *p_pos, direction_t *p_direction);

void apply_move(game_t *p_game);

void update_game(game_t *p_game)
{
    if (p_game == NULL)
    {
        hard_treat_error("update_game should not be called on null game instance\n");
    }
    map_t *p_map = (*p_game).p_map;
    if (are_directions_equals((*p_game).p_next_direction, ZERO) == false)
    {
        if (movement_is_allowed(p_map, (*p_game).p_actual_position, (*p_game).p_next_direction))
        {
            apply_move(p_game);
            uint cc_s = compute_connected_component(p_map, (*p_game).p_actual_position);
            if (cc_s >= (*p_map).min_nb_cells_in_cc)
            {
                color_t color_id = (*p_map).cells[valid_position_to_index(p_map, (*p_game).p_actual_position)];
                erase_connected_component(p_map);
                set_color_cell(p_map, (*p_game).p_actual_position, color_id);
                (*p_game).score = (*p_game).score + (cc_s - 1);
            }
        }
    }
    (*p_game).p_next_direction = ZERO;
}

bool can_move(game_t *p_game);
bool end_of_program(game_t *p_game);

direction_t *read_direction(display_t *p_display)
{
    if (p_display == NULL)
    {
        return read_ascii_direction();
    }
    MLV_Keyboard_button key_sym;
    MLV_wait_keyboard(&(key_sym), NULL, NULL);
    if (key_sym == MLV_KEYBOARD_LEFT)
    {
        return LEFT;
    }
    if (key_sym == MLV_KEYBOARD_RIGHT)
    {
        return RIGHT;
    }
    if (key_sym == MLV_KEYBOARD_UP)
    {
        return UP;
    }
    if (key_sym == MLV_KEYBOARD_DOWN)
    {
        return DOWN;
    }
    return ZERO;
}

uint compute_connected_component(map_t *p_map, position_t *p_pos);
void erase_connected_component(map_t *p_map);
char get_ascii_color(color_t c);
void draw_ascii_tiles(map_t *p_map, position_t *p_actual_pos);
void draw_ascii_menu(game_t *p_game);
void draw_ascii_awards(game_t *p_game);

void update_ascii_display(game_t *p_game)
{
    if (p_game == NULL)
    {
        hard_treat_error("update_ascii_display should not be called on null game instance\n");
    }
    map_t *p_map = (*p_game).p_map;
    draw_ascii_tiles(p_map, (*p_game).p_actual_position);
    draw_ascii_menu(p_game);
}

void print_separator()
{
    for (uint i = 0; i < 59; i = i + 1)
    {
        printf("-");
    }
    printf("\n");
}

void usage()
{
    print_separator();
    print_separator();
    printf("Rules:\n > Current position is denoted by a '!'.\n");
    printf(" > Use the arrow to move in left, right, down and up directions.\n");
    printf(" > Colors are represented as positive integers from 0 to 9.\n");
    printf(" > Empty cells are represented with an 'X'.\n");
    print_separator();
    print_separator();
}

int main(void)
{
    srand(time(NULL));
    position_t p;
    map_t m;
    display_t d;
    game_t g;
    init_game(&(g), &(m), &(p), "carte.xml");
    print_game(&(g));
    init_display(&(d), m.nb_columns, m.nb_lines);
    update_display(&(d), &(g));
    while (!end_of_program(&(g)))
    {
        g.p_next_direction = read_direction(&(d));
        update_game(&(g));
        update_display(&(d), &(g));
    }
    draw_awards(&(d), &(g));
    delete_display(&(d));
    return EXIT_SUCCESS;
}
