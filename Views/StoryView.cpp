#include "StoryView.hpp"
#include <QMimeData>

StoryView::StoryView(QWidget* parent) :
    QGraphicsView(parent)
{
    setAlignment( Qt::AlignTop | Qt::AlignLeft );
}

StoryView::~StoryView()
{

}

void StoryView::wheelEvent(QWheelEvent* event)
{
    QGraphicsView::wheelEvent(event);
}
