#include "NodeInfo.hpp"

using namespace StoryCommon;

NodeInfo::NodeInfo(int nodeId, const QString& typeNode) :
    m_nodeID(nodeId),
    m_typeNode(typeNode),
    m_isEntryPoint(false)
{

}

void NodeInfo::setEntryPointFlag(bool state)
{
    m_isEntryPoint = state;
}

void NodeInfo::setTitle(const QString& title)
{
    m_title = title;
}

void NodeInfo::setText(const QString& text)
{
    m_text = text;
}

void NodeInfo::setImageUrl(const QUrl& imageUrl)
{
    m_imageUrl = imageUrl;
}

int NodeInfo::getNodeID() const
{
    return m_nodeID;
}

QString NodeInfo::getTypeNode() const
{
    return m_typeNode;
}

bool NodeInfo::getEntryPointFlag() const
{
    return m_isEntryPoint;
}

QString NodeInfo::getTitle() const
{
    return m_title;
}

QString NodeInfo::getText() const
{
    return m_text;
}

QUrl NodeInfo::getImageUrl() const
{
    return m_imageUrl;
}

/*void NodeInfo::addAction(const Action& action)
{
    m_actions << action;
}*/

int NodeInfo::actionCount() const
{
    return m_actions.size();
}
