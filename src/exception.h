#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "basic.h"
#include <QtGui>

/*****************************************************************************************************/
typedef struct estack {
       CError *error;
       struct estack *next;
}ExceptionStatck;


class Window;
class ExceptionManagement : public QThread {
    Q_OBJECT
public :
    QMutex *mutex;
    QSemaphore *exceptionSemaphore;
    enum EXCEPTION{ERROR,WARNING,OTHER};
    ExceptionManagement(Window *window);
private :
    Window *parent;
    QTreeWidget *treeWidget;
    ExceptionStatck *headException;
    CError *error;
    int errorCount;
    int warningCount;
    int otherCount;
    
    QTreeWidgetItem *topError;
    QTreeWidgetItem *topWarning;
    QTreeWidgetItem *topOther;


    //here do with the display of the errors
    void exceptionDisplay(EXCEPTION type);
public slots:
    void getException(CError *e);
    void clear();
protected :
    void exceptionStackPush(CError *e);
    CError *exceptionStackPop();
    void run();
signals:
    void emitUpdateLable(int e,int w,int o);

};



#endif // EXCEPTION_H
