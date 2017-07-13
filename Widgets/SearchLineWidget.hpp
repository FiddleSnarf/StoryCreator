#pragma once

#include <QWidget>

namespace Ui
{
    class SearchLineWidget;
}

class SearchLineWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SearchLineWidget(QWidget* parent = Q_NULLPTR);
    virtual ~SearchLineWidget();

signals:
    void signalTextChanged(const QString& text);

private:
    QScopedPointer<Ui::SearchLineWidget> m_ui;
};
