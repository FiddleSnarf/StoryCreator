#ifndef STORYMANAGER_H
#define STORYMANAGER_H

#include <QObject>
#include "Models/SelectNodeModel.hpp"

#include "Scene/StoryScene.hpp"

class StoryManager : public QObject
{
    Q_OBJECT

public:
    explicit StoryManager(QObject* parent = 0);

    StoryScene& getStoryScene();
    SelectNodeModel& getStoryNodeSelectModel();

    void setSelectListStoryNodes(const StoryCommon::SelectNodeList& listTNodes);

signals:

public slots:

private:
    StoryScene m_storyScene;
    SelectNodeModel m_storyNodeSelectModel;
};

#endif // STORYMANAGER_H
