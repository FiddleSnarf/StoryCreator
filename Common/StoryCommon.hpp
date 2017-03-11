#ifndef STORYTYPES_H
#define STORYTYPES_H

#include <QList>
#include <QString>
#include <QIcon>

namespace StoryCommon
{

struct NodeTemplate
{
    QString nodeType;
    QIcon icon;
    QString toolTip;
};

typedef QList<NodeTemplate> SelectNodeList;


}

#endif // STORYTYPES_H
