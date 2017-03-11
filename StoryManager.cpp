#include "StoryManager.hpp"

StoryManager::StoryManager(QObject* parent) : QObject(parent)
{

}

StoryScene& StoryManager::getStoryScene()
{
    return m_storyScene;
}

SelectNodeModel& StoryManager::getStoryNodeSelectModel()
{
    return m_storyNodeSelectModel;
}

void StoryManager::setSelectListStoryNodes(const StoryCommon::SelectNodeList& listTNodes)
{
    m_storyNodeSelectModel.addTemplateNodesList(listTNodes);
    /*m_storyNodeSelectModel.clear();
    m_storyNodeSelectModel.setRowCount(listTNodes.size());
    m_storyNodeSelectModel.setColumnCount(1);

    for (int i = 0; i < m_storyNodeSelectModel.rowCount() ; ++i)
    {
        QModelIndex idx = m_storyNodeSelectModel.index(i, 0);
        m_storyNodeSelectModel.setData(idx, listTNodes[i].toolTip, Qt::ToolTipRole);
        m_storyNodeSelectModel.setData(idx, listTNodes[i].icon, Qt::DecorationRole);
    }*/
}
