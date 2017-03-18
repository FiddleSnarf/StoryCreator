#pragma once

#include <QString>
#include <QUrl>
#include <QList>

#include "StoryCommon.hpp"

namespace StoryCommon
{
    class NodeInfo
    {
    public:
        explicit NodeInfo(int nodeId, const QString& typeNode);

        void setEntryPointFlag(bool state);
        void setTitle(const QString& title);
        void setText(const QString& text);
        void setImageUrl(const QUrl& imageUrl);

        int getNodeID() const;
        QString getTypeNode() const;
        bool getEntryPointFlag() const;
        QString getTitle() const;
        QString getText() const;
        QUrl getImageUrl() const;

        //void addAction(const Action& action);
        int actionCount() const;

    private:
        int m_nodeID;
        QString m_typeNode;

        bool m_isEntryPoint;
        QString m_title;
        QString m_text;
        QUrl m_imageUrl;
        QList<CompleteAction> m_actions;
    };
}
