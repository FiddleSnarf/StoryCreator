#pragma once

#include <QGraphicsScene>
#include <QList>
#include <set>

#include "Items/StoryNodeItemImpl.hpp"
#include "Common/StoryCommon.hpp"

class StoryNodeItem;

class StoryScene : public QGraphicsScene
{
private:
    static const int NODE_Z_DEPTH = 2;
    static const int LINK_Z_DEPTH = 1;

    static const int LIMIT_ID = 1000;

public:
	explicit StoryScene(QObject* parent = Q_NULLPTR);
	~StoryScene();

    int nodeCount() const;
    StoryNodeItemList getStoryNodeList() const;

    void initStoryInfo(const StoryCommon::StoryInfo& storyInfo);

public slots:
    void clearScene();

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent* event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;
    void dropEvent(QGraphicsSceneDragDropEvent* event) override;


private:
    bool addEmptyStoryNode(const QString& nodeType, const QIcon& icon, const QPointF& pos);
    bool addStoryNode(const StoryNode& node);
    int getFreeID() const;

private:
    std::set<int> m_idSet;  /**< Множество выданных id-шников. */
};

typedef QSharedPointer<StoryScene> StoryScenePtr;
