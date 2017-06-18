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
    bool isStoryBeLoaded() const;

signals:
    void signalStoryStateChanged(bool state);

public slots:
    void slotCreateNewStory();
    void slotLoadStory();
    void slotCloseStory();
    void slotSaveStory();
    void slotSaveAsStory();

private:
    void initialization();

private:
    StoryTypesNodeCollector& m_typesCollector;
    StoryScenePtr m_storyScene;
    SelectNodeModelPtr m_storyNodeSelectModel;

    bool m_isStoryOpen;
    bool m_isLoadedStory;   /**< Имеет ли текущая история путь к файлу для сохранения. */
};
