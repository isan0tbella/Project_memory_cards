#include "game_card2.h"
#include <cstdlib>
#include <string>
#include <ctime>
#include <algorithm>

using namespace Graph_lib;

// Внешние объявления глобальных переменных
extern Graph_lib::Window* main_window;
extern std::vector<Shape*> shapes_to_delete;
extern std::vector<Widget*> widgets_to_delete;
extern std::vector<CardState> cards;
extern std::vector<int> selected_cards;
extern bool can_flip;
extern int attempts_counter;
extern bool showing_initial_twos;
extern Callback card_callbacks[TOTAL_CARDS];

// Начать игру после задержки
void start_game_after_delay(void*)
{
    if (!shapes_to_delete.empty()) {
        for (auto it = shapes_to_delete.rbegin(); it != shapes_to_delete.rend(); ++it) {
            Text* text = dynamic_cast<Text*>(*it);
            if (text && text->label().find("Подготовка") != std::string::npos) {
                main_window->detach(**it);
                delete *it;
                shapes_to_delete.erase(std::next(it).base());
                break;
            }
        }
    }
    
    for (auto& card : cards) {
        if (!card.removed && card.flipped) {
            card.flipped = false;
            card.rect->set_fill_color(Color::blue);
            card.rect->set_color(Color::dark_blue);
            card.text->set_color(Color::white);
            card.text->set_label("?");
        }
    }
    
    showing_initial_twos = false;
    can_flip = true;
    
    Text* start_text = new Text(Point(250, 550), "Игра началась!");
    start_text->set_font_size(16);
    start_text->set_color(Color::red);
    main_window->attach(*start_text);
    shapes_to_delete.push_back(start_text);
    
    main_window->redraw();
    
    Fl::add_timeout(2.0, [](void*){
        if (!shapes_to_delete.empty()) {
            for (auto it = shapes_to_delete.rbegin(); it != shapes_to_delete.rend(); ++it) {
                Text* text = dynamic_cast<Text*>(*it);
                if (text && text->label() == "Игра началась!") {
                    main_window->detach(**it);
                    delete *it;
                    shapes_to_delete.erase(std::next(it).base());
                    main_window->redraw();
                    break;
                }
            }
        }
    }, nullptr);
}

// Очистка всех виджетов и фигур из окна
void clear_window()
{
    if (!main_window) return;
    
    selected_cards.clear();
    can_flip = true;
    showing_initial_twos = false;
    
    for (auto widget : widgets_to_delete) {
        main_window->detach(*widget);
        delete widget;
    }
    widgets_to_delete.clear();
    
    for (auto shape : shapes_to_delete) {
        main_window->detach(*shape);
        delete shape;
    }
    shapes_to_delete.clear();
    
    for (auto& card : cards) {
        if (card.rect) {
            main_window->detach(*card.rect);
            delete card.rect;
        }
        if (card.text) {
            main_window->detach(*card.text);
            delete card.text;
        }
        if (card.button) {
            main_window->detach(*card.button);
            delete card.button;
        }
    }
    cards.clear();
}

// Показать начальное расположение двоек
void show_initial_twos()
{
    showing_initial_twos = true;
    
    for (size_t i = 0; i < cards.size(); ++i) {
        if (cards[i].value == 2 && !cards[i].removed) {
            cards[i].flipped = true;
            cards[i].rect->set_fill_color(Color::white);
            cards[i].rect->set_color(Color::black);
            cards[i].text->set_color(Color::black);
            cards[i].text->set_label(std::to_string(cards[i].value));
        }
    }
    
    main_window->redraw();
}

void exit_callback(Address, Address)
{
    exit(0);
}

void show_score_callback(void*)
{
    int final_score = attempts_counter;
    clear_window();
    
    std::string score_text = "Ваш счёт: " + std::to_string(final_score);
    Text* score_display = new Text(Point(250, 200), score_text);
    score_display->set_font_size(20);
    main_window->attach(*score_display);
    shapes_to_delete.push_back(score_display);
    
    Button* back_btn = new Button(Point(250, 300), 140, 35, "Вернуться в меню", back_to_menu_callback);
    main_window->attach(*back_btn);
    widgets_to_delete.push_back(back_btn);
    
    main_window->redraw();
}

