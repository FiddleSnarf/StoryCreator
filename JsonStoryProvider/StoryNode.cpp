#include <QJsonArray>
#include "StoryNode.h"

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
    if (!jsonNode.contains("nodeID") ||
        !jsonNode.contains("title") ||
        !jsonNode.contains("text") ||
        !jsonNode.contains("type"))
    {
        m_isValid = false;
        return;
    }

    m_id = jsonNode["nodeID"].toInt();
    m_type = jsonNode["type"].toString();
    m_title = jsonNode["title"].toString();
    m_text = jsonNode["text"].toString();
    if (jsonNode.contains("actions"))
    {
        QJsonArray actionsArray = jsonNode["actions"].toArray();
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

    jsonObject["nodeID"] = m_id;
    jsonObject["type"] = m_type;
    jsonObject["title"] = m_title;
    jsonObject["text"] = m_text;

    if (!m_actionList.isEmpty())
    {
        QJsonArray actionsArray;
        foreach (const NodeAction& action, m_actionList)
        {
            QJsonObject actionObject;
            action.write(actionObject);
            actionsArray.append(actionObject);
        }
        jsonObject["actions"] = actionsArray;
    }
}

bool StoryNode::isValid() const
{
    return m_isValid;
}
