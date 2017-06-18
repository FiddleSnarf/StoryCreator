#pragma once

#include <QList>
#include <QJsonObject>

class NodeAction
{
public:
    NodeAction();
    virtual ~NodeAction();

    void read(const QJsonObject& jsonAction);
    void write(QJsonObject& jsonObject) const;
    bool isValid() const;

    bool operator==(const NodeAction& other) const;

    QString getActionLogic() const;
    QString getActionType() const;
    QString getActionText() const;

private:
    QString m_actLogic;
    QString m_type;
    QString m_text;

    bool m_isValid;
};

typedef QList<NodeAction> NodeActionList;
