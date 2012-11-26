//main
#include "window.h"
#include <QtGui>
#include <QApplication>

int main(int argc,char **argv) {
    qRegisterMetaType<CError>("CError&");
    qRegisterMetaType<SError>("SError&");
    
    //QApplication::setStyle ("cleanlooks");
    
    QApplication app(argc,argv);
    Q_INIT_RESOURCE(window);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    Window *mainWindow = new Window;
  //  mainWindow->setAttribute (Qt::WA_ShowModal,true);
   // mainWindow->setWindowOpacity (1);
//    mainWindow->setWindowFlags (Qt::FramelessWindowHint);
//    mainWindow->setAttribute (Qt::WA_TranslucentBackground);
    
    QStringList args = app.arguments();
    if(args.count () > 1) {
        for(int i = 1;i < args.count();i ++)
            mainWindow->autoOpenFile (args[i]);
    }
    mainWindow->show();
    return app.exec ();
}
