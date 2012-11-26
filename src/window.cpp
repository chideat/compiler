#include <QtGui>
#include "window.h"

Window::Window(QWidget *parent) :QMainWindow(parent) {
    readConfig ();
    initMenu ();
    initStatusBar ();
    initDockException ();
    initDockDirList ();
    initToolBar ();
    initCentralWidget ();
    initPreferences ();
    
    fileStack = new QHash<QString,QWidget *>;
    exception = new ExceptionManagement(this);
    compiler = new Compiler(exception);
    
    exception->start ();
   //start exception thread
    
    resize (800,600);
    move(290,30);
    initConnect ();
}

/****************************************************************************************************************/
//init
bool Window::initCompiler (QString project, COMPILERLEVEL level){
    emit emitExceptionClear ();
    dockException->setVisible (true);
    if(!project.isEmpty () && compiler->setLevel (level) && compiler->setProject (project)) {
        return true;
    }
    return false;
}

void Window::initMenu () {
    //file
    menuFile = new QMenu(tr("File"),this);
    actFileNew = menuFile->addAction (QIcon(":/16X16/new.png"),tr("New"));
    actFileNew->setShortcut (QKeySequence::New);
    actFileOpen = menuFile->addAction (QIcon(":/16X16/open.png"),tr("Open"));
    actFileOpen->setShortcut (QKeySequence::Open);
    menuFile->addSeparator ();
    menuFileOpenRecent = menuFile->addMenu (QIcon(":/16X16/recent.png"),tr("Open Recent"));
    for(int i = 0;i < RECENT_MAX;i ++) {
        actRecent[i] = new QAction(this);
        menuFileOpenRecent->addAction (actRecent[i]);
        actRecent[i]->setVisible (false);
    }
    menuFileOpenRecent->addSeparator ();
    actRecentClear = menuFileOpenRecent->addAction (QIcon(":/16X16/clear.png"),tr("Clear"));
    menuFile->addSeparator ();
    actFileSave = menuFile->addAction (QIcon(":/16X16/save.png"),tr("Save"));
    actFileSave->setShortcut (QKeySequence::Save);
    actFileSaveAs = menuFile->addAction (QIcon(":/16X16/saveas.png"),tr("Save As"));
    actFileSaveAs->setShortcut (QKeySequence::SaveAs);
    menuFile->addSeparator ();
    actFileClose = menuFile->addAction (QIcon(":/16X16/close.png"),tr("Close"));
    actFileClose->setShortcut (QKeySequence::Close);
    actFileExit = menuFile->addAction (QIcon(":/16X16/exit.png"),tr("Exit"));
    actFileExit->setShortcut (QKeySequence::Quit);
        
    //edit
    menuEdit = new QMenu(tr("Edit"),this);
    actEditUndo = menuEdit->addAction (QIcon(":/16X16/undo.png"),tr("Undo"));
    actEditUndo->setShortcut (QKeySequence::Undo);
    actEditRedo = menuEdit->addAction (QIcon(":/16X16/redo.png"),tr("Redo"));
    actEditRedo->setShortcut (QKeySequence::Redo);
    menuEdit->addSeparator ();
    actEditCopy = menuEdit->addAction (QIcon(":/16X16/copy.png"),tr("Copy"));
    actEditCopy->setShortcut (QKeySequence::Copy);
    actEditCut = menuEdit->addAction (QIcon(":/16X16/cut.png"),tr("Cut"));
    actEditCut->setShortcut (QKeySequence::Cut);
    actEditPaste = menuEdit->addAction (QIcon(":/16X16/paste.png"),tr("Paste"));
    actEditPaste->setShortcut (QKeySequence::Paste);
    menuEdit->addSeparator ();
    actEditSelectAll = menuEdit->addAction (QIcon(":/16X16/selectall.png"),tr("Select All"));
    actEditSelectAll->setShortcut (QKeySequence::SelectAll);
    menuEdit->addSeparator ();
    actEditFind = menuEdit->addAction (QIcon(":/16X16/find.png"),tr("Find..."));
    actEditFind->setShortcut (QKeySequence::Find);
    actEditFind->setCheckable (true);
    actEditReplace = menuEdit->addAction (QIcon(":/16X16/replace"),tr("Replace..."));
    actEditReplace->setShortcut (QKeySequence::Replace);
    actEditReplace->setCheckable (true);
    menuEditCharacterCase = menuEdit->addMenu (tr("Character Case"));
    actCharacterUpperCase = menuEditCharacterCase->addAction (tr("All Upper Case"));
    actCharacterLowerCase = menuEditCharacterCase->addAction (tr("All Lower Case"));
    actCharacterInvertCase = menuEditCharacterCase->addAction (tr("Invert Case"));
    actCharacterTitleCase = menuEditCharacterCase->addAction (tr("Title Case"));
    actEditPreferences = menuEdit->addAction (QIcon(":/16X16/preferences.png"),tr("Preferences"));
    
    //view
    menuView = new QMenu(tr("View"),this);
    actViewWordWrap = menuView->addAction (tr("Word Wrap"));
    actViewWordWrap->setShortcut (QKeySequence("F10"));
    actViewWordWrap->setCheckable (true);
    actViewLineNumber = menuView->addAction (tr("Line Number"));
    actViewLineNumber->setShortcut (QKeySequence("F11"));
    actViewLineNumber->setCheckable (true);
    
    actViewRunSeparator = new QAction(this);
    actViewRunSeparator->setSeparator (true);
    menuView->addAction (actViewRunSeparator);
    
    actViewToolBar = menuView->addAction (tr("ToolBar"));
    actViewToolBar->setCheckable (true);
    actViewStatusBar = menuView->addAction (tr("StatusBar"));
    actViewStatusBar->setCheckable (true);
    menuView->addSeparator ();
    //add the with the mouse roller wheel short cut
    actViewZoomIn = menuView->addAction (QIcon(":/16X16/zoomin.png"),tr("Zoom In"));
    actViewZoomIn->setShortcut (QKeySequence("Ctrl+Up"));
    actViewZoomOut = menuView->addAction ((QIcon(":/16X16/zoomout.png")),tr("Zoom Out"));
    actViewZoomOut->setShortcut (QKeySequence("Ctrl+Down"));
    menuView->addSeparator ();
    menuViewHighlight = menuView->addMenu (tr("Highlight Mode"));
    //after need to init highlight menu
    
    //tool
    menuTool = new QMenu(tr("Tool"),this);
    actToolFont = menuTool->addAction (tr("Font"));
    actToolColor = menuTool->addAction (tr("Color"));
    
    //run
    menuRun = new QMenu(tr("Run"),this);
    actRunRun = menuRun->addAction (tr("Run"));
    actRunRun->setShortcut (QKeySequence("F5"));
    actRunMorphologicalAnalysis = menuRun->addAction (tr("Morphological Analysis"));
    actRunMorphologicalAnalysis->setShortcut (QKeySequence("F6"));
    
    //help
    menuHelp = new QMenu(tr("Help"),this);
    actHelpAboutQt = menuHelp->addAction (QIcon(":/16X16/aboutqt.png"),tr("About Qt"));
    menuHelp->addSeparator ();
    actHelpAbout = menuHelp->addAction (QIcon(":/16X16/about.png"),tr("About Rich Window Editor"));
    actHelpAbout->setShortcut (QKeySequence("F1"));
    
    menuBar ()->addMenu (menuFile);
    menuBar ()->addMenu (menuEdit);
    menuBar ()->addMenu (menuView);
    menuBar ()->addMenu (menuTool);
    menuBar ()->addMenu (menuRun);
    menuBar ()->addMenu (menuHelp);
}

