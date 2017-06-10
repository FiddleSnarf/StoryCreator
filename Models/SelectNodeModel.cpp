#include "SelectNodeModel.hpp"
#include <QMimeData>
#include <QDataStream>

//==================================== public ===========================================

SelectNodeModel::SelectNodeModel(QObject* parent) :
    QAbstractListModel(parent)
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

    mimeData->setData(SoryGUI::NODE_MIME_TYPE, encodedData);
    return mimeData;
}

QStringList SelectNodeModel::mimeTypes() const
{
    QStringList types;
    types << SoryGUI::NODE_MIME_TYPE;
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
    /*if (parent.isValid())
        return 0;
    else
        return m_nodesTemplates.size();*/
    return 0;
}

void SelectNodeModel::addTemplateNode(const StoryCommon::NodeSelectTemplate& nodeTemplate)
{
    m_nodesTemplates.append(nodeTemplate);
    removeDuplicates();
}

void SelectNodeModel::addTemplateNodesList(const StoryCommon::SelectTNodeList& listTNodes)
{
    m_nodesTemplates += listTNodes;
    removeDuplicates();
}

//=======================================================================================

//=================================== private ===========================================

void SelectNodeModel::removeDuplicates()
{
    // TODO не уверен надо ли это вообще (по идее эта модель заполняется при старте и больше не редактируется)
}

//=======================================================================================
