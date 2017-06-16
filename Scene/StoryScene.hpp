#pragma once

#include <QGraphicsScene>
#include <QList>
#include <set>

#include "Items/StoryNodeItemImpl.hpp"
#include "Common/StoryCommon.hpp"
#include "Common/StoryTypesNodeCollector.hpp"

class StoryNodeItem;

class StoryScene : public QGraphicsScene
{
    Q_OBJECT

private:
    static const int NODE_Z_DEPTH = 2;
    static const int LINK_Z_DEPTH = 1;

public:
    StoryScene(QObject* parent = Q_NULLPTR);
	~StoryScene();

    int nodeCount() const;
    StoryNodeItemList getStoryNodeList() const;

    void setTypesNodeCollector(const StoryTypesNodeCollector& collector);
    void initStoryInfo(const StoryCommon::StoryInfo& storyInfo);
    int getFreeID() const;

public slots:
    void slotClearScene();

signals:
    void signalItemSelected();

protected:
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override;
    virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent* event) override;
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;
    virtual void dropEvent(QGraphicsSceneDragDropEvent* event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;


private:
    bool addEmptyStoryNode(const QString& nodeType, const QIcon& icon, const QPointF& pos);
    bool addStoryNode(const StoryNode& nodeInfo, const QPointF& pos);

private:
    StoryTypesNodeCollector m_storyTypesNodeCollector;   /**< Класс хранящий информацию о возможных типах нодов. */
    std::set<int> m_idSet;                               /**< Множество выданных id-шников. */
};

typedef QSharedPointer<StoryScene> StoryScenePtr;
