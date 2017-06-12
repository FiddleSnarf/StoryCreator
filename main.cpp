#include <QApplication>
#include "StoryCreator.hpp"
#include "Core/ICore.hpp"
#include "Core/CoreImpl.hpp"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    ICorePtr core(new CoreImpl());
    StoryCreator storyCreator(core);
    storyCreator.show();

    return app.exec();
}

