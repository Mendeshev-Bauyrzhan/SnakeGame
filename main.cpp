#include <QApplication>
#include "SnakeWidget.h"

// импоритрование заголовчного файла содержащий класс Змейки

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    // Создание объекта змейки с заданными параметрами

    int a[2] = {12, 12};

    SnakeWidget object(30, 30, 25, 25, 12, a, 2, 80);

    object.start_game();    // метод start_game начинает игру с заданными параметрами

    return app.exec();
}
