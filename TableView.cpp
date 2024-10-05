#include "TableView.h"

TableView::TableView(QWidget *parent) : QTableView(parent)
{
    this->setDragEnabled(true);                     //pour faire le drag and drop
    this->setDropIndicatorShown(true);
    this->setAcceptDrops(true);
    this->setDragDropMode(QAbstractItemView::DragDrop);
    this->setDefaultDropAction(Qt::CopyAction);
    this->setDragDropOverwriteMode(true);

}

//on cree une methode SelectedIndexes accessible a toutes les autres classes dont le but est de retourner selectedIndexes

QModelIndexList TableView::SelectedIndexes()
{
    return this->selectedIndexes();
}


void TableView::contextMenuEvent(QContextMenuEvent *event)
{
    emit clickDroit(event);                                    // un signal est emit si le click droit est effectue dans la tableView
}


