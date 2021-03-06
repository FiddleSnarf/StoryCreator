#include "NodeAction.h"
#include "Common/StoryJsonTags.hpp"

NodeAction::NodeAction() :
    m_isValid(false)
{

}

NodeAction::~NodeAction()
{

}

bool NodeAction::operator==(const NodeAction& other) const
{
    return (m_actLogic == other.getActionLogic() &&
            m_type == other.getActionType() &&
            m_text == other.getActionText());
}

bool NodeAction::operator!=(const NodeAction& other) const
{
    return !(*this == other);
}

void NodeAction::read(const QJsonObject& jsonAction)
{
    if (!jsonAction.contains(StoryJsonTags::type_tag))
    {
        m_isValid = false;
        return;
    }

    m_type = jsonAction[StoryJsonTags::type_tag].toString();
    if (jsonAction.contains(StoryJsonTags::action_logic_tag))
        m_actLogic = jsonAction[StoryJsonTags::action_logic_tag].toString();

    if (jsonAction.contains(StoryJsonTags::text_tag))
        m_text = jsonAction[StoryJsonTags::text_tag].toString();

    m_isValid = true;
}

void NodeAction::write(QJsonObject& jsonObject) const
{
    if (!m_isValid)
        return;

    jsonObject[StoryJsonTags::type_tag] = m_type;
    if (!m_actLogic.isEmpty())
        jsonObject[StoryJsonTags::action_logic_tag] = m_actLogic;

    if (!m_text.isEmpty())
        jsonObject[StoryJsonTags::text_tag] = m_text;
}

bool NodeAction::isValid() const
{
    return m_isValid;
}

QString NodeAction::getActionLogic() const
{
    return m_actLogic;
}

QString NodeAction::getActionType() const
{
    return m_type;
}

QString NodeAction::getActionText() const
{
    return m_text;
}
