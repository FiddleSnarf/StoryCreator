#include "SelectNodeModel.hpp"
#include <QMimeData>

//==================================== public ===========================================

SelectNodeModel::SelectNodeModel(QObject* parent) : QAbstractListModel(parent)
{

}

SelectNodeModel::~SelectNodeModel()
{

}

QVariant SelectNodeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch(role)
    {
        case Qt::DecorationRole:
            return m_nodesTemplates.value(index.row()).icon;

        case Qt::ToolTipRole:
            return m_nodesTemplates.value(index.row()).toolTip;

        case Qt::UserRole:
            return m_nodesTemplates.value(index.row()).nodeType;
    }

    return QVariant();
}

QMimeData* SelectNodeModel::mimeData(const QModelIndexList& indexes) const
{
    QMimeData* mimeData = new QMimeData();
    QModelIndex idx = indexes.first();
    if (!idx.isValid())
        return mimeData;

    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    QIcon icon = qvariant_cast<QIcon>(data(idx, Qt::DecorationRole));
    QString nodeType = data(idx, Qt::UserRole).toString();
    stream << nodeType << icon;

    mimeData->setData(StoryCommon::NODE_MIME_TYPE, encodedData);
    return mimeData;
}

QStringList SelectNodeModel::mimeTypes() const
{
    QStringList types;
    types << StoryCommon::NODE_MIME_TYPE;
    return types;
}

Qt::DropActions SelectNodeModel::supportedDropActions() const
{
    return Qt::CopyAction;
}

Qt::ItemFlags SelectNodeModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractListModel::flags(index);
    if (index.isValid())
        flags |= Qt::ItemIsDragEnabled;

    return flags;
}

int SelectNodeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    else
        return m_nodesTemplates.size();
}

void SelectNodeModel::clear()
{
    //beginRemoveRows(QModelIndex(), 0, m_nodesTemplates.size() - 1);
    m_nodesTemplates.clear();
    //endRemoveRows();
}

void SelectNodeModel::addTemplateNode(const StoryCommon::NodeTemplate& nodeTemplate)
{
    m_nodesTemplates.append(nodeTemplate);
    removeDuplicates();
}

void SelectNodeModel::addTemplateNodesList(const StoryCommon::SelectNodeList& listTNodes)
{
    m_nodesTemplates += listTNodes;
    removeDuplicates();
}

//=======================================================================================

//=================================== private ===========================================

void SelectNodeModel::removeDuplicates()
{
    // TODO
}

//=======================================================================================