void Window::initHighlightMode() {
    
}

void Window::initToolBar () {
    //addToolbar item
    toolBar = new QToolBar(this);
    toolBar->setWindowTitle (tr("Tool Bar"));
    
    toolBar->setAllowedAreas(Qt::AllToolBarAreas);
    toolBar->setOrientation(Qt::Horizontal);
    
    actToolBarNew = toolBar->addAction (QIcon(":/24X24/new.png"),tr("New"));
    actToolBarOpen = toolBar->addAction (QIcon(":/24X24/open.png"),tr("Open"));
    actToolBarSave = toolBar->addAction (QIcon(":/24X24/save.png"),tr("Save"));
    toolBar->addSeparator ();
    actToolBarCopy = toolBar->addAction (QIcon(":/24X24/copy.png"),tr("Copy"));
    actToolBarCut = toolBar->addAction (QIcon(":/24X24/cut.png"),tr("Cut"));
    actToolBarPaste = toolBar->addAction (QIcon(":/24X24/paste.png"),tr("Paste"));
    toolBar->addSeparator ();
    actToolBarUndo = toolBar->addAction (QIcon(":/24X24/undo.png"),tr("Undo"));
    actToolBarRedo = toolBar->addAction (QIcon(":/24X24/redo.png"),tr("Redo"));
    toolBar->addSeparator ();
    actToolBarFind = toolBar->addAction (QIcon(":/24X24/find.png"),tr("Find"));
    actToolBarFind->setCheckable (true);
    actToolBarReplace = toolBar->addAction (QIcon(":/24X24/replace.png"),tr("Replace"));
    actToolBarReplace->setCheckable (true);
   // toolBar->addSeparator ();
   // toolBar->addWidget ();
    addToolBar(Qt::TopToolBarArea, toolBar);
    toolBar->setVisible(true);
}

