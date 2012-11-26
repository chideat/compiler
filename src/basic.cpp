#include "basic.h"
#include <QtGui>

//ERROR
void CError::setErrorType (QString type) {
    errorType = type;
}

void CError::setErrorLine (long line) {
    errorLine = line;
}

void CError::setErrorFile (QString file) {
    errorFile = file;
}

void CError::setErrorDetail (QString detail) {
    errorDetail = detail;
}

QString CError::getErrorType () {
    return errorType;
}


long CError::getErrorLine () {
    return errorLine;
}

QString CError::getErrorFile () {
    return errorFile;
}

QString CError::getErrorDetail () {
    return errorDetail;
}


