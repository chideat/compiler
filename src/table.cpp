#include "table.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>

/************************************************************************************************************************/
//marktable
MarkTable::MarkTable() {
    headId = NULL;
    headConst = NULL;
    countId = 1;
    countConst = 1;
}


long MarkTable::appendConst (QString &constant) {
//    Constant *tmp = new Constant;
//    tmp->locate = countConst ++;
//    tmp->name = "num";
//    tmp->value = constant;
    
    Constant *next = headConst;
    if(next == NULL) {
        Constant *tmp = new Constant;
        tmp->locate = countConst ++;
        tmp->name = constant;
        tmp->value = "--";
        
        tmp->next = headConst;
        headConst = tmp;
        return countConst - 1;
    }
    else 
        while(next != NULL) {
            if(next->name.compare (constant) == 0){
                return next->locate;
            }
            else if(next->next == NULL) {
                    Constant *tmp = new Constant;
                    tmp->locate = countConst ++;
                    tmp->name = constant;
                    tmp->value = "--";

                    tmp->next = NULL;
                    next->next = tmp;
                    return countConst - 1;
            }
            next = next->next;
        }
    return -1;
}

long MarkTable::appendId (QString &id) {
    Identifier *next = headId;
    if(next == NULL) {
        Identifier *tmp = new Identifier;
        tmp->locate = countId ++;
        tmp->name = id;
        tmp->address = "--";
        tmp->type = "--";
        
        tmp->next = headId;
        headId = tmp;
        return countId - 1;
    }
    else 
        while(next != NULL) {
            if(next->name.compare (id) == 0){
                return next->locate;
            }
            else if(next->next == NULL) {
                    Identifier *tmp = new Identifier;
                    tmp->locate = countId ++;
                    tmp->name = id;
                    tmp->address = "--";
                    tmp->type = "--";
                    
                    tmp->next = NULL;
                    next->next = tmp;
                    return countId - 1;
            }
            next = next->next;
        }
    return -1;
}


void MarkTable::checkConst () {
    
}

void MarkTable::checkId () {
    
}

//tmp fun
void MarkTable::displayConstAll () {
    //default filename
    //constDF.df
    QFile file("./constDF.df");
    if(!file.open (QFile::WriteOnly | QFile::Truncate)) {
        qDebug () << "output file open error "<<"constDF.df";
    }
    QTextStream *out = new QTextStream(&file);
    out->setCodec ("UTF-8");
    Constant *next = headConst;
    while(next != NULL){
        (*out)<<next->locate<<'\t'<<next->name<<'\t'<<next->value<<'\n';
        next = next ->next;
    }
    file.close();
}


void MarkTable::displayConstItem () {

}


void MarkTable::displayIdAll () {
    //idDF.df
    QFile file("./idDf.df");
    if(!file.open (QFile::WriteOnly | QFile::Truncate)) {
        qDebug () << "output file open error "<<"idDF.df";
    }
    QTextStream *out = new QTextStream(&file);
    out->setCodec ("UTF-8");
    Identifier *next = headId;
    while(next != NULL){
        (*out)<<next->locate<<'\t'<<next->name<<'\t'<<next->type<<'\t'<<next->address<<'\n';
        next = next ->next;
    }
    file.close();
}


void MarkTable::displayIdItem () {
    
}
