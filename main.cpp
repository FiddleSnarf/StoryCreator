#include "StoryCreator.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StoryCreator w;
    w.show();

    return a.exec();
}