void Window::initCentralWidget () {
    centerTab = new QTabWidget(this);
//    currentEditor = new CodeEditor(this);
//    centerTab->addTab (currentEditor,QIcon(":/16X16/text.png"),NEW_FILE);
//    centerTab->setTabText (centerTab->indexOf (currentEditor),NEW_FILE);
    centerTab->setTabsClosable (true);
    centerTab->setTabShape (QTabWidget::Rounded);
    
    setCentralWidget (centerTab);
//    setWindowTitle(NEW_FILE + "[*]");
//    currentEditor->setWindowModified (false);
//    setWindowModified (false);
//    statusLabelRowCol->setHidden (true);
//    statusLabelModel->setHidden (true);
//    statusLabelOperator->setHidden (true);
}

void Window::initStatusBar () {
    currentStatus = statusBar();
    statusLabelRowCol = new QLabel(tr("Ln 1,Col 1"),this);
    statusLabelModel = new QLabel(tr("INC"),this);
    statusLabelOperator = new QLabel(tr("Success"),this);
    
    currentStatus->addWidget (statusLabelRowCol,2);
    currentStatus->addWidget (statusLabelOperator,2);
    currentStatus->addWidget (statusLabelModel,1);
}

void Window::initDockException () {
    QWidget *widget = new QWidget;
    QVBoxLayout *vBoxLayout = new QVBoxLayout(widget);
    treeWidget = new QTreeWidget(widget);
    dockLabelException = new QLabel(widget);
    exceptionLabel(0,0,0);
   
    treeWidget->setColumnCount (5);
    treeWidget->setHeaderLabels (QStringList()<<tr("File")<<tr("Line")<<tr("Type")<<tr("Detail")<<tr("..."));
    treeWidget->header ()->setResizeMode (0,QHeaderView::Interactive);
    treeWidget->header ()->setResizeMode (1,QHeaderView::Interactive);
    treeWidget->header ()->setResizeMode (3,QHeaderView::Interactive);
    treeWidget->header ()->setResizeMode (4,QHeaderView::Interactive);
    treeWidget->setSortingEnabled (true);
    treeWidget->sortByColumn (0);
    
    vBoxLayout->setMargin (5);
    vBoxLayout->setSpacing (0);
    vBoxLayout->addWidget (dockLabelException,0);
    vBoxLayout->addWidget (treeWidget,10);
    
    
    dockException = new QDockWidget(tr("Result"));
    dockException->setWidget (widget);
    dockException->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    dockException->setFeatures (QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetVerticalTitleBar | QDockWidget::DockWidgetFloatable);
    dockException->setFloating (true);
    dockException->setVisible (false);
    addDockWidget (Qt::BottomDockWidgetArea,dockException);
}

void Window::initDockDirList () {
//    projectList = new TreeView;
//    dockDirList = new QDockWidget;
//    dockDirList->setWidget (projectList);
//    dockDirList->setAllowedAreas (Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
//    dockDirList->setFeatures (QDockWidget::AllDockWidgetFeatures);
//    dockDirList->setFloating (true);
//    dockDirList->setVisible (true);
//    addDockWidget (Qt::LeftDockWidgetArea,dockDirList);
}


