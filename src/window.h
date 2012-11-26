#ifndef Window_H
#define Window_H

#define RECENT_MAX 10
#define CONFIG_ORG "text-editor"
#define CONFIG_APP "Window Editor"
#define CONFIG_GROUP "config"
#define CONFIG_RECENT "recent"
#define NEW_FILE tr("untitled")

#include "exception.h"
#include "compiler.h"
#include "codeEditor.h"
#include "treeview.h"
//class XMLReader {
//public :
//    XMLReader();
//private :
//    void parseEntry(const QDomElement &element);
//    //there type of node
//    QSet <QString> Identifier;
//    QHash <QString,QString> cError;
//};




/*
    设计编译器的界面功能
    包括菜单，项目栏，编译输出栏，工具栏...
 */
class Window : public QMainWindow {
    Q_OBJECT
public :
    Window(QWidget *parent = 0);
    CError *error;
    QTreeWidget *treeWidget;
   // TreeView *projectList;
    ExceptionManagement  *exception;
private :
    Compiler *compiler;//store the compiler object

    //store the toolbar
    QHash <QString,QWidget *> *fileStack;
    CodeEditor *currentEditor;
    QString currentFileName;
    bool isUntitled;
    
    //menubar    
    QMenu *menuFile;
    QAction *actFileNew;
    QAction *actFileOpen;
    QMenu *menuFileOpenRecent;
    QAction *actRecent[RECENT_MAX];
    QAction *actRecentClear;
    QAction *actFileSave;
    QAction *actFileSaveAs;
    QAction *actFileClose;
    QAction *actFileExit;
        
    QMenu *menuEdit;
    QAction *actEditUndo;
    QAction *actEditRedo;
    QAction *actEditCopy;
    QAction *actEditCut;
    QAction *actEditPaste;
    QAction *actEditSelectAll;
    QAction *actEditFind;
    QAction *actEditReplace;
    QMenu *menuEditCharacterCase;
    QAction *actCharacterUpperCase;
    QAction *actCharacterLowerCase;
    QAction *actCharacterInvertCase;
    QAction *actCharacterTitleCase;
    
    QAction *actEditPreferences;
        
    QMenu *menuView;
    QAction *actViewWordWrap;
    QAction *actViewLineNumber;
    QAction *actViewRunSeparator;
//        QAction *actViewRunMenu;
//        QAction *actViewRunOutputDock;
//        QAction *actViewToolBarRun;
    QAction *actViewToolBar;
    QAction *actViewStatusBar;
    //this must be connected to the toolmenu 
    //and this is seted in preferences
    //as enable Run checkbox
//        QAction *actViewFont;
//        QAction *actViewColor;
    //add here the pop up menu to custom the toolbar
    QAction *actViewZoomIn;
    QAction *actViewZoomOut;
    QMenu *menuViewHighlight;
    
    //menu tools     
    //if this is plain text disable Color
    QMenu *menuTool;
    QAction *actToolFont;
    QAction *actToolColor;
//        QMenu *menuRun;
//            QAction *actRunRun;
//            QAction *actRunDebug;
//            QAction *actRunClean;
//            QAction *actRunSyntax;
//            QAction *actRunScanner;
        

    //run
    QMenu *menuRun;
    QAction *actRunRun;
    QAction *actRunMorphologicalAnalysis;
    
    
    QMenu *menuHelp;
    QAction *actHelpAboutQt;
    QAction *actHelpAbout;
    
    QToolBar *toolBarFileEdit;
//    QToolBar *toolBarRun;
    QToolBar *toolBarFontColor;
    
    QTabWidget *centerTab;
    QStatusBar *currentStatus;
    QLabel *statusLabelRowCol;
    QLabel *statusLabelModel;
    QLabel *statusLabelOperator;
    QLabel *dockLabelException;
    QDockWidget *dockException;
    QDockWidget *dockDirList;
    //here may be not need stack to story all the tabs
    
    //toolbar
    QToolBar *toolBar;
    QAction *actToolBarNew;
    QAction *actToolBarOpen;
    QAction *actToolBarSave;
    QAction *actToolBarUndo;
    QAction *actToolBarCut;
    QAction *actToolBarPaste;
    QAction *actToolBarRedo;
    QAction *actToolBarCopy;
    QAction *actToolBarFind;
    QAction *actToolBarReplace;
    QWidget *actToolBarFont;
    QWidget *actToolBarColor;
    
    //functions
    void closeEvent (QCloseEvent *); 
    void readConfig();
    void writeConfig();
    void newTab(const QString &fileName,int flag=0);
    void titleOrUntitleed(bool flag);
    

    bool initCompiler(QString project,COMPILERLEVEL level);
    void initMenu();    
    void initHighlightMode();
    void initToolBar();
    void initCentralWidget();
    void initStatusBar();
    void initDockException();
    void initDockDirList();
    void initConnect();
    void initPreferences();
    void initRecentFile();//init or update recentfile list
    
    
    bool saveFile(const QString &);
    //bool loadFile (QFile *file,CodeEditor *textEdit) ;
    void whetherSaved();
    //add current file to recentfile list
    void setRecentFile(const QString &fileName);
public slots:
    void exceptionLabel(int e,int w,int o);
    void runMenuEnable(bool enable);
    void autoOpenFile(const QString fileName);
private slots :
    void setCurrentFile(int index);
    void closeTab(int index);
    void setCurrentRowCol(int row = 1,int col = 1);
    
    //file
    void _fileNew();
    void _fileOpen();
    void _fileRecentOpen();
    void _fileRecentClear();
    void _fileSave();
    void _fileSaveAs();
    void _fileClose();
    void _fileExit();
    //edit
    void _editUndo();
    void _editRedo();
    void _editCopy();
    void _editCut();
    void _editPaste();
    void _editSelectAll();
    void _editFind();
    void _editReplace();
    void _editCharacterCase();
    void _editPreferences();
    //view
    void _viewWordWrap();
    void _viewLineNumber();
    void _viewToolBar();
    void _viewStatusBar();
    void _viewZoomIn();
    void _viewZoomOut();
    void _viewHighlight();
    //tool
    void _toolFont();
    void _toolColor();
    //run
    void _runRun();
    void _runMorphologicalAnalysis();
    //help
    void _helpAboutQt();
    void _helpAbout();

signals:
    void emitExceptionClear();
};
#endif // Window_H
