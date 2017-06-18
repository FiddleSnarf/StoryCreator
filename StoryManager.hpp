#pragma once

#include <QObject>
#include "Models/SelectNodeModel.hpp"
#include "Scene/StoryScene.hpp"

class StoryManager : public QObject
{
    Q_OBJECT

public:
    StoryManager(StoryTypesNodeCollector& collector, QObject* parent = Q_NULLPTR);
    virtual ~StoryManager();

    StoryScenePtr getStoryScene();
    SelectNodeModelPtr getStoryNodeSelectModel();

    int getCountStoryNodes() const;
    bool isStoryOpen() const;

signals:
    void signalStoryStateChanged(bool state);

public slots:
    void slotCreateNewStory();
    void slotLoadStory();
    void slotCloseStory();

private:
    void initialization();

private:
    StoryTypesNodeCollector& m_typesCollector;
    StoryScenePtr m_storyScene;
    SelectNodeModelPtr m_storyNodeSelectModel;

    bool m_isStoryOpen;
};
