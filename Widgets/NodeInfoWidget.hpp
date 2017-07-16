#pragma once

#include <QWidget>
#include <QTextEdit>

#include "Common/StoryTypesNodeCollector.hpp"

class StoryNodeItem;

namespace Ui
{
    class NodeInfoWidget;
}

class NodeInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NodeInfoWidget(QWidget* parent = Q_NULLPTR);
    virtual ~NodeInfoWidget();

    void setCurrentNodeItem(StoryNodeItem* node);

public slots:
    void slotNodeDeleted();
    void slotSearchTextChanged(const QString& searchedText);

protected:
    virtual void hideEvent(QHideEvent* event) override;

private slots:
    void slotUpdateNodeData();

private:
    void initialization();
    void initHighlightParams();
    void updateUI();
    void blockUISignals();
    void unblockUISignals();
    void updateSelectionSearchedText();

private:
    QScopedPointer<Ui::NodeInfoWidget> m_ui;

    StoryTypesNodeCollector m_typesNodeCollector;
    StoryNodeItem* m_currentNodeItem;

    QString m_searchedText;                         /**< Текущий искомый текст в панели навигации. */
    QTextEdit::ExtraSelection m_highlightSelection; /**< Параметры выделения найденного через поиск текста. */
};
