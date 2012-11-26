#include "treeview.h"
#include <QtGui>

//TreeView::TreeView (QWidget *parent) : QTreeView(parent) { 
//    model = new QStandardItemModel(this);
//    model->setHeaderData (0,Qt::Horizontal,tr("Project"));
//    setModel (model);
//   // setHeaderHidden (true);
//}

//QList <QStandardItem *> TreeView::returnTheItems () {
//    return model->findItems ("*",Qt::MatchWildcard | Qt::MatchRecursive);
//}


//void TreeView::iterateOverItems () {
//    QList <QStandardItem *> list = returnTheItems ();
//    foreach(QStandardItem *item,list) {
//        qDebug ()<<item->text ();
//    }
//}

//void TreeView::mouseDoubleClickEvent (QMouseEvent *event) {
//    if(event->button () == Qt::LeftButton) {
//        QModelIndex index = currentIndex ();
//        qDebug ()<<index.data ().toString ();
//    }
//}
