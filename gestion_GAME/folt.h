/*
* This file is a copy of the orginal. Modification while have no effects on the program.
*/

//Global limits
#define NB_MAX_CELLS 70      //Maximal number of map cells
#define NB_MAX_COLORS 10     //Maximal number of colors used in the game
#define NB_MAX_NEXT_COLORS 4 //Maximal number of known coming colors

//Special cells
#define EMPTY_CELL -9
#define BORDER_CELL -8

typedef unsigned int uint;

//Types
typedef struct
{
    uint x;
    uint y;
} position_t;

typedef struct
{
    int deltax;
    int deltay;
} direction_t;

//Directions definition
// Four constants are defined as direction_t *
direction_t ZERO, LEFT, RIGHT, UP, DOWN;
// Details ommited here.

typedef int color_t; //Abstract represetnation of a color

typedef struct
{
    uint nb_columns;             //Number of columns of the map
    uint nb_lines;               //Number of lines of the map
    color_t cells[NB_MAX_CELLS]; //Colors in each map cell
    uint goal;                   //Score to attend in order to win
    uint nb_colors;              //Number of colors used in the map
    uint nb_next_colors;         //Number of previewed colors in the menu
    uint min_nb_cells_in_cc;     //Number of cells needed to form a connected component
} map_t;                         //Model of the game

typedef struct
{
} display_t; //Empty type for now

typedef struct
{
    map_t *p_map;                         //Current map used
    direction_t *p_next_direction;        //Next direction to be applied
    position_t *p_actual_position;        //Actual position in the map
    uint next_colors[NB_MAX_NEXT_COLORS]; //Colors to come
    uint score;                           //Current score
} game_t;

uint get_random_integer(uint start, uint stop);
xml_t *load_xml(const char *xml_file_path);
bool get_uint_value_from_xml(const xml_t *xml_data, uint *result, const char *xpath);
void unload_xml(xml_t *xml_data);

void set_position(uint x, uint y, position_t *p_pos);
void translate(position_t *p_position, direction_t *p_dir, position_t *p_newposition);
bool are_positions_equals(position_t *p_position_1, position_t *p_position_2);
/*Error treating functions*/

void hard_treat_error(char *error_message);

/*Functions related to View*/

void update_display(display_t *p_display, game_t *p_game);
void draw_tiles(display_t *p_display, map_t *p_map, position_t *p_pos);
void draw_menu(display_t *p_display, game_t *p_game);
void draw_awards(display_t *p_display, game_t *p_game);
void delete_display(display_t *p_display);

char get_ascii_color(color_t c);
void update_ascii_display(game_t *p_game);
void draw_ascii_tiles(map_t *p_map, position_t *p_actual_pos);
void draw_ascii_menu(game_t *p_game);
void draw_ascii_awards(game_t *p_game);

/*Functions related to Model*/

//Related to game
void init_game(game_t *p_game, map_t *p_map, position_t *p_pos, char *config_filename);
void update_next_colors(game_t *p_game);
void initiate_next_colors(game_t *p_game);
bool is_wining(game_t *p_game);
void apply_move(game_t *p_game);
void update_game(game_t *p_game);
bool can_move(game_t *p_game);
bool end_of_program(game_t *p_game);

//Related to map
bool is_border_cell(map_t *p_map, uint pos_index);
void initiate_map_cells(map_t *p_map);
bool is_position_valid(position_t *p_position);
uint valid_position_to_index(map_t *p_map, position_t *p_position);
void valid_index_to_position(map_t *p_map, uint i, position_t *p_position);
color_t get_cell(map_t *p_map, position_t *p_position);
void set_color_cell(map_t *p_map, position_t *p_position, color_t color_id);
void set_empty_cell(map_t *p_map, position_t *p_position);
bool movement_is_allowed(map_t *p_map, position_t *p_pos, direction_t *p_direction);
uint compute_connected_component(map_t *p_map, position_t *p_pos);
void erase_connected_component(map_t *p_map);

/*Function related to Controller*/
direction_t *read_direction(display_t *p_display);
direction_t *read_ascii_direction();

/* Debug functions */
void print_position(position_t *p_pos);
void print_map(map_t *p_map);
void print_game(game_t *p_game);
void print_display(display_t *p_display);
void print_direction(direction_t *p_direction);