void Window::initConnect () {    
    //file
    connect (actFileNew,SIGNAL(triggered()),this,SLOT(_fileNew()));
    connect(actFileOpen,SIGNAL(triggered()),this,SLOT(_fileOpen()));
    connect(actRecentClear,SIGNAL(triggered()),SLOT(_fileRecentClear()));
    for(int i = 0;i < RECENT_MAX;i ++) {
        connect(actRecent[i],SIGNAL(triggered()),this,SLOT(_fileRecentOpen()));
    }
    connect (actFileSave,SIGNAL(triggered()),this,SLOT(_fileSave()));
    connect (actFileSaveAs,SIGNAL(triggered()),this,SLOT(_fileSaveAs()));
    connect (actFileClose,SIGNAL(triggered()),this,SLOT(_fileClose()));
    connect (actFileExit,SIGNAL(triggered()),this,SLOT(_fileExit()));
    //edit
    connect(actEditUndo,SIGNAL(triggered()),this,SLOT(_editUndo()));
    connect(actEditRedo,SIGNAL(triggered()),this,SLOT(_editRedo()));
    connect(actEditCopy,SIGNAL(triggered()),this,SLOT(_editCopy()));
    connect(actEditCut,SIGNAL(triggered()),this,SLOT(_editCut()));
    connect(actEditPaste,SIGNAL(triggered()),this,SLOT(_editPaste()));
    connect(actEditSelectAll,SIGNAL(triggered()),this,SLOT(_editSelectAll()));
    connect(actEditFind,SIGNAL(triggered()),this,SLOT(_editFind()));
    connect(actEditReplace,SIGNAL(triggered()),this,SLOT(_editReplace()));
    connect(actEditPreferences,SIGNAL(triggered()),this,SLOT(_editPreferences()));
    //view
    connect(actViewWordWrap,SIGNAL(triggered()),this,SLOT(_viewWordWrap()));
    connect(actViewLineNumber,SIGNAL(triggered()),this,SLOT(_viewLineNumber()));
    connect(actViewToolBar,SIGNAL(triggered()),this,SLOT(_viewToolBar()));
    connect (actViewStatusBar,SIGNAL(triggered()),this,SLOT(_viewStatusBar()));
    connect(actViewZoomIn,SIGNAL(triggered()),this,SLOT(_viewZoomIn()));
    connect(actViewZoomOut,SIGNAL(triggered()),this,SLOT(_viewZoomOut()));
    //tool
    connect (actToolFont,SIGNAL(triggered()),this,SLOT(_toolFont()));
    connect(actToolColor,SIGNAL(triggered()),this,SLOT(_toolColor()));
    //run
    connect(actRunRun,SIGNAL(triggered()),this,SLOT(_runRun()));
    actRunRun->setShortcut (QKeySequence("F5"));
    connect(actRunMorphologicalAnalysis,SIGNAL(triggered()),this,SLOT(_runMorphologicalAnalysis()));
    actRunMorphologicalAnalysis->setShortcut (QKeySequence("F9"));
    
    //help
    connect(actHelpAboutQt,SIGNAL(triggered()),this,SLOT(_helpAboutQt()));
    connect (actHelpAbout,SIGNAL(triggered()),this,SLOT(_helpAbout()));
    
    
    //toolbar
    connect(actToolBarNew,SIGNAL(triggered()),this,SLOT(_fileNew()));
    connect(actToolBarOpen,SIGNAL(triggered()),this,SLOT(_fileOpen()));
    connect(actToolBarSave,SIGNAL(triggered()),this,SLOT(_fileSave()));
    connect(actToolBarUndo,SIGNAL(triggered()),this,SLOT(_editUndo()));
    connect(actToolBarRedo,SIGNAL(triggered()),this,SLOT(_editRedo()));
    connect(actToolBarCopy,SIGNAL(triggered()),this,SLOT(_editCopy()));
    connect(actToolBarCut,SIGNAL(triggered()),this,SLOT(_editCut()));
    connect(actToolBarPaste,SIGNAL(triggered()),this,SLOT(_editPaste()));
//    connect(actToolBarFind,SIGNAL(triggered()),this,SLOT(_editFind()));
//    connect(actToolBarReplace,SIGNAL(triggered()),this,SLOT(_editReplace()));
//    connect(actToolBarFont,SIGNAL(triggered()),this,SLOT(_toolFont()));
//    connect(actToolBarColor,SIGNAL(triggered()),this,SLOT(_toolColor()));
    
    connect (centerTab,SIGNAL(currentChanged(int)),this,SLOT(setCurrentFile(int)));
    connect(centerTab,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
    connect(this,SIGNAL(emitExceptionClear()),exception,SLOT(clear()));
}

void Window::initPreferences () {
    
    //whether load the plugins
  //  initPlugins ();
}





void Window::initRecentFile () {
    QSettings settings(CONFIG_ORG,CONFIG_APP);
    QStringList recentFile = settings.value (CONFIG_RECENT).toStringList ();
    
    int numRecentFiles = qMin(recentFile.size(),(int)RECENT_MAX);
    for(int i = 0;i < numRecentFiles;i ++) {
        QString text = tr("&%1  %2").arg(i + 1).arg(recentFile[i]);
        actRecent[i]->setText(text);
        actRecent[i]->setData(recentFile[i]);
        actRecent[i]->setVisible(true);
      }
    for(int i = numRecentFiles;i < RECENT_MAX; i ++)
        actRecent[i]->setVisible(false);
}



/****************************************************************************************************************/
//file
void Window::_fileNew () {
    newTab (NEW_FILE,1);
}

void Window::_fileOpen () {  
    QString fileName = QFileDialog::getOpenFileName(this,tr("File Open"),"~/",tr("C/C++ Files (*.c *.cpp *.h);;Projects(*.pro);;All Files (*)"));
    if(!fileName.isEmpty()) {
        //check if exists
        if(!fileStack->contains(fileName)) {
            newTab(fileName);
            fileStack->insert (fileName,currentEditor);
        }
        else 
            currentEditor = (CodeEditor *)fileStack->value (fileName);
        
        centerTab->setCurrentWidget (currentEditor);
        titleOrUntitleed (false);
        currentFileName = fileName;
     }
    else {
        //error
    }
}

void Window::_fileRecentOpen () {
    QAction* action = qobject_cast<QAction*>(sender());
    if(action) {
        newTab (action->data ().toString ());
    }
}

void Window::_fileRecentClear () {
    QSettings settings(CONFIG_ORG,CONFIG_APP);
    settings.remove(CONFIG_RECENT);
    initRecentFile ();
}

void Window::_fileSave () {
    if(isUntitled) {
        _fileSaveAs ();
    }
    else
        saveFile (currentFileName);
}

void Window::_fileSaveAs () {
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save As"),"~/",tr("C/C++ Files (*.c *.cpp *.h);;Projects(*.pro);;All Files (*)"));
    if(!fileName.isEmpty())
        saveFile(fileName);
}

void Window::_fileClose () {
    //close current tab
    centerTab->removeTab (centerTab->indexOf (currentEditor));
    currentEditor = (CodeEditor *)centerTab->currentWidget ();
}

void Window::_fileExit () {
    //share the same function with CloseEvent
    //save all dialog
    writeConfig ();
    qApp->quit ();
}
/****************************************************************************************************************/
//edit
void Window::_editUndo () {
    currentEditor->undo ();
}

void Window::_editRedo () {
    currentEditor->redo ();
}

void Window::_editCopy () {
    currentEditor->copy ();
}

void Window::_editCut () {
    currentEditor->cut ();
}

void Window::_editPaste () {
    currentEditor->paste ();
}

void Window::_editSelectAll () {
    currentEditor->selectAll ();
}

void Window::_editFind () {
    
}

void Window::_editReplace () {
    
}

void Window::_editCharacterCase () {
    //change selected charactes to upper lower or invert the string or set the selection in title case
    QAction * action = qobject_cast<QAction *>(sender());
    if(action == actCharacterUpperCase) {
        
    }
    else if(action == actCharacterLowerCase) {
        
    }
    else if(action == actCharacterInvertCase) {
        
    }
    else if(action == actCharacterTitleCase) {
        
    }
}



void Window::_editPreferences () {
    //connect to readconfig
}

/****************************************************************************************************************/
//view
void Window::_viewWordWrap () {
    if(!actViewWordWrap->isChecked())
      currentEditor->setLineWrapMode(CodeEditor::NoWrap);
    else
      currentEditor->setLineWrapMode(CodeEditor::WidgetWidth);
}

void Window::_viewLineNumber () {
    
}

void Window::_viewToolBar () {
    
}

void Window::_viewStatusBar () {
    currentStatus->setVisible (true);
}

void Window::_viewZoomIn () {
//    currentEditor->zoomIn (1);
}

void Window::_viewZoomOut () {
//    currentEditor->zoomOut (1);
}

void Window::_viewHighlight () {
    //create a menu here
}

/****************************************************************************************************************/
//tool
void Window::_toolFont () {
    
}

void Window::_toolColor () {
    
}


/****************************************************************************************************************/
//run
void Window::_runRun () {
    if(initCompiler (currentFileName,RUN))
        compiler->start ();
}

void Window::_runMorphologicalAnalysis () {
    if(initCompiler (currentFileName,MA))
        compiler->start ();
}



/****************************************************************************************************************/
//help
void Window::_helpAboutQt () {
    QMessageBox::aboutQt(this,tr(" About Qt"));
}

void Window::_helpAbout () {
    QMessageBox::about(this,tr("Rich Window Editor"),tr("<h2>Rich Window Editor</h2>\n于2012年2月17日开始设计"));
}




/****************************************************************************************************************/
void Window::autoOpenFile (const QString fileName) {
    if(!fileName.isEmpty()) {
        //check if exists
        if(!fileStack->contains(fileName)) {
            newTab(fileName);
            fileStack->insert (fileName,currentEditor);
        }
        else 
            currentEditor = (CodeEditor *)fileStack->value (fileName);
        
        centerTab->setCurrentWidget (currentEditor);
        titleOrUntitleed (false);
        currentFileName = fileName;
     }
    else {
        //error
    }
}



void Window::newTab (const QString &fileName,int flag) {
    CodeEditor *textEdit = new CodeEditor(this,treeWidget);    
    currentEditor = textEdit;
    connect (currentEditor,SIGNAL(emitCurrentLineNumChanged(int,int)),this,SLOT(setCurrentRowCol(int,int)));
    setCurrentRowCol();
    
    if(flag == 0){
        //set label
        statusLabelOperator->setText (tr("Opening"));
        //this fun use to load the new file and to make sure whether needed to create a new tab
        QFile *file = new QFile(fileName);
        QFileInfo *fileInfo = new QFileInfo(fileName);
        if(file->open (QFile::ReadOnly |QFile::Text)) {
            QTextStream in(file);
            QApplication::setOverrideCursor(Qt::WaitCursor);
            centerTab->addTab (currentEditor,QIcon(":/16X16/text.png"),fileInfo->fileName ());
            
            currentEditor->setPlainText (in.readAll ());
            QApplication::restoreOverrideCursor();
            statusLabelOperator->setText(tr("Sucess"));
            titleOrUntitleed (false);
            file->close ();
        }
        else {
            statusLabelOperator->setText (tr("Failed"));
            delete textEdit;
            QMessageBox::warning(this,tr("Error"),tr("Can't read %1:\n%2.").arg(fileName).arg(file->errorString()));
        }
    }
    else {
        centerTab->addTab (currentEditor,QIcon(":/16X16/text.png"),fileName);
        titleOrUntitleed (true);
    }
}

bool Window::saveFile (const QString &fileName) {
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this,tr("Save"),tr("Can't save %1:\n %2.").arg(fileName).arg(file.errorString()));
        return false;
      }
    QTextStream *out = new QTextStream(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    (*out) << currentEditor->toPlainText ();
    QApplication::restoreOverrideCursor();
    setRecentFile (fileName);
    statusLabelOperator->setText(tr("Saved"));
  
    file.close ();
    
    QFileInfo *fileInfo = new QFileInfo(fileName);
    centerTab->setTabText (centerTab->indexOf (currentEditor),fileInfo->fileName ());
    
    titleOrUntitleed (false);
    currentFileName = fileName;
    
    fileStack->remove (fileStack->key (currentEditor));
    fileStack->insert (fileName,currentEditor);
    return true;
}