void start_game()
{
    clear_window();
    reset_game_state();
    
    std::srand(std::time(0));
    
    Text* title_text = new Text(Point(200, 30), "Игровое поле - Карточки-перевёртыши");
    title_text->set_font_size(18);
    main_window->attach(*title_text);
    shapes_to_delete.push_back(title_text);
    
    Button* back_btn = new Button(Point(20, 20), 120, 35, "Выйти в меню", back_to_menu_callback);
    main_window->attach(*back_btn);
    widgets_to_delete.push_back(back_btn);
    
    int grid_width = CARDS_PER_ROW * CARD_WIDTH + (CARDS_PER_ROW - 1) * CARD_MARGIN;
    int grid_height = (TOTAL_CARDS / CARDS_PER_ROW) * CARD_HEIGHT + ((TOTAL_CARDS / CARDS_PER_ROW) - 1) * CARD_MARGIN;
    int start_x = (600 - grid_width) / 2;
    int start_y = (600 - grid_height) / 3;
    
    std::vector<int> values;
    for (int i = 1; i <= TOTAL_CARDS / 2; ++i) {
        values.push_back(i);
        values.push_back(i);
    }
    
    std::random_shuffle(values.begin(), values.end());
    
    for (int i = 0; i < TOTAL_CARDS; ++i) {
        int row = i / CARDS_PER_ROW;
        int col = i % CARDS_PER_ROW;
        
        int x = start_x + col * (CARD_WIDTH + CARD_MARGIN);
        int y = start_y + row * (CARD_HEIGHT + CARD_MARGIN);
        
        CardState card;
        
        card.rect = new Rectangle(Point(x, y), CARD_WIDTH, CARD_HEIGHT);
        card.rect->set_fill_color(Color::blue);
        card.rect->set_color(Color::dark_blue);
        main_window->attach(*card.rect);
        
        card.text = new Text(Point(x + CARD_WIDTH/2 - 8, y + CARD_HEIGHT/2 + 4), "?");
        card.text->set_color(Color::white);
        main_window->attach(*card.text);
        
        card.button = new Button(Point(x, y), CARD_WIDTH, CARD_HEIGHT, "", card_callbacks[i]);
        main_window->attach(*card.button);
        
        card.flipped = false;
        card.removed = false;
        card.value = values[i];
        
        cards.push_back(card);
    }
    
    can_flip = false;
    
    Text* prep_text = new Text(Point(200, 550), "Подготовка к игре... Показаны карточки с цифрой 2");
    prep_text->set_font_size(14);
    prep_text->set_color(Color::dark_magenta);
    main_window->attach(*prep_text);
    shapes_to_delete.push_back(prep_text);
    
    show_initial_twos();
    
    main_window->redraw();
    
    Fl::add_timeout(3.0, start_game_after_delay, nullptr);
}

void create_main_menu()
{
    clear_window();
    reset_game_state();
    
    Text* welcome_text = new Text(Point(100, 100), "Добро пожаловать в игру 'Карточки-перевёртыши'!");
    welcome_text->set_font_size(16);
    main_window->attach(*welcome_text);
    shapes_to_delete.push_back(welcome_text);
    
    int button_width = 120;
    int button_height = 35;
    int start_x = (600 - button_width) / 2;
    int start_y = 200;
    int button_spacing = 60;
    
    Button* new_game_btn = new Button(Point(start_x, start_y), button_width, button_height, "Новая игра", new_game_callback);
    Button* authors_btn = new Button(Point(start_x, start_y + button_spacing), button_width, button_height, "Авторы", authors_callback);
    Button* exit_btn = new Button(Point(start_x, start_y + 2 * button_spacing), button_width, button_height, "Выход", exit_callback);
    
    main_window->attach(*new_game_btn);
    main_window->attach(*authors_btn);
    main_window->attach(*exit_btn);
    
    widgets_to_delete.push_back(new_game_btn);
    widgets_to_delete.push_back(authors_btn);
    widgets_to_delete.push_back(exit_btn);
    
    main_window->redraw();
}

void create_authors_window()
{
    clear_window();
    
    Text* authors_text = new Text(Point(60, 200), "Авторы: Рябоконь Илья, Таисия Косицына, Алексахин Вячеслав");
    authors_text->set_font_size(16);
    main_window->attach(*authors_text);
    shapes_to_delete.push_back(authors_text);
    
    Button* back_btn = new Button(Point(240, 300), 140, 35, "Вернуться в меню", back_to_menu_callback);
    main_window->attach(*back_btn);
    widgets_to_delete.push_back(back_btn);
    
    main_window->redraw();
}

void back_to_menu_callback(Address, Address)
{
    create_main_menu();
}

void authors_callback(Address, Address)
{
    create_authors_window();
}

void new_game_callback(Address, Address) 
{
    start_game();
}

int main()
{
    try {
        Graph_lib::Window win(Point(100, 100), 600, 600, "Карточки-перевёртыши");
        main_window = &win;
        create_main_menu();
        
        return gui_main();
    }
    catch (const std::exception& e) {
        return 1;
    }
    catch (...) {
        return 2;
    }
}
