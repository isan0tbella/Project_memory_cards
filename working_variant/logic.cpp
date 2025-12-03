#include "game_card2.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace Graph_lib;

// Определение глобальных переменных
Graph_lib::Window* main_window = nullptr;
std::vector<Shape*> shapes_to_delete;
std::vector<Widget*> widgets_to_delete;
std::vector<CardState> cards;
std::vector<int> selected_cards;
bool can_flip = true;
int attempts_counter = 0;
bool showing_initial_twos = false;

// Массив callback-функций для карточек
Callback card_callbacks[TOTAL_CARDS] = {
    card1_callback, card2_callback, card3_callback, card4_callback, card5_callback,
    card6_callback, card7_callback, card8_callback, card9_callback, card10_callback,
    card11_callback, card12_callback, card13_callback, card14_callback, card15_callback,
    card16_callback, card17_callback, card18_callback, card19_callback, card20_callback
};

// Функция: Обновление отображения карточки
void update_card_display(int card_index)
{
    if (card_index < 0 || static_cast<size_t>(card_index) >= cards.size()) return;
    
    CardState& card = cards[card_index];
    
    if (card.removed) {
        // Удаленная карточка
        card.rect->set_fill_color(Color::green);
        card.rect->set_color(Color::green);
        card.text->set_color(Color::green);
        card.text->set_label("");
        if (card.button) card.button->hide();
    } else if (card.flipped) {
        // Открытая карточка
        card.rect->set_fill_color(Color::white);
        card.rect->set_color(Color::black);
        card.text->set_color(Color::black);
        card.text->set_label(std::to_string(card.value));
    } else {
        // Закрытая карточка
        card.rect->set_fill_color(Color::blue);
        card.rect->set_color(Color::dark_blue);
        card.text->set_color(Color::white);
        card.text->set_label("?");
        if (card.button) card.button->show();
    }
    
    if (main_window) main_window->redraw();
}

// Вспомогательная функция для обработки клика по карточке
void handle_card_click(int card_index)
{
    if (!can_flip || card_index >= cards.size() || cards[card_index].flipped || cards[card_index].removed) {
        return;
    }
    
    flip_card_down(card_index);
    
    if (cards[card_index].value == 2) {
        can_flip = false;
        attempts_counter++;
        int* card_index_ptr = new int(card_index);
        Fl::add_timeout(1.0, remove_single_two_callback, card_index_ptr);
    } else {
        selected_cards.push_back(card_index);
        
        if (selected_cards.size() == 2) {
            can_flip = false;
            attempts_counter++;
            Fl::add_timeout(1.0, check_match_callback, nullptr);
        }
    }
}

// Callback функции для карточек
void card1_callback(Address, Address) { handle_card_click(0); }
void card2_callback(Address, Address) { handle_card_click(1); }
void card3_callback(Address, Address) { handle_card_click(2); }
void card4_callback(Address, Address) { handle_card_click(3); }
void card5_callback(Address, Address) { handle_card_click(4); }
void card6_callback(Address, Address) { handle_card_click(5); }
void card7_callback(Address, Address) { handle_card_click(6); }
void card8_callback(Address, Address) { handle_card_click(7); }
void card9_callback(Address, Address) { handle_card_click(8); }
void card10_callback(Address, Address) { handle_card_click(9); }
void card11_callback(Address, Address) { handle_card_click(10); }
void card12_callback(Address, Address) { handle_card_click(11); }
void card13_callback(Address, Address) { handle_card_click(12); }
void card14_callback(Address, Address) { handle_card_click(13); }
void card15_callback(Address, Address) { handle_card_click(14); }
void card16_callback(Address, Address) { handle_card_click(15); }
void card17_callback(Address, Address) { handle_card_click(16); }
void card18_callback(Address, Address) { handle_card_click(17); }
void card19_callback(Address, Address) { handle_card_click(18); }
void card20_callback(Address, Address) { handle_card_click(19); }

// Функция для сброса игрового состояния
void reset_game_state()
{
    selected_cards.clear();
    can_flip = true;
    attempts_counter = 0;
    showing_initial_twos = false;
}

// Игра заканчивается, когда остались только двойки
bool should_game_end()
{
    for (const auto& card : cards) {
        if (!card.removed && card.value != 2) {
            return false;
        }
    }
    return true;
}

// Функция для проверки завершения игры
void check_game_completion()
{
    if (should_game_end()) {
        Fl::add_timeout(1.0, show_score_callback, nullptr);
    }
}

void flip_card_down(int card_index)
{
    if (card_index >= 0 && static_cast<size_t>(card_index) < cards.size() && !cards[card_index].removed) {
        cards[card_index].flipped = true;
        update_card_display(card_index);
    }
}

void remove_card(int card_index)
{
    if (card_index >= 0 && static_cast<size_t>(card_index) < cards.size()) {
        cards[card_index].removed = true;
        update_card_display(card_index);
        
        check_game_completion();
    }
}

// Функция для удаления одиночной двойки через 1 секунду
void remove_single_two_callback(void* card_index_ptr)
{
    int card_index = *static_cast<int*>(card_index_ptr);
    delete static_cast<int*>(card_index_ptr);
    
    if (card_index >= 0 && static_cast<size_t>(card_index) < cards.size()) {
        remove_card(card_index);
        
        selected_cards.clear();
        
        shuffle_remaining_cards();
        
        can_flip = true;
    }
}

void check_match_callback(void*)
{
    if (selected_cards.size() == 2) {
        int card1 = selected_cards[0];
        int card2 = selected_cards[1];
        
        if (cards[card1].value == cards[card2].value) {
            if (cards[card1].value != 2) {
                remove_card(card1);
                remove_card(card2);
            } else {
                cards[card1].flipped = false;
                cards[card2].flipped = false;
                update_card_display(card1);
                update_card_display(card2);
            }
        } else {
            cards[card1].flipped = false;
            cards[card2].flipped = false;
            update_card_display(card1);
            update_card_display(card2);
        }
        
        selected_cards.clear();
        can_flip = true;
    }
}

// Функция для перемешивания оставшихся карточек
void shuffle_remaining_cards()
{
    std::vector<int> remaining_values;
    
    for (const auto& card : cards) {
        if (!card.removed) {
            remaining_values.push_back(card.value);
        }
    }
    
    std::random_shuffle(remaining_values.begin(), remaining_values.end());
    
    size_t value_index = 0;
    for (auto& card : cards) {
        if (!card.removed) {
            card.value = remaining_values[value_index++];
            card.flipped = false;
            update_card_display(&card - &cards[0]);
        }
    }
    
    check_game_completion();
}
