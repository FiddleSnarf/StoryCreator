#pragma once

#include "NodeAction.h"

class StoryNode
{
public:
    StoryNode();
    StoryNode(int id, const QString& type);
    virtual ~StoryNode();

    void read(const QJsonObject& jsonNode);
    void write(QJsonObject& jsonObject) const;

    bool isValid() const;
    int getId() const;
    QString getType() const;
    QString getTitle() const;
    QString getText() const;
    const NodeActionList& getNodeActionList() const;

    void setType(const QString& type);
    void setTitle(const QString& title);
    void setText(const QString& text);

    bool operator< (const StoryNode& other) const;
    bool operator==(const StoryNode& other) const;

private:
    int m_id;
    QString m_type;
    QString m_title;
    QString m_text;
    NodeActionList m_actionList;

    bool m_isValid;
};

typedef QList<class StoryNode> StoryNodeList;
