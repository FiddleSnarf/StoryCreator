#pragma once

#include <QGraphicsView>

class StoryView : public QGraphicsView
{
public:
	StoryView(QWidget* parent = Q_NULLPTR);
	~StoryView();

protected:
    void dropEvent(QDropEvent* event) override;
};