void Window::whetherSaved () {
    
}

void Window::exceptionLabel (int e, int w, int o) {
    QString tmp;
    if(e != 0)
        tmp = tr("<font color=\"red\">%1 errors</font>,%2 warnings,%3 others").arg (e).arg (w).arg (o);
    else 
         tmp = tr("%1 errors,%2 warnings,%3 others").arg (e).arg (w).arg (o);
    dockLabelException->setText (tmp);
}


void Window::setRecentFile(const QString &fileName) {
    
}

void Window::closeEvent (QCloseEvent *event) {
    //here need to check all the file's wheather all of them has being saved
    writeConfig ();
    event->accept ();
}

void Window::runMenuEnable (bool enable) {
    actRunRun->setEnabled (enable);
}


void Window::setCurrentFile (int index) {
    if(centerTab->count () == 0)
        return ;
    currentEditor =  (CodeEditor *)centerTab->currentWidget ();
    currentFileName = fileStack->key(currentEditor);
    if(currentFileName.compare ("") == 0)
        isUntitled = true;
    else 
        isUntitled = false;
    currentEditor->getCursorPositionChanged ();
}

void Window::titleOrUntitleed (bool flag)  {
    isUntitled = flag;
}

void Window::closeTab (int index) {
    if(isUntitled == false) {
        QWidget *tmp = centerTab->widget (index);
        fileStack->remove (fileStack->key (centerTab->widget(index)));
        delete tmp;
    }
    else
        centerTab->removeTab (index);
//    if(centerTab->count () == 0) {
//        statusLabelRowCol->setHidden (true);
//        statusLabelModel->setHidden (true);
//        statusLabelOperator->setHidden (true);
//    }
    currentEditor = NULL;
     setCurrentFile(0);
}

