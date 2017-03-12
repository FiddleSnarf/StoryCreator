#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QIcon>
#include <Qpen>
#include "StoryScene.hpp"

StoryScene::StoryScene(QObject* parent) :
    QGraphicsScene(parent),
    m_boundRect(nullptr)
{

}

StoryScene::~StoryScene()
{

}

void StoryScene::dragEnterEvent(QGraphicsSceneDragDropEvent* event)
{
    if (event->mimeData()->hasFormat("story/node-template"))
        event->acceptProposedAction();
}

void StoryScene::dragLeaveEvent(QGraphicsSceneDragDropEvent* event)
{
    event->acceptProposedAction();
}

void StoryScene::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
    Q_UNUSED(event);
    /*if (event->mimeData()->hasFormat("story/node-template"))
    {
        //event->acceptProposedAction();
        //event->setDropAction(Qt::MoveAction);
        //event->setAccepted(true);
        //event->setPossibleActions(Qt::MoveAction);
        //event->setProposedAction(Qt::MoveAction);
        //->setDropAction(Qt::MoveAction);
        //event->acceptProposedAction();
    }*/
}

void StoryScene::dropEvent(QGraphicsSceneDragDropEvent* event)
{
    const QMimeData* data = event->mimeData();
    if (!data->hasFormat("story/node-template"))
        return;

    QByteArray encodedData = data->data("story/node-template");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    while (!stream.atEnd())
    {
        QIcon icon;
        QString typeNode;
        stream >> typeNode >> icon;

        QPen pen(Qt::red);
        pen.setWidth(5);
        QPen pen2(Qt::black);
        pen2.setWidth(2);

        QPointF p = event->scenePos();
        addRect(p.x(), p.y() , 50, 50, pen);
        if(m_boundRect)
        {
            m_boundRect->setRect(sceneRect());
        }else
        {
            m_boundRect = addRect(sceneRect(), pen2);
        }
    }
}
