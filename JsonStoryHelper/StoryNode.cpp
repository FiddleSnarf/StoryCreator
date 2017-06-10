#include <QJsonArray>
#include "StoryNode.h"
#include "Common/StoryJsonTags.hpp"

StoryNode::StoryNode() :
    m_id(-1),
    m_isValid(false)
{

}

StoryNode::~StoryNode()
{

}

void StoryNode::read(const QJsonObject& jsonNode)
{
    if (!jsonNode.contains(StoryJsonTags::nodeID_tag) ||
        !jsonNode.contains(StoryJsonTags::title_tag) ||
        !jsonNode.contains(StoryJsonTags::text_tag) ||
        !jsonNode.contains(StoryJsonTags::type_tag))
    {
        m_isValid = false;
        return;
    }

    m_id = jsonNode[StoryJsonTags::nodeID_tag].toInt();
    m_type = jsonNode[StoryJsonTags::type_tag].toString();
    m_title = jsonNode[StoryJsonTags::title_tag].toString();
    m_text = jsonNode[StoryJsonTags::text_tag].toString();
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
    m_isValid = true;
}

void StoryNode::write(QJsonObject& jsonObject) const
{
    if (!m_isValid)
        return;

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
