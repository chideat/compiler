#include "window.h"
#include "exception.h"
#include <QtGui>

//ExceptionManagement
ExceptionManagement::ExceptionManagement(Window *window) {
    this->parent = window;
    headException = NULL;
    errorCount = 0;
    warningCount = 0;
    otherCount = 0;
    treeWidget = parent->treeWidget;
    topError = new QTreeWidgetItem(treeWidget);
    topWarning = new QTreeWidgetItem(treeWidget);
    topOther = new QTreeWidgetItem(treeWidget);
    
    topError->setText (0,tr("0 errors"));
    topWarning->setText (0,tr("0 warnings"));
    topOther->setText (0,tr("0 others"));
    
    exceptionSemaphore = new QSemaphore(0);
    mutex = new QMutex;
    
    topError->setHidden (true);
    topWarning->setHidden (true);
    topOther->setHidden (true);
    
    connect (this,SIGNAL(emitUpdateLable(int,int,int)),parent,SLOT(exceptionLabel(int,int,int)));
    //connect (parent,SIGNAL(emitExceptionClear()),this,SLOT(clear()));
}

void ExceptionManagement::clear () {
    errorCount = 0;
    warningCount = 0;
    otherCount = 0;
    topError->setText (0,tr("0 errors"));
    topWarning->setText (0,tr("0 warnings"));
    topOther->setText (0,tr("0 others"));
    
    topError->setHidden (true);
    topWarning->setHidden (true);
    topOther->setHidden (true);
    
    int tmp = topError->childCount ();
    for(int i = 0;i < tmp;i ++) {
        topError->removeChild (topError->child (0));
    }
    tmp = topWarning->childCount ();
    for(int i = 0;i < tmp;i ++){
        topWarning->removeChild (topWarning->child (0));
    }
    tmp = topOther->childCount ();
    for(int i = 0;i < tmp;i ++){
        topOther->removeChild (topOther->child (0));
    }
    emit emitUpdateLable (topError->childCount (),topWarning->childCount (),topOther->childCount ());
}

void ExceptionManagement::exceptionStackPush (CError *e) {
    ExceptionStatck *tmp = new ExceptionStatck;
    tmp->error = e;
    tmp->next = headException;
    headException = tmp;
}

CError * ExceptionManagement::exceptionStackPop () {
    ExceptionStatck *tmp = NULL;
    if(headException != NULL) {
        tmp = headException;
        headException = tmp->next;
    }
    return tmp->error;
}

void ExceptionManagement::getException (CError *e) {
    mutex->lock ();
   exceptionStackPush (e);
   mutex->unlock ();
   exceptionSemaphore->release (1);
}

void ExceptionManagement::run () {
    while(1) {
        //wakeup
        exceptionSemaphore->acquire (1);
        mutex->lock ();
        error = exceptionStackPop ();
        mutex->unlock ();
        
        if(error != NULL) {
            exceptionDisplay(ERROR);
            error = NULL;
        }
        emit emitUpdateLable (topError->childCount (),topWarning->childCount (),topOther->childCount ());
    }
}

void ExceptionManagement::exceptionDisplay (EXCEPTION type) {
    QTreeWidgetItem *tmp;
    if(type == ERROR) {
        topError->setHidden (false);
        tmp = new QTreeWidgetItem(topError);
        topError->setText (0,tr("%1 errors").arg (topError->childCount ()));
        tmp->setText (0,error->getErrorFile ());
        tmp->setText (1,QString::number (error->getErrorLine ()));
        tmp->setText (2,error->getErrorType ());
        tmp->setText (3,error->getErrorDetail ());
    }
    else if(type == WARNING) {
        topWarning->setHidden (false);
        topWarning->setText (0,tr("%1 warnings").arg (topWarning->childCount ()));

    }
    else {
        topOther->setHidden (false);
        topOther->setText (0,tr("%1 others").arg (topOther->childCount ()));
    }
}

