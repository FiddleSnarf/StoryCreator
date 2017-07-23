#include <QApplication>
#include <QTranslator>
#include "StoryCreator.hpp"
#include "Core/ICore.hpp"
#include "Core/CoreImpl.hpp"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // TODO Переводы нужно мутить в соответствии с настройками из ConfigManager
    QTranslator translator;
    const QString appDir = QApplication::applicationDirPath();
    const QString trFile = "Translations\\story_creator_ru.qm";
    translator.load(appDir + "\\" + trFile);
    app.installTranslator(&translator);

    StoryTypesNodeCollector typesCollector;

    ICorePtr core(new CoreImpl(typesCollector));
    StoryCreator storyCreator(core);
    storyCreator.show();

    return app.exec();
}

