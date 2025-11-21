#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

// Первое объявление функций
int choose_difficulty();
vector<char> generate_cards(int total_of_cards);
void board(const vector<char>& cards, const vector<bool>& revealed, int columns = 10);
void game(int total_of_cards);
void statistics(int score, int turns, int pairs_count);

// Функция выбора сложности
int choose_difficulty() {
    cout << "Игра 'Карточки-перевёртыши'\n";
    cout << "Найди все пары!\n";
    cout << "Выбери уровень сложности:\n";
    cout << "1 - Лёгкий (20 карт = 10 пар)\n";
    cout << "2 - Средний (50 карт = 25 пар)\n";
    cout << "3 - Сложный (100 карт = 50 пар)\n";
    cout << "Выбор: ";

    int choice;
    cin >> choice;

    switch (choice) {
        case 1: return 20;
        case 2: return 50;
        case 3: return 100;
        default:
            cout << "Некорректный выбор. Установлен режим по умолчанию (20 карт).\n";
            return 20;
    }
}

// Функция генерации карт
vector<char> generate_cards(int total_of_cards) {
    int pairs_count = total_of_cards / 2;
    string symbols = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    vector<char> cards;
    for (int i = 0; i < pairs_count; ++i) {
        cards.push_back(symbols[i]);
        cards.push_back(symbols[i]);
        // cards.push_back(symbols[i % symbols.size()]);  // Этот вариант подстраховачный.
        // cards.push_back(symbols[i % symbols.size()]);  // Для возможных изменений условия.
    }

    random_shuffle(cards.begin(), cards.end()); // Пишет что такая версия устаревшая, но она работает
    return cards;                               // А разбираться в новой мне лень (сорян 2 часа ночи)
}

// Функция отображения поля
void board(vector<char>& cards, vector<bool>& revealed, int columns) {
    for (int i = 0; i < cards.size(); ++i) {
        if (revealed[i]) {
            cout << " " << cards[i] << " ";
        }    
        else {
            cout << " * ";
        }
        if ((i + 1) % columns == 0) {
            cout << "\n";
        }
    } 
    cout << "\n";
}

// Функция описывающая игровой процесс
void game(int total_of_cards) {
    vector<char> cards = generate_cards(total_of_cards);
    vector<bool> revealed(total_of_cards, false);

    int pairs_count = total_of_cards / 2;
    int found_pairs = 0;
    int score = 0;
    int turns = 0;

    cout << "\nПоле готово. Номера карт: 1.." << total_of_cards << "\n";

    while (found_pairs < pairs_count) {
        board(cards, revealed, 10);

        double first, second; //double не вызовет ошибку если пользователь введет номер 1.5?
        cout << "Выберите первую карту (1-" << total_of_cards << "): ";
        cin >> first;
        cout << "Выберите вторую карту (1-" << total_of_cards << "): ";
        cin >> second;

        if (first < 1 || first > total_of_cards || second < 1 || second > total_of_cards || 
            first == second /*|| first % 1 != 0 || second % 1 != 0*/) {
            cout << "Некорректный выбор. Попробуйте снова.\n";
            continue;
        }

        first--; second--;

        if (revealed[first] || revealed[second]) {
            cout << "Одна из выбранных карт уже открыта!\n";
            continue;
        }

        revealed[first] = true;
        revealed[second] = true;

        board(cards, revealed, 10);
        turns++;

        if (cards[first] == cards[second]) {
            cout << "Пара найдена: " << cards[first] << "!\n";
            found_pairs++;
            score += 10;
        } else {
            cout << "Не совпадает.\n";
            revealed[first] = false;
            revealed[second] = false;
            score -= 2;
        }

        cout << "Очки: " << score << "\n";
        cout << "Найдено пар: " << found_pairs << " / " << pairs_count << "\n";
        cout << "------------------------------\n";
    }

    statistics(score, turns, pairs_count);
}

// Итоговая статистика
void statistics(int score, int turns, int pairs_count) {
    cout << "\nПоздравляем! Все пары найдены!\n";
    cout << "Количество ходов: " << turns << "\n";
    cout << "Итоговый счёт: " << score << "\n";

    if (score > pairs_count * 8)
        cout << "Отличный результат!\n";
    else if (score > pairs_count * 5)
        cout << "Хорошо сыграно!\n";
    else
        cout << "Можно лучше\n";
}

int main() {
    srand(static_cast<unsigned>(time(0)));
    int total_of_cards = choose_difficulty();
    game(total_of_cards);
    return 0;
}
