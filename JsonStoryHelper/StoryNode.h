#pragma once

#include "NodeAction.h"

class StoryNode
{
public:
    StoryNode();
    virtual ~StoryNode();

    void read(const QJsonObject& jsonNode);
    void write(QJsonObject& jsonObject) const;
    bool isValid() const;

private:
    int m_id;
    QString m_type;
    QString m_title;
    QString m_text;
    NodeActionList m_actionList;

    bool m_isValid;
};

typedef QList<class StoryNode> StoryNodeList;
