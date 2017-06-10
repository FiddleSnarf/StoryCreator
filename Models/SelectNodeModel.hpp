#pragma once

#include <QAbstractListModel>

#include "Common/StoryCommon.hpp"

class QMimeData;

class SelectNodeModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SelectNodeModel(QObject* parent = 0);
    ~SelectNodeModel();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QMimeData* mimeData(const QModelIndexList& indexes) const override;
    QStringList mimeTypes() const override;
    Qt::DropActions supportedDropActions() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent) const override;

    void clear();
    /*void addTemplateNode(const StoryCommon::NodeTemplate& nodeTemplate);
    void addTemplateNodesList(const StoryCommon::SelectNodeList& listTNodes);*/

private:
    void removeDuplicates();

private:
    //StoryCommon::SelectNodeList m_nodesTemplates;
};
