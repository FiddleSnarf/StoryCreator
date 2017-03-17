#pragma once

#include <QGraphicsView>
#include <QWheelEvent>

class StoryView : public QGraphicsView
{
public:
    explicit StoryView(QWidget* parent = Q_NULLPTR);
	~StoryView();

protected:
    void wheelEvent (QWheelEvent* event) override;

private slots:

private:

};
