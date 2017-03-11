#pragma once

#include <QListWidget>

class ItemListWidget : public QListWidget
{
public:
    ItemListWidget(QWidget *parent = Q_NULLPTR);
    ~ItemListWidget();
};
