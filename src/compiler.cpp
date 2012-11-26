#include "exception.h"
#include "compiler.h"
#include <QtGui>

Compiler::Compiler (ExceptionManagement *e) {
    exception = e;
    initIdentifier ();
    initConnect ();
}

void Compiler::initConnect(){
    connect (this,SIGNAL(emitError(CError*)),exception,SLOT(getException(CError*)));
}

//set Exception and set the project file
bool Compiler::setProject(QString p) {
    project = p;
    return true;
}

bool Compiler::setLevel (COMPILERLEVEL l){
    level = l;
    return true;
}

void Compiler::initIdentifier () {
    Identifier << "int" << "char" << "float" << "void" << "const" <<"for" 
               <<"if" <<"else" <<"then" <<"while" <<"switch" <<"break"
               << "begin" <<"end" <<"include" <<"string" <<"define";
}

void Compiler::run (){
    if(level == RUN) {
        execute ();
    }
    else if(level == DEBUG) {
        debug ();
    }
    else if(level == MA) {
        morphologicalAnalysis ();
    }
    else if(level == SA) {
        syntacticAnalysis ();
    }
    level = STOP;
}

void Compiler::execute() {
    
}

void Compiler::morphologicalAnalysis () {
    state = NONE_S;
    previous = NONE_S;
    result = NONE_R;
    table = new MarkTable;
    
    //qDebug () << "here in compiler";
    //init out file
    QString outputFileName;
    QFileInfo *fileInfo = new QFileInfo(project); 
    outputFileName =  "./" + fileInfo->fileName () + ".d";

    
    QFile file(outputFileName);
    if(!file.open (QFile::WriteOnly | QFile::Truncate)) {
        //emit system error
        
        qDebug () << "output file open error " + outputFileName;
    }
    out = new QTextStream(&file);
    out->setCodec ("UTF-8");
    sourceCode ();
    emit emitRunEnable(true);

    table->displayConstAll();
    table->displayIdAll();
    delete table;
}

void Compiler::syntacticAnalysis (){
    
}

void Compiler::debug () {
    
}

void Compiler::createDuality(const QString &word,const QString &value) {
    QString result;
    result = "(" + word + "," + value + ")";
    (*out) << result;
}

void Compiler::sourceCode () {
    QFile *file = new QFile(project);
    QString text;
    QString identifiedText;
    QTextStream in(file);
    QChar oneChar;
    previous = NONE_S;
    state = NONE_S;
    result = NONE_R;
    currentLine = 1;
    currentCol = 1;
    if(file->open (QFile::ReadOnly | QFile::Text)) {
        while(!in.atEnd ()) {
            text = in.read (BLOCK_SIZE);
            for(long i = 0;i <= text.length ();i ++) {
                if(i == text.length ()) 
                    oneChar = ' ';
                else 
                   oneChar = text.at (i);
                isWhat (oneChar,&i);
                
                if(state == END) {
                    if(previous == ING_OPERATOR_TWO) {
                        //relation
                        if(result != SINGLE) {
                            identifiedText.append (oneChar);
                        }
                        QRegExp regOne("[<>=!]=?");
                        QRegExp regTwo("([/\\*%\\|&=\\+-\\.#:]=?)|(--)|(\\+\\+)|(::)|(->)");
                        if(regOne.exactMatch (identifiedText)) {
                            if(identifiedText.compare ("!") == 0)
                                createDuality("not","-");
                            createDuality ("relop",identifiedText);
                        }
                        else if(regTwo.exactMatch (identifiedText))
                            createDuality (identifiedText,"-");
                        else if(identifiedText.compare ("||") == 0)
                            createDuality ("or","-");
                        else if(identifiedText.compare ("&&") == 0){
                            createDuality ("and","-");
                        }
                        else {
                            initCError ("DF001",currentLine,project,identifiedText);
                        }
                    }
                    else if(previous == ING_DIGIT || previous == ING_STRING) {
                        if(result == ERROR){
                            //dot error
                            initCError ("DF001",currentLine,project,"extra dot");
                            i ++;
                        }
                        //digit or string
                        //insert into the table first and return the value of the 
                        //may be i can return the relative locate of it 
                        //createDuality ("const",QString::number (table->appendConst (identifiedText)));
                        else {
                            if(previous == ING_STRING)
                            identifiedText.append (oneChar);
                       // table->appendConst (identifiedText);
                           createDuality ("const",QString::number (table->appendConst (identifiedText)));
                       }
                   }
                    else if(previous == ING_IDENTIFIER) {
                        if(Identifier.contains (identifiedText)) {
                            createDuality (identifiedText,"-");
                        }
                        else 
                            createDuality ("id",QString::number (table->appendId(identifiedText)));
                    }
                    else if(previous == ING_OPERATOR_TWO) {
                        identifiedText.append (oneChar);
                        createDuality (identifiedText,"-");
                    }
                    else if(previous == ING_REMARK){
                        identifiedText.append (oneChar);
                        //ignore
                        //qDebug ()<<identifiedText<<"remark";
                    }
                    else if(previous == ING_NEW_LINE) {
                        //do nothing
                    }
                    else {
                        identifiedText.append (oneChar);
                        initCError ("DF002",currentLine,project,identifiedText);
                    }
                    
                    state = NONE_S;
                    result = NONE_R;
                    identifiedText.clear ();
                }
                else if(oneChar.toAscii () != ' ' && oneChar.toAscii () != '\t' && oneChar.toAscii () != '\n'){
                    //add the char
                    identifiedText.append (oneChar);
                }
                
                if(oneChar.toAscii () == '\n') {
                    currentLine ++;
                    if(result != END_NEEDED) {
                        state = NONE_S;
                        previous = NONE_S;
                        identifiedText.clear();
                    }
                    else {
                        identifiedText.append (oneChar);
                    }
                }
            }
            //make sure the remakr have end
            if(result != MATCHED) {
                identifiedText = identifiedText.left (20);
                if(state == ING_REMARK){
                    initCError ("DF010",currentLine,project,"remark end needed!");
                }
                else if(state == ING_CHAR){
                    initCError ("DF010",currentLine,project," \'\"\' as the end of the string " + identifiedText + " needed!");
                }
                else if(state == ING_STRING ){
                     initCError ("DF010",currentLine,project," \"\'\" as the end of the string "+ identifiedText + " end needed!");
                }
            }
        }
    }
    else {
        QMessageBox::warning(0,tr("open"),tr("Can't open file"));
    }
}






