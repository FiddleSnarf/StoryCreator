#include "StoryView.hpp"
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragLeaveEvent>
#include <QMimeData>

StoryView::StoryView(QWidget* parent) : QGraphicsView(parent)
{

}

StoryView::~StoryView()
{

}


/*void StoryView::dropEvent(QDropEvent* event)
{
    int a = 0;
}*/

void StoryView::dragEnterEvent(QDragEnterEvent* event)
{
    //if (event->mimeData()->hasFormat("story/node-template"))
    //    event->acceptProposedAction();

    QGraphicsView::dragEnterEvent(event);
}

void StoryView::dragLeaveEvent(QDragLeaveEvent* event)
{
    //event->accept();
    QGraphicsView::dragLeaveEvent(event);
}

void StoryView::dragMoveEvent(QDragMoveEvent* event)
{
    if (event->mimeData()->hasFormat("story/node-template"))
    {
        //event->acceptProposedAction();
        //event->setDropAction(Qt::MoveAction);
    }

    QGraphicsView::dragMoveEvent(event);
}
