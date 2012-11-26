#ifndef FUNCTIONCOMPILER_H
#define FUNCTIONCOMPILER_H

#include "basic.h"
#include "table.h"
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QSet>
#include <QThread>

#define BLOCK_SIZE 4096

class ExceptionManagement;
class Compiler : public QThread
{
    Q_OBJECT
public :
    
    Compiler(ExceptionManagement *e);
    bool setProject(QString p);
    bool setLevel(COMPILERLEVEL l);
protected:
    void run();
private :
    enum STATE {
        NONE_S,
        ING_CHAR,
        ING_STRING,
        ING_DIGIT,
        ING_IDENTIFIER,
        ING_OPERATOR_TWO,ING_OPERATOR_ONE,ING_OPERATOR_THREE,
        ING_REMARK,ING_MAYBE_REMARK,
        ING_NEW_LINE,
        END
    } state,previous;
    enum RESULT{
        ERROR,NONE_R,END_NEEDED,MAY_END,MATCHED,SKIP,SINGLE,NEW_LINE,DOT_MATCHED
    }result;
    
    
    MarkTable *table;
    CError *error;
    COMPILERLEVEL level;
    ExceptionManagement *exception;
    
    //QStringList Identifier;
    QSet<QString> Identifier;
    int currentLine;
    QString project;
    QTextStream *out;
    
    //compiler fun funs
    void execute();
    void debug ();
    void morphologicalAnalysis();
    void syntacticAnalysis ();
    
    //here i need to count the number of '\n's to point out the numbers of the lines
    void sourceCode();
    void isWhat(QChar oneChar,long *i);
    //here search the to tables
    void createDuality(const QString &word,const QString &value);
    void initIdentifier() ;
    void initConnect();
    void initCError(QString type,long line,QString fileName,QString detail);
    void initSError(ERRORLEVEL type,QString detail);
signals :
    void emitError(CError *error);
    void emitRunEnable(bool runEnable);
    void emitSystemError(SError *error);
};
#endif // FUNCTIONCOMPILER_H
