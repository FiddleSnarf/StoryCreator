#pragma once

#include <QList>
#include <QString>
#include <QIcon>
#include <QRectF>

namespace StoryCommon
{

const QString NODE_MIME_TYPE = "story/node-template";

enum class ActionType
{
    en_Fight,
    en_Force,
    en_Vote
};

enum class ActionLogicType
{
    en_Goto,
    en_Hero,
    en_Items,
    en_Npc,
    en_Endlife
};

enum class NodeType
{
    en_Stats,
    en_Common,
    en_Story
};

struct CompleteAction
{
    explicit CompleteAction()
    {
    }

    ActionType type;
    QString text;
    QString action;
    QString condition;
};

struct NodeTemplate
{
    QString nodeType;
    QIcon icon;
    QString toolTip;
};

typedef QList<NodeTemplate> SelectNodeList;

//============================================ global functions ===========================================
//TODO
//=========================================================================================================

} // namespace StoryCommon
