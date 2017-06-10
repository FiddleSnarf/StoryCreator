#pragma once

#include <QObject>
#include "Models/SelectNodeModel.hpp"
#include "Scene/StoryScene.hpp"

class StoryManager : public QObject
{
    Q_OBJECT

public:
    explicit StoryManager(QObject* parent = nullptr);
    StoryScenePtr getStoryScene();
    SelectNodeModelPtr getStoryNodeSelectModel();

    // методы для работы с окном шаблонов нодов
    //void setSelectListStoryNodes(const StoryCommon::SelectNodeList& listTNodes);

    // методы для работы с основной сценой
    int getCountStoryNodes() const;

signals:

public slots:

private:
    StoryScenePtr m_storyScene;
    SelectNodeModelPtr m_storyNodeSelectModel;
};
