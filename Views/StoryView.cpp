#include "StoryView.hpp"
#include <QMimeData>

StoryView::StoryView(QWidget* parent) :
    QGraphicsView(parent),
    m_currZoom(0)
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

void StoryView::wheelEvent(QWheelEvent* event)
{
    const bool inc = event->delta() > 0;
    if (inc)
    {
        zoomIn();
    }
    else
    {
        zoomOut();
    }

    event->accept();
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
