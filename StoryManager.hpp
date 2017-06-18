#pragma once

#include <QObject>
#include "Models/SelectNodeModel.hpp"
#include "Scene/StoryScene.hpp"

class StoryManager : public QObject
{
    Q_OBJECT

    static const QString DEF_STORY_FILE_NAME;

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
    void slotUpdateStoryName(const QString& storyName);

private:
    void initialization();
    bool saveStory();

private:
    StoryTypesNodeCollector& m_typesCollector;
    StoryScenePtr m_storyScene;
    SelectNodeModelPtr m_storyNodeSelectModel;

    StoryCommon::StoryInfo m_currentStoryInfo;
    bool m_isStoryOpen;
    bool m_isLoadedStory;   /**< Имеет ли текущая история путь к файлу для сохранения. */
};
