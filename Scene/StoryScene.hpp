#pragma once
#include <QGraphicsScene>
#include <QGraphicsRectItem>

class StoryScene : public QGraphicsScene
{
public:
	explicit StoryScene(QObject* parent = Q_NULLPTR);
	~StoryScene();

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent* event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;
    void dropEvent(QGraphicsSceneDragDropEvent* event) override;

private:
    QGraphicsRectItem* m_boundRect;
};
