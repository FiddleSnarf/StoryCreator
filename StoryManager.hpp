#pragma once

#include <QObject>
#include "Models/SelectNodeModel.hpp"
#include "Scene/StoryScene.hpp"

class StoryManager : public QObject
{
    Q_OBJECT

public:
    StoryManager(QObject* parent = Q_NULLPTR);
    virtual ~StoryManager();

    StoryScenePtr getStoryScene();
    SelectNodeModelPtr getStoryNodeSelectModel();

    // методы для работы с основной сценой
    int getCountStoryNodes() const;

signals:
    void signalStoryOpened();
    void signalStoryClosed();

public slots:
    void createNewStory();
    void loadStory();
    void closeStory();

private:
    void initialization();

private:
    StoryTypesNodeCollector m_typesCollector;
    StoryScenePtr m_storyScene;
    SelectNodeModelPtr m_storyNodeSelectModel;
};
