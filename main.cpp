#include <QApplication>
#include "StoryCreator.hpp"
#include "Core/ICore.hpp"
#include "Core/CoreImpl.hpp"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    StoryTypesNodeCollector typesCollector;

    ICorePtr core(new CoreImpl(typesCollector));
    StoryCreator storyCreator(core);
    storyCreator.show();

    return app.exec();
}

