#pragma once

#include <QObject>
#include "Models/SelectNodeModel.hpp"
#include "Scene/StoryScene.hpp"

typedef QSharedPointer<SelectNodeModel> SelectNodeModelPtr;
typedef QSharedPointer<StoryScene> StoryScenePtr;

class StoryManager : public QObject
{
    Q_OBJECT

public:
    explicit StoryManager(QObject* parent = 0);
    StoryScene* getStoryScene();
    SelectNodeModel* getStoryNodeSelectModel();

    // методы для работы с окном шаблонов нодов
    void setSelectListStoryNodes(const StoryCommon::SelectNodeList& listTNodes);

    // методы для работы с основной сценой
    int getCountStoryNodes() const;

signals:

public slots:

private:
    StoryScenePtr m_storyScene;
    SelectNodeModelPtr m_storyNodeSelectModel;
};
