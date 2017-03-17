#pragma once

#include <QGraphicsScene>
#include <QList>

class StoryNodeItem;

class StoryScene : public QGraphicsScene
{
private:
    static const int NODE_Z_DEPTH = 2;
    static const int LINK_Z_DEPTH = 1;

public:
	explicit StoryScene(QObject* parent = Q_NULLPTR);
	~StoryScene();

    int nodeCount() const;
    QList<StoryNodeItem*> getStoryNodeList() const;

public slots:
    void clearScene();

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent* event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;
    void dropEvent(QGraphicsSceneDragDropEvent* event) override;


private:
    void addStoryNode(const QString& nodeType, const QIcon& icon, const QPointF& pos);
    int getFreeID() const;

private:
};
