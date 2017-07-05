#pragma once

#include "NodeAction.h"

class StoryNode
{
    typedef unsigned int NodeErrorFlags;
    enum EnErrorFlags
    {
        enIdIncorrectFlag = 1,
        enTypeEmptyFlag = 2,
        enTitleEmptyFlag = 4,
        enTextEmptyFlag = 8
    };

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
    const QStringList& getErrorsList();

    void setId(int id);
    void setType(const QString& type);
    void setTitle(const QString& title);
    void setText(const QString& text);
    void setNodeActionList(const NodeActionList& actionList);

    bool operator<(const StoryNode& other) const;
    bool operator==(const StoryNode& other) const;
    bool operator!=(const StoryNode& other) const;

private:
    void checkValid();

    int m_id;
    QString m_type;
    QString m_title;
    QString m_text;
    NodeActionList m_actionList;

    bool m_isValid;
    NodeErrorFlags m_errorFlags;
    QStringList m_errorsList;
};

typedef QList<class StoryNode> StoryNodeList;
