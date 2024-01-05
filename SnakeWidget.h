#pragma once

#include <QWidget>
#include <QList>

class QFrame;
class QGridLayout;

class SnakeWidget : public QWidget
{
private:

    // Игровое поле:

    int cell_width;     // Ширина одной ячейки игрового поля
    int cell_length;    // Длина одной ячейки игрового поля
    int pf_width;       // Количество ячеек игрового поля в ширину
    int pf_length;      // Количество ячеек игрового поля в длину

    QGridLayout* pf_layout;
    // менеджер табличной компановки для создания сетки игрового поля

    QFrame* temp;   // буферная переменная для хранения объекта ячейки игрового поля

    // Змейка:

    QList<QList<int>>::iterator it;
    QList<QList<int>>::reverse_iterator rit;
    QList<QList<int>> positions;                // массив координат змейки
    QList<int> buf;
    // буферная переменная для хранения предыдущей координаты хвостовой части змейки

    int snake_size;                     // размер змейки в ячейках

    bool is_alive;                      // переменная фиксирующая смерть змейки
    int move_direction;                 // переменная определяющая направление движения змейки
    int time_sleep;                     // задержка обновления окна в миллисекундах

    /*

    1 - движение вверх (Up);
    2 - движение вниз (Down);
    3 - движение вправо (Right)
    4 - движение влево (Left)

    */

    // Фрукты:

    QVector<QVector<int>> fruits;       // вектор для хранения координат фруктов

    int fruit_max_amount;               // максимальное количество фруктов на игровом поле
    int total_game_score;               // общий счёт игры

    int f;                              // количество фруктов на игрвом поле

public:

    // Конструктор класса змейки создает задает виджету окна нужные параметры и инициализирует переменные:

    SnakeWidget (int cell_width, int cell_length, int pf_width, int pf_length, int snake_size, int start_position[2], int fruit_max_amount, int time_sleep);

    void start_game();                         // метод start_game() начинает игру с заданными параметрами
    QFrame* createCell(int r, int g, int b);   // метод createCell() создаёт ячейку игрового поля
    void keyPressEvent(QKeyEvent* pe);         // метод для обработки событий клавиатуры
    void move(int i, int j);
};
