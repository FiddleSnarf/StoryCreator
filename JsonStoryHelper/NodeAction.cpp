#include "NodeAction.h"

NodeAction::NodeAction() :
    m_isValid(false)
{

}

NodeAction::~NodeAction()
{

}

void NodeAction::read(const QJsonObject& jsonAction)
{
    if (!jsonAction.contains("type"))
    {
        m_isValid = false;
        return;
    }

    m_type = jsonAction["type"].toString();
    if (jsonAction.contains("action_logic"))
        m_actLogic = jsonAction["action_logic"].toString();

    if (jsonAction.contains("text"))
        m_text = jsonAction["text"].toString();

    m_isValid = true;
}

void NodeAction::write(QJsonObject& jsonObject) const
{
    if (!m_isValid)
        return;

    jsonObject["type"] = m_type;
    if (!m_actLogic.isEmpty())
        jsonObject["action_logic"] = m_actLogic;

    if (!m_text.isEmpty())
        jsonObject["text"] = m_text;
}

bool NodeAction::isValid() const
{
    return m_isValid;
}
