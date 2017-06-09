#include "StoryCreator.hpp"
#include "JsonStoryProvider/JsonStoryProvider.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //StoryCreator w;
    //w.show();


    JsonStoryProvider jsonStoryProvider;
    jsonStoryProvider.startOpenStoryFileDialog();

    const StoryInfo info = jsonStoryProvider.getStoryInfo();

    jsonStoryProvider.startSaveStoryFileDialog();

    a.exit();
    return a.exec();
}