void Compiler::isWhat (const QChar oneChar,long *i) {
    if(state == ING_MAYBE_REMARK) {
        //end
        //begin
        if(oneChar.toAscii () == '/') {
            state = ING_REMARK;
            result = MATCHED;
            return;
        }
        else if(oneChar.toAscii () == '*') {
            state = ING_REMARK;
            result = END_NEEDED;
            return;
        }
        else  {
            state = ING_OPERATOR_TWO;
        }
    }
    if(state == ING_IDENTIFIER) {    //match identifier//标识符
        if((oneChar.toAscii () >= 'a' && oneChar.toAscii () <= 'z') || (oneChar.toAscii () >= 'A' && oneChar.toAscii ()<= 'Z') || (oneChar.toAscii () > '0' && oneChar.toAscii () <= '9') || oneChar.toAscii () == '_') {
            result = NONE_R;
            state = ING_IDENTIFIER;
        }
        else {
            previous = ING_IDENTIFIER;
            state = END;
            result = MATCHED;
            if(oneChar.toAscii () == '\n')
                currentLine --;
            (*i) --;
        }
    }
    //match '\''
    else if(state == ING_CHAR) {
        if(result == SKIP) {
            result = END_NEEDED;
        }
        else if(oneChar.toAscii () == '\\') {
            result = SKIP;
        }
        else if(oneChar.toAscii () == '\'') {
            state = END;
            previous = ING_CHAR;
            result = MATCHED;
        }
    }
    //match '"'
    else if(state == ING_STRING) {
        if(result == SKIP) {
            result = END_NEEDED;
        }
        else if(oneChar.toAscii () == '\\') {
            result = SKIP;
        }
        else if(oneChar.toAscii () == '"') {
            state = END;
            previous = ING_STRING;
            result = MATCHED;
        }
    }
    // a = -5; a = 1 - 5;
    //match digital
    else if(state == ING_DIGIT) {
        if(oneChar.toAscii () >= '0' && oneChar.toAscii () <= '9'){
            state = ING_DIGIT;
           // result = NONE_R;
        }
        else if(oneChar.toAscii () == '.'){
            if(result != DOT_MATCHED){
                result = DOT_MATCHED;
                qDebug()<<result;
            }
            else {
                previous = ING_DIGIT;
                state = END;
                result = ERROR;
                if(oneChar.toAscii () == '\n')
                    currentLine --;
                (*i) --;
            }
        }
        else {
            previous = ING_DIGIT;
            state = END;
            result = MATCHED;
            if(oneChar.toAscii () == '\n')
                currentLine --;
            (*i) --;
        }
    }
    //match operator
    else if(state == ING_OPERATOR_TWO) {
        if(oneChar.toAscii () == '-' || oneChar.toAscii () == '+'  || oneChar.toAscii () == '&'  || oneChar.toAscii () == '=' || oneChar.toAscii () == '|') {
            state = ING_OPERATOR_TWO;
            previous = NONE_S;
            result = MATCHED;
        }
        else {
            previous = ING_OPERATOR_TWO;
            state = END;
            result = MATCHED;
            (*i) --;
        }
    }
    else if(state == ING_OPERATOR_THREE) {
        if(oneChar.toAscii () == '.' || oneChar.toAscii () == '<' || oneChar.toAscii () == '>' || oneChar.toAscii () == '='){
            state = ING_OPERATOR_THREE;
        }
    }
    //match // or /* */
    else if(state == ING_REMARK) {
        //with end or without end
        if(result == END_NEEDED) {
            if(oneChar.toAscii () == '*')
                result = MAY_END;
        }
        else if(result == MAY_END ){
            if(oneChar.toAscii () == '/') {
                state = END;
                previous = ING_REMARK;
                result = MATCHED;
            }
            else if(oneChar.toAscii () != '*')
                result = END_NEEDED;
        }
    }
    else if(state == NONE_S) {
        //here begin the judgement
        if(oneChar.toAscii () == '\'') {
            state = ING_CHAR;
            result = END_NEEDED;
        }
        else if(oneChar.toAscii () == '"') {
            state = ING_STRING; //means need the end of IND_STRING_DOUBLE
            result = END_NEEDED;
        }
        else if((oneChar.toAscii () >= 'a' && oneChar.toAscii () <= 'z') || (oneChar.toAscii () >= 'A' && oneChar.toAscii ()<= 'Z') || oneChar.toAscii () == '_') {
            state = ING_IDENTIFIER;
            result = NONE_R;
        }
        else if(oneChar.toAscii () >= '0' && oneChar.toAscii () <= '9') {
            state = ING_DIGIT;
            result = NONE_R;
        }
        else if(oneChar.toAscii () == '/') {
            // maybe / or // or /*  */
            state = ING_MAYBE_REMARK;
            result = NONE_R;
        }
        // first type
        else if(oneChar.toAscii () == ';' || oneChar.toAscii () == ','  || oneChar.toAscii () == '('  || oneChar.toAscii () == ')'  || oneChar.toAscii () == '['  || oneChar.toAscii () == ']'  || oneChar.toAscii () == '{'  || oneChar.toAscii () == '}' || oneChar.toAscii () == '?' || oneChar.toAscii () == ':' || oneChar.toAscii () == '~') {
            state = END;
            previous = ING_OPERATOR_ONE;
            result = MATCHED;
        }
        //second type except 
        else if(oneChar.toAscii () == '-' || oneChar.toAscii () == '+' || oneChar.toAscii () == '*' || oneChar.toAscii () == '%' || oneChar.toAscii () == '^'  || oneChar.toAscii () == '&'  || oneChar.toAscii () == '=' || oneChar.toAscii () == '!' || oneChar.toAscii () == '|') {
           // identifiedText.append (oneChar);
            state = ING_OPERATOR_TWO;
            result = NONE_R;
        }
        //third type
        else if(oneChar.toAscii () == '.' || oneChar.toAscii () == '<' || oneChar.toAscii () == '>') {
            state = ING_OPERATOR_THREE;
            result = NONE_R;
        }
        else if(oneChar.toAscii () != ' ' && oneChar.toAscii () != '\t' && oneChar.toAscii () != '\n' && oneChar.toAscii () != '\r'){
            result = ERROR;
            state = END;
            previous = NONE_S;
        }
    }
    else {
        result = ERROR;
        //error char
    }
}

void Compiler::initCError(QString type,long line,QString fileName,QString detail) {
    type.detach ();
    fileName.detach ();
    detail.detach ();
    error = new CError;
    error->setErrorType (type);
    error->setErrorLine (line);
    error->setErrorFile (fileName);
    error->setErrorDetail (detail);
    emit emitError (error);
}
