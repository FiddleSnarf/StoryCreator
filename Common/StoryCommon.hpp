#pragma once

#include <QList>
#include <QString>
#include <QIcon>
#include <QRectF>

namespace StoryCommon
{

const QString NODE_MIME_TYPE = "story/node-template";

struct NodeTemplate
{
    QString nodeType;
    QIcon icon;
    QString toolTip;
};

typedef QList<NodeTemplate> SelectNodeList;
}
