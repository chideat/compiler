#ifndef BASIC_H
#define BASIC_H

#include <QString>
#include <QMetaType>

enum COMPILERLEVEL {
    RUN,DEBUG,MA,SA,STOP
};

enum ERRORLEVEL{
    ERROR,WARNING,OTHER
};

class SError {
public :
    QString getErrorType();
    QString getErrorDetail();
    void setErrorType(ERRORLEVEL type);
    void setErrorDetail(QString detail);
private:
    ERRORLEVEL errorType;
    QString errorDetail;
};
Q_DECLARE_METATYPE(SError)

class CError{
public :
    QString getErrorType();
    long getErrorLine();
	QString getErrorFile();
	QString getErrorDetail();
    void setErrorType (QString type);
    void setErrorLine(long line);
    void setErrorFile(QString file);
    void setErrorDetail(QString detail);
private :
	QString errorType;//错误类型
    long errorLine;//错误行号
	QString errorFile;//出错文件
	QString errorDetail;
};

Q_DECLARE_METATYPE(CError)




#endif // BASIC_H
