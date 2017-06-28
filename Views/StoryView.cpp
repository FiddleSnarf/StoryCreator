#include "StoryView.hpp"
#include "./StoryManager.hpp"
#include <QMimeData>

StoryView::StoryView(QWidget* parent) :
    QGraphicsView(parent),
    m_currZoom(0),
    m_originX(0.),
    m_originY(0.)
{
    setAlignment( Qt::AlignTop | Qt::AlignLeft );


    this->setRenderHint(QPainter::Antialiasing, true);
    this->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setupMatrix();
}

StoryView::~StoryView()
{

}

void StoryView::setCore(ICorePtr core)
{
    m_core = core;
}

void StoryView::wheelEvent(QWheelEvent* event)
{
    if (!m_core)
    {
        event->accept();
        return;
    }

    const bool inc = event->delta() > 0;
    if (inc)
    {
        zoomIn();
        StoryNodeItemPtr selectedNodeItem = m_core->getStoryManager()->getSelectedNodeItem();
        if (selectedNodeItem)
            centerOn(selectedNodeItem);
    }
    else
    {
        zoomOut();
    }

    event->accept();
}

void StoryView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
    {
        // Сохраним оригинальную позицию.
        m_originX = event->x();
        m_originY = event->y();
    }
    QGraphicsView::mousePressEvent(event);
}

void StoryView::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::RightButton)
    {
        QPointF oldp = mapToScene(m_originX, m_originY);
        QPointF newp = mapToScene(event->pos());
        QPointF translation = newp - oldp;

        translate(translation.x(), translation.y());

        m_originX = event->x();
        m_originY = event->y();
    }
    QGraphicsView::mouseMoveEvent(event);
}

void StoryView::zoomIn()
{
    if (m_currZoom < m_zoomMax)
    {
        m_currZoom += m_zoomStep;
        setupMatrix();
    }
}

void StoryView::zoomOut()
{
    if (m_currZoom > m_zoomMin)
    {
        m_currZoom -= m_zoomStep;
        setupMatrix();
    }
}

void StoryView::unZoom()
{
    m_currZoom = 0;
    setupMatrix();
}

void StoryView::setZoom(double zoom)
{
    m_currZoom = zoom;
    setupMatrix();
}

void StoryView::setupMatrix()
{
    double scale = pow(2., m_currZoom);
    this->setMatrix(QMatrix(scale, 0, 0, scale, 0, 0));
}
