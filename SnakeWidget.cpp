#include <QtWidgets>
#include "SnakeWidget.h"
#include <random>

// Конструктор класса змейки создает задает виджету окна нужные параметры и инициализирует переменные:

SnakeWidget::SnakeWidget(int cell_width, int cell_length, int pf_width, int pf_length, int snake_size, int start_position[2], int fruit_max_amount, int time_sleep)
{
    // Установка параметров окна:

    resize(cell_width * pf_width, cell_length * pf_length);   // установка нужного размера окна
    setWindowTitle("Snake Game");                             // установка названия заголовка окна
    setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);          // делает недоступным окно к изменению размера

    // Инициализация переменных внутри класса:

    this->cell_width = cell_width;
    this->cell_length = cell_length;
    this->pf_width = pf_width;
    this->pf_length = pf_length;
    this->fruit_max_amount = fruit_max_amount;
    this->snake_size = snake_size;
    this->time_sleep = time_sleep;

    total_game_score = 0;   // начальное значение общего счёта равно нулю

    pf_layout = new QGridLayout;

    // Добавление в массив координат исходного положения змейки:

    positions.resize(snake_size);

    for (int i = 0; i < snake_size; i++)
    {
        positions[i].append(start_position[1] + i);      // координаты по оси x не меняются так как змейка расположена вертикально
        positions[i].append(start_position[0]);          // координаты по оси y уменьшаются так как змея "спущена" вниз
    }

    fruits.resize(fruit_max_amount);    // массив координат фруктов может вместить не больше заданного порога
    f = 0;                              // начальное количество фруктов на игровом поле равно нулю
}

// -----------------------------------------------------------------------------------------------------------------------------------//

// метод start_game() начинает игру с заданными параметрами:

void SnakeWidget::start_game()
{
    // Создание игрового поля:

    for (int i = 0; i < pf_length; ++i)
    {
        for (int j = 0; j < pf_width; ++j)
        {
            pf_layout->addWidget(createCell(1, 1, 1), i, j);
        }
    }

    pf_layout->setSpacing(0);
    pf_layout->setContentsMargins(0, 0, 0, 0);
    // установка нулевых внутренних и внешних отступов менеджера компановки

    setLayout(pf_layout);

    // Отображение змейки в начале игры

    for (it = positions.begin(); it != positions.end(); ++it)
    {
        pf_layout->addWidget(createCell(1, 155, 1), (*it)[0], (*it)[1]);
    }

    // Механика игры:

    is_alive = true; // для начала цикла игры нужно сделать змейку живой

    this->show();

    while (is_alive)    // продолжать до тех пор пока змейка жива
    {
        /* !!!
         *
         * Возможные исправления:
         *
         *   - Вместо использования QFrame и QGridLayout для создания игрового поля
         *   можно использовать средства Qt для работы с графикой что сильно увеличит FPS игры
         *   - Можно внедрить многопоточность для повышения FPS
         *   - Можно улучшить обработку событий с клавиатуры добавив очередь (queue) обработки событий с клавиатуры
         * !!! */


        switch (move_direction)
        {

        case 1:     // Движение ВВЕРХ

            move(-1, 0);

        break;

        case 2:     // Движение ВНИЗ

            move(1, 0);

        break;

        case 3:     // Движение ВПРАВО

            move(0, 1);

        break;

        case 4:     // Движение ВЛЕВО

            move(0, -1);

        break;

        };

        // Если змейка столкнулась об края игрового поля:

        if (positions[0][0] == 0 || positions[0][0] == pf_length  - 1 || positions[0][1] == 0 || positions[0][1] == pf_width - 1)
        {
            is_alive = false;   // то змейка считается мёртвой и игра заканчивается
        }

        // Если змейка замкнулась на себе то игра прекращается:

        for (it = positions.begin() + 1; it != positions.end(); ++it)
        {
            if (*it == positions[0])
            {
                    pf_layout->addWidget(createCell(1, 155, 1), buf[0], buf[1]);

                    is_alive = false;
                    break;
            }
        }

        /* Если на игровом поле нет фруктов (игра только началась или
        змейка сьела все фрукты что были на игровом поле): */

        if (f == 0)
        {
            srand(time(0));
            fruits.resize(fruit_max_amount);

            for (int i = 0; i < fruit_max_amount; ++i)
            {
                    // Добавление фрукта с случайными координатами:

                    fruits[i].push_back((int) rand()%(pf_length-1));
                    fruits[i].push_back((int) rand()%(pf_width-1));

                    // Предохранение от генерации фрукта на змейке:

                    while (true)
                    {
                        if (positions.contains(fruits[i]))
                        {
                            fruits[i][0] = (int) rand()%(pf_length-1);
                            fruits[i][1] = (int) rand()%(pf_width-1);
                        }
                        else
                        {
                            break;
                        }
                    }

                    // Отображение сгенерированного фрукта:

                    temp = createCell(255, 0, 0);
                    pf_layout->addWidget(temp, fruits[i][0], fruits[i][1]);
                    temp->show();

                    f++;    // количество фруктов увеличивется на один
            }
        }

        // Если змейка наткнулась на фрукт:

        for (int i = 0; i < fruits.count(); ++i)
        {
            if (positions[0] == fruits[i])
            {
                    positions.resize(positions.count() + 1);

                    // массив координат змейки увеличиваем на один

                    // Добавляем новую ячейку к хвостовой части змейки:

                    positions[positions.count() - 1] = buf;

                    temp = createCell(1, 155, 1);
                    pf_layout->addWidget(temp, buf[0], buf[1]);
                    temp->show();

                    f--;
                    // так как змейка сьела один фрукт то количество фруктов на поле становиться меньше

                    fruits.remove(i);   // удаляем фрукт из массива фруктов

                    total_game_score++; // увеличиваем счёт игры на один

                    break;
            }
        }

        QThread::msleep(time_sleep);    // задержка цикла игры (уменьшение скорости движения змейки)

        QApplication::processEvents();
    }

    qDebug()<<"Game Over! Your score = "<<total_game_score;
}