void Window::setCurrentRowCol (int row, int col) {
    statusLabelRowCol->setText (tr("Ln %1,Col %2").arg (row).arg (col));
}


void Window::readConfig () {
    
}

void Window::writeConfig () {
    QSettings settings(CONFIG_ORG,CONFIG_APP);
    settings.beginGroup (CONFIG_GROUP);
    //settings.setValue ();
    settings.endGroup ();
}





/*******************************************************************************************************************/
//XMLReader::XMLReader() {
//    QFile *keyWordFile = new QFile("./src/config/keyWords.xml");
//    QFile *exceptionFile = new QFile("./src/config/exceptionType.xml");
//    QString errorStr;
//    int errorLine;
//    int errorColumn;
//    QDomDocument doc;
//    //keyWords
//    if(!doc.setContent (keyWordFile,true,&errorStr,&errorLine,&errorColumn)) {
//        qDebug ()<<"error";
//        return;
//    }
//    QDomElement root = doc.documentElement ();
//    if(root.tagName () != "language")
//        return;
//    QDomNode node = root.firstChild ();
//    while(!node.isNull ()) {
//        if(node.toElement ().tagName () == "word")
//            Identifier << node.toText ().data ();
//        node = node.nextSibling ();
//    }
//    //exception type
//    if(!doc.setContent (exceptionFile,true,&errorStr,&errorLine,&errorColumn)) {
//        qDebug ()<<"error";
//        return;
//    }
//    root = doc.documentElement ();
//    if(root.tagName () != "exception")
//        return;
//    QDomNode nodeOne = root.firstChild ();
//    while(!nodeOne.isNull ()) {
//        if(nodeOne.toElement ().tagName () == "system") {
//            QDomNode nodeTwo  = nodeOne.firstChild ();
//            while(!nodeTwo.isNull ()) {
//                if(nodeTwo.toElement ().tagName () == "error") {
                    
//                }
//                else if(nodeTwo.toElement ().tagName () == "warning") {
                    
//                }
//                else if(nodeTwo.toElement ().tagName () == "other") {
                    
//                }
//            }
//        }
//        else if(nodeOne.toElement ().tagName () == "compiler") {
            
//        }
//        nodeOne = node.nextSibling ();
//    }
//}
