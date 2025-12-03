#ifndef GAME_H
#define GAME_H

#include "Graph_lib/GUI.h"
#include "Graph_lib/Graph.h"
#include <vector>
#include <string>

// Константы для игрового поля
const int CARD_WIDTH = 70;
const int CARD_HEIGHT = 100;
const int CARD_MARGIN = 8;
const int CARDS_PER_ROW = 5;
const int TOTAL_CARDS = 20;

// Структура для хранения состояния карточки
struct CardState {
    Graph_lib::Rectangle* rect;
    Graph_lib::Text* text;
    Graph_lib::Button* button;
    bool flipped;
    bool removed;
    int value;
};

// Глобальные переменные (будут объявлены как extern)
extern Graph_lib::Window* main_window;
extern std::vector<Graph_lib::Shape*> shapes_to_delete;
extern std::vector<Graph_lib::Widget*> widgets_to_delete;
extern std::vector<CardState> cards;
extern std::vector<int> selected_cards;
extern bool can_flip;
extern int attempts_counter;
extern bool showing_initial_twos;
extern Graph_lib::Callback card_callbacks[TOTAL_CARDS]; // ДОБАВЛЕНО

// Прототипы функций - Логика игры
void handle_card_click(int card_index);
void reset_game_state();
bool should_game_end();
void check_game_completion();
void flip_card_down(int card_index);
void remove_card(int card_index);
void shuffle_remaining_cards();
void update_card_display(int card_index); // НОВАЯ ФУНКЦИЯ

// Callback функции для логики
void check_match_callback(void*);
void remove_single_two_callback(void* card_index_ptr);
void start_game_after_delay(void*);
void show_score_callback(void*);

// Callback функции для карточек
void card1_callback(Graph_lib::Address, Graph_lib::Address);
void card2_callback(Graph_lib::Address, Graph_lib::Address);
void card3_callback(Graph_lib::Address, Graph_lib::Address);
void card4_callback(Graph_lib::Address, Graph_lib::Address);
void card5_callback(Graph_lib::Address, Graph_lib::Address);
void card6_callback(Graph_lib::Address, Graph_lib::Address);
void card7_callback(Graph_lib::Address, Graph_lib::Address);
void card8_callback(Graph_lib::Address, Graph_lib::Address);
void card9_callback(Graph_lib::Address, Graph_lib::Address);
void card10_callback(Graph_lib::Address, Graph_lib::Address);
void card11_callback(Graph_lib::Address, Graph_lib::Address);
void card12_callback(Graph_lib::Address, Graph_lib::Address);
void card13_callback(Graph_lib::Address, Graph_lib::Address);
void card14_callback(Graph_lib::Address, Graph_lib::Address);
void card15_callback(Graph_lib::Address, Graph_lib::Address);
void card16_callback(Graph_lib::Address, Graph_lib::Address);
void card17_callback(Graph_lib::Address, Graph_lib::Address);
void card18_callback(Graph_lib::Address, Graph_lib::Address);
void card19_callback(Graph_lib::Address, Graph_lib::Address);
void card20_callback(Graph_lib::Address, Graph_lib::Address);

// Прототипы функций - Отрисовка и интерфейс
void exit_callback(Graph_lib::Address, Graph_lib::Address);
void back_to_menu_callback(Graph_lib::Address, Graph_lib::Address);
void authors_callback(Graph_lib::Address, Graph_lib::Address);
void new_game_callback(Graph_lib::Address, Graph_lib::Address);
void clear_window();
void show_initial_twos();
void start_game();
void create_main_menu();
void create_authors_window();

#endif // GAME_H