// -----------------------------------------------------------------------------------------------------------------------------------//

// Метод move() выполняет передвижение змейки на один шаг в соответствии с заданным направлением движения

void SnakeWidget::move(int i, int j)
{
    // Добавление одной ячейки к головной части и удаление хвостовой части:

    temp = createCell(1, 1, 1);

    pf_layout->addWidget(temp, positions[positions.count() - 1][0], positions[positions.count() - 1][1]);

    temp->show();

    temp = createCell(1, 155, 1);

    pf_layout->addWidget(temp, positions[0][0] + i, positions[0][1] + j);

    temp->show();

    buf = positions[positions.count() - 1];

    // Изменение массива:

    for (rit = positions.rbegin(); rit != positions.rend() - 1; ++rit)
    {
        *rit = *(rit + 1);
    }

    positions[0][0] += i;
    positions[0][1] += j;
}

// -----------------------------------------------------------------------------------------------------------------------------------//

// метод createCell() создаёт ячейку игрового поля:

QFrame* SnakeWidget::createCell(int r, int g, int b)
{
    QFrame* cell = new QFrame;  // создание объекта(указателя) рамки QFrame для создания ячейки
    QPalette pal;               // создание объекта QPalette для задания фонового цвета рамки(ячейки)

    // Задание чёрного цвета палитре:

    pal.setColor(cell->backgroundRole(), QColor(r, g, b));

    cell->setPalette(pal);
    cell->setAutoFillBackground(true);
    cell->setFixedSize(cell_width, cell_length);

    return cell;
}

// -----------------------------------------------------------------------------------------------------------------------------------//

// Метод для обработки событий клавиатуры:

void SnakeWidget::keyPressEvent(QKeyEvent *pe)
{
    switch (pe->key())
    {

    case Qt::Key_W:     // если была нажата клавиша - "W"

        if (move_direction != 2 && move_direction != 1) // и если змейка на движеться назад или вперёд
        {
            move_direction = 1;     // то направление движения змейки меняется на - ВПЕРЁД
        }

    break;

    case Qt::Key_S:     // если была нажата клавиша - "S"

        if (move_direction != 1 && move_direction != 2)     // и если змейка не двигалась вперёд или назад
        {
            move_direction = 2;     // то направление движения змейки меняется на - НАЗАД
        }

    break;

    case Qt::Key_D:     // если была нажата клавиша - "D"

        if (move_direction != 4 && move_direction != 3)     // и если змейка не двигалась вправо или влево
        {

            move_direction = 3;     // то направление движение меняется на - ВПРАВО
        }

    break;

    case Qt::Key_A:     // если была нажата клавиша - "A"


        if (move_direction != 3 && move_direction != 4)     // и если змейка не двигалась вправо или влево
        {

            move_direction = 4;     // то направление движение меняется на - ВПРАВО
        }

    break;

    }
}

