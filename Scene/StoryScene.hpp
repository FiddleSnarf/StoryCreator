#pragma once
#include <QGraphicsScene>

class StoryScene : public QGraphicsScene
{
public:
	explicit StoryScene(QObject* parent = Q_NULLPTR);
	~StoryScene();
};