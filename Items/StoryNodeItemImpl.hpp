#include <QGraphicsObject>

class StoryNodeItem : public QGraphicsObject
{
public:
    StoryNodeItem(QGraphicsItem *parent = Q_NULLPTR);
    ~StoryNodeItem();

private:
    int m_nodeID;
    QString m_caption;
    QString m_text;
};
