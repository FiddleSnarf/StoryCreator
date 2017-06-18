#include <QJsonArray>
#include "StoryNode.h"
#include "Common/StoryJsonTags.hpp"

StoryNode::StoryNode() :
    m_id(-1),
    m_isValid(false)
{

}

StoryNode::StoryNode(int id, const QString& type) :
    m_id(id),
    m_type(type),
    m_isValid(false)
{
    if (id > 0 && !type.isEmpty())
        m_isValid = true;
}

StoryNode::~StoryNode()
{

}

bool StoryNode::operator< (const StoryNode& other) const
{
    return m_id < other.getId();
}

bool StoryNode::operator==(const StoryNode& other) const
{
    return (m_id == other.getId() &&
            m_type == other.getType() &&
            m_title == other.getTitle() &&
            m_text == other.getText() &&
            m_actionList == other.getNodeActionList());
}

void StoryNode::read(const QJsonObject& jsonNode)
{
    if (!jsonNode.contains(StoryJsonTags::nodeID_tag) || !jsonNode.contains(StoryJsonTags::type_tag))
    {
        m_isValid = false;
        return;
    }

    m_id = jsonNode[StoryJsonTags::nodeID_tag].toInt();
    m_type = jsonNode[StoryJsonTags::type_tag].toString();
    m_title = jsonNode[StoryJsonTags::title_tag].toString();
    m_text = jsonNode[StoryJsonTags::text_tag].toString();

    if (m_id < 0 || m_type.isEmpty())
        m_isValid = false;
    else
        m_isValid = true;

    if (jsonNode.contains(StoryJsonTags::actions_tag))
    {
        QJsonArray actionsArray = jsonNode[StoryJsonTags::actions_tag].toArray();
        if (!actionsArray.isEmpty())
        {
            for(int actIdx = 0; actIdx < actionsArray.size(); actIdx++)
            {
                NodeAction action;
                action.read(actionsArray[actIdx].toObject());
                if (action.isValid())
                    m_actionList << action;
            }
        }
    }
}

void StoryNode::write(QJsonObject& jsonObject) const
{
    jsonObject[StoryJsonTags::nodeID_tag] = m_id;
    jsonObject[StoryJsonTags::type_tag] = m_type;
    jsonObject[StoryJsonTags::title_tag] = m_title;
    jsonObject[StoryJsonTags::text_tag] = m_text;

    if (!m_actionList.isEmpty())
    {
        QJsonArray actionsArray;
        foreach (const NodeAction& action, m_actionList)
        {
            QJsonObject actionObject;
            action.write(actionObject);
            actionsArray.append(actionObject);
        }
        jsonObject[StoryJsonTags::actions_tag] = actionsArray;
    }
}

bool StoryNode::isValid() const
{
    return m_isValid;
}

int StoryNode::getId() const
{
    return m_id;
}

QString StoryNode::getType() const
{
    return m_type;
}

QString StoryNode::getTitle() const
{
    return m_title;
}

QString StoryNode::getText() const
{
    return m_text;
}

const NodeActionList& StoryNode::getNodeActionList() const
{
    return m_actionList;
}

void StoryNode::setType(const QString& type)
{
    m_type = type;
    m_isValid = !type.isEmpty();
}

void StoryNode::setTitle(const QString& title)
{
    m_title = title;
}

void StoryNode::setText(const QString& text)
{
    m_text = text;
}
