#ifndef TABLEVIEW_H
#define TABLEVIEW_H


#include <QTableView>
#include <QContextMenuEvent>
#include <QMenu>

class TableView : public QTableView
{
    Q_OBJECT


    public:

       TableView(QWidget *parent);                            // on reimplemente la classe QTableView pour pouvoir utiliserla methode selectedIndexes() laquelle est par defaut privee
       QModelIndexList SelectedIndexes();     //on redefinit la methode selectedIndexes


    signals:
       void clickDroit(QContextMenuEvent *event);


    protected:
        void contextMenuEvent(QContextMenuEvent *event);


};


#endif // TABLEVIEW_H
