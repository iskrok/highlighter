#include <QtGui>
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QColorDialog>
#include <QComboBox>
#include <QFontComboBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QMenu>
#include <QMenuBar>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>
#include <QTextCodec>
#include <QTextEdit>
#include <QToolBar>
#include <QTextCursor>
#include <QTextDocumentWriter>
#include <QTextList>
#include <QtDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <QtPrintSupport/QPrintPreviewDialog>
#include "mainwindow.h"
#include "qstatusbar.h"
#include <QLabel>
#include <QDateTime>
#include <QGroupBox>
#include <QToolButton>
#include <QSplitter>
#include <QFontDialog>
#include <QVBoxLayout>
#include <QWidgetAction>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    textEdit = new CodeEditor(this);
    highlighter = new Highlighter(textEdit->document());
    colcur = new QLabel(tr("Column - 0"),this);
    rowcur = new QLabel(tr("Row - 1"),this);
    time = new QLabel(this);
    lines = new QLabel(this);
    words = new QLabel(this);
    chars = new QLabel(this);
    size = new QLabel(this);
    tb = new QToolBar(this);
    setToolButtonStyle(Qt::ToolButtonFollowStyle);
    setupFileAndEditActions();
    setupTextActions();
    setupViewActions();
    setupStatusBar();
    setCentralWidget(textEdit);
    textEdit->setFocus();
    setCurrentFileName(QString());
    menuBar()->addAction(tr("&Save Settings"), this, SLOT(save_set()));
    menuBar()->addAction(tr("&About"), this, SLOT(about()));
    connect(textEdit->document(), SIGNAL(modificationChanged(bool)),
            actionSave, SLOT(setEnabled(bool)));
    connect(textEdit->document(), SIGNAL(modificationChanged(bool)),
            this, SLOT(setTime()));
    connect(textEdit->document(),SIGNAL(contentsChanged()),
            this,SLOT(setTime()));
    connect(textEdit->document(), SIGNAL(modificationChanged(bool)),
            this, SLOT(setWindowModified(bool)));
    connect(textEdit->document(), SIGNAL(undoAvailable(bool)),
            actionUndo, SLOT(setEnabled(bool)));
    connect(textEdit->document(), SIGNAL(redoAvailable(bool)),
            actionRedo, SLOT(setEnabled(bool)));

    setWindowModified(textEdit->document()->isModified());
    actionSave->setEnabled(textEdit->document()->isModified());
    actionUndo->setEnabled(textEdit->document()->isUndoAvailable());
    actionRedo->setEnabled(textEdit->document()->isRedoAvailable());

    connect(actionUndo, SIGNAL(triggered()), textEdit, SLOT(undo()));
    connect(actionRedo, SIGNAL(triggered()), textEdit, SLOT(redo()));

    actionCut->setEnabled(false);
    actionCopy->setEnabled(false);
    actionWrap->setChecked(true);
    connect(actionCut, SIGNAL(triggered()), textEdit, SLOT(cut()));
    connect(actionCopy, SIGNAL(triggered()), textEdit, SLOT(copy()));
    connect(actionPaste, SIGNAL(triggered()), textEdit, SLOT(paste()));
    connect(actionFind, SIGNAL(triggered()), textEdit, SLOT(findtxt()));
    connect(actionFindAndReplace, SIGNAL(triggered()), textEdit, SLOT(findrepl()));
    connect(actionSelectAll, SIGNAL(triggered()), textEdit, SLOT(selectAll()));
    connect(actionWrap,SIGNAL(triggered(bool)),textEdit,SLOT(setWrap(bool)));
    connect(actionSettings,SIGNAL(triggered()),this,SLOT(settings()));

    connect(textEdit, SIGNAL(copyAvailable(bool)), actionCut, SLOT(setEnabled(bool)));
    connect(textEdit, SIGNAL(copyAvailable(bool)), actionCopy, SLOT(setEnabled(bool)));

}
void MainWindow::closeEvent(QCloseEvent *e)
{
    if (maybeSave())
        e->accept();
    else
        e->ignore();
}
void MainWindow::setupFileAndEditActions()
{

    tb->setWindowTitle(tr("ToolBar"));
    addToolBar(tb);
    tb->setMovable(false);

    QMenu *menu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(menu);

    QToolButton *find = new QToolButton(this);
    find->setPopupMode(QToolButton::MenuButtonPopup);
    find->setText("&Find");
    QIcon icon("zoomin.png");
    find->setIcon(icon);
    find->setText("&Find");

    QAction *a;
    icon = QIcon("filenew.png");
    a = new QAction(this);
    a->setIcon(icon);
    a->setIconText("New");
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::New);
    connect(a, SIGNAL(triggered()), this, SLOT(fileNew()));
    tb->addAction(a);
    menu->addAction(a);

    a = new QAction(this);
    icon = QIcon("fileopen.png");
    a->setIcon(icon);
    a->setIconText("Open");
    a->setShortcut(QKeySequence::Open);
    connect(a, SIGNAL(triggered()), this, SLOT(fileOpen()));
    tb->addAction(a);
    menu->addAction(a);

    menu->addSeparator();

    actionSave = a = new QAction(this);
    icon = QIcon("filesave.png");
    a->setIcon(icon);
    a->setIconText("Save");
    a->setShortcut(QKeySequence::Save);
    connect(a, SIGNAL(triggered()), this, SLOT(fileSave()));
    connect(a, SIGNAL(triggered()),this,SLOT(setLWCS()));
    a->setEnabled(false);
    tb->addAction(a);
    menu->addAction(a);

    a = new QAction(tr("Save &As..."), this);
    a->setPriority(QAction::LowPriority);
    connect(a, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
    menu->addAction(a);
    menu->addSeparator();

    a = new QAction(tr("&Quit"), this);
    a->setShortcut(Qt::CTRL + Qt::Key_Q);
    connect(a, SIGNAL(triggered()), this, SLOT(close()));
    menu->addAction(a);

    QMenu *menu2 = new QMenu(tr("&Edit"), this);
    menuBar()->addMenu(menu2);

    a = actionUndo = new QAction(this);
    icon = QIcon("editundo.png");
    a->setIcon(icon);
    a->setIconText("Undo");
    a->setShortcut(QKeySequence::Undo);
    tb->addAction(a);
    menu2->addAction(a);

    a = actionRedo = new QAction(this);
    icon = QIcon("editredo.png");
    a->setIcon(icon);
    a->setIconText("Redo");
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::Redo);
    tb->addAction(a);
    menu2->addAction(a);
    menu2->addSeparator();

    a = actionCut = new QAction(this);
    icon = QIcon("editcut.png");
    a->setIcon(icon);
    a->setIconText("Cut");
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::Cut);
    tb->addAction(a);
    menu2->addAction(a);

    a = actionCopy = new QAction(this);
    icon = QIcon("editcopy.png");
    a->setIcon(icon);
    a->setIconText("Copy");
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::Copy);
    tb->addAction(a);
    menu2->addAction(a);

    a = actionPaste = new QAction(this);
    icon = QIcon("editpaste.png");
    a->setIcon(icon);
    a->setIconText("Paste");
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::Paste);
    tb->addAction(a);
    menu2->addAction(a);

    a = actionFind = new QAction(tr("&Find"),this);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::Find);
    find->addAction(a);
    menu2->addAction(a);

    a = actionFindAndReplace = new QAction(tr("&Find and replace"), this);
    a->setPriority(QAction::LowPriority);
    find->addAction(a);

    menu2->addAction(a);
    a = actionSelectAll = new QAction(tr("&Select all"), this);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::SelectAll);
    menu2->addAction(a);
    tb->addWidget(find);

}
void MainWindow::setupTextActions()
{
    QMenu *menu = new QMenu(tr("F&ormat"), this);
    menuBar()->addMenu(menu);

    actionWrap = new QAction(tr("&Wrap words"), this);
    actionWrap->setPriority(QAction::LowPriority);
    actionWrap->setCheckable(true);
    menu->addAction(actionWrap);

    actionSettings= new QAction(tr("&Text format"), this);
    actionSettings->setPriority(QAction::LowPriority);
    menu->addAction(actionSettings);
}
void MainWindow::setupViewActions()
{
    QMenu *menu = new QMenu(tr("&View"), this);
    menuBar()->addMenu(menu);
    QAction *a;

    a = actionBackgroundColor = new QAction(tr("&Background Color"), this);
    menu->addAction(a);
    connect(a,SIGNAL(triggered()),this,SLOT(ChangeBgColor()));

    a = actionCurLineColor = new QAction(tr("&Current Line Color"), this);
    menu->addAction(a);
    connect(a,SIGNAL(triggered()),this,SLOT(ChangeClColor()));

    a = actionNumLines = new QAction(tr("&Line Numbering"), this);
    a->setCheckable(true);
    a->setChecked(true);
    connect(a,SIGNAL(triggered(bool)),textEdit,SLOT(setLineNumbers(bool)));
    menu->addAction(a);

    a = actionToolBar = new QAction(tr("&Toolbar"), this);
    a->setCheckable(true);
    a->setChecked(true);
    connect(a,SIGNAL(triggered(bool)),this->tb,SLOT(setVisible(bool)));
    menu->addAction(a);

    a = actionStatusBar = new QAction(tr("&Statusbar"), this);
    a->setCheckable(true);
    a->setChecked(true);
    connect(a,SIGNAL(triggered(bool)),this->statusBar(),SLOT(setVisible(bool)));
    menu->addAction(a);

    a = actionHighLight = new QAction(tr("&Syntax Highlight"), this);
    a->setCheckable(true);
    a->setChecked(true);
    connect(a,SIGNAL(triggered(bool)),highlighter,SLOT(setHighligth(bool)));
    menu->addAction(a);

    QMenu *m;
    m = actionChooseHighLight = new QMenu(tr("&Choose syntax version"), this);
    menu->addMenu(m);

    QGroupBox *b = new QGroupBox(this);
    QVBoxLayout *vbox = new QVBoxLayout;
    QWidgetAction *tmp = new QWidgetAction(m);
    tmp->setDefaultWidget(b);
    m->addAction(tmp);
    C89 = new QRadioButton(tr("C89"),this);
    C03 = new QRadioButton(tr("C++03"),this);
    C11 = new QRadioButton(tr("C++11"),this);
    C17 = new QRadioButton(tr("C++17"),this);
    C20 = new QRadioButton(tr("C++20"),this);
    vbox->addWidget(C89);
    vbox->addWidget(C03);
    vbox->addWidget(C11);
    vbox->addWidget(C17);
    vbox->addWidget(C20);
    connect(C89,SIGNAL(toggled(bool)),highlighter,SLOT(setC89(bool)));
    connect(C03,SIGNAL(toggled(bool)),highlighter,SLOT(setC03(bool)));
    connect(C11,SIGNAL(toggled(bool)),highlighter,SLOT(setC11(bool)));
    connect(C17,SIGNAL(toggled(bool)),highlighter,SLOT(setC17(bool)));
    connect(C20,SIGNAL(toggled(bool)),highlighter,SLOT(setC20(bool)));
    C89->setChecked(true);
    b->setLayout(vbox);
}
bool MainWindow::load(const QString &f)
{
    if (!QFile::exists(f))
        return false;
    QFile file(f);
    if (!file.open(QFile::ReadOnly))
        return false;

    QByteArray data = file.readAll();
    QTextCodec *codec = Qt::codecForHtml(data);
    QString str = codec->toUnicode(data);
    str = QString::fromLocal8Bit(data);
    textEdit->setPlainText(str);

    setCurrentFileName(f);
    return true;
}
bool MainWindow::maybeSave()
{
    if (!textEdit->document()->isModified())
        return true;
    if (fileName.startsWith(QLatin1String(":/")))
        return true;
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Application"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard
                               | QMessageBox::Cancel);
    if (ret == QMessageBox::Save)
        return fileSave();
    else if (ret == QMessageBox::Cancel)
        return false;
    return true;
}
void MainWindow::setCurrentFileName(const QString &fileName)
{
    this->fileName = fileName;
    textEdit->document()->setModified(false);
    QString shownName;
    if (fileName.isEmpty())
        shownName = "untitled.txt";
    else
        shownName = QFileInfo(fileName).fileName();
    if(fileName.size()<32)
        setWindowTitle(tr("[*]%1 - %2").arg(shownName).arg(tr("Text Edit")));
    else{
        shownName.remove(32,shownName.size()-32);
        shownName += "...";
        setWindowTitle(tr("[*]%1 - %2").arg(shownName).arg(tr("Text Edit")));
    }
    setWindowModified(false);
}
void MainWindow::fileNew()
{
    if (maybeSave()) {
        textEdit->clear();
        setCurrentFileName(QString());
    }
}
void MainWindow::fileOpen()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                              QString(), tr("HTML-Files (*.htm *.html);;ODT-Files (*.odt);;TXT-Files (*.txt);;All Files (*)"));
    if (!fn.isEmpty())
        load(fn);
}

bool MainWindow::fileSave()
{
    if (fileName.isEmpty())
        return fileSaveAs();

    QTextDocumentWriter writer(fileName);
    bool success = writer.write(textEdit->document());
    if (success)
        textEdit->document()->setModified(false);
    return success;
}
bool MainWindow::fileSaveAs()
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                              QString(), tr("ODF files (*.odt);;HTML-Files (*.htm *.html);;TXT files (*.txt);;All Files (*)"));
    if (fn.isEmpty())
        return false;
    if (! (fn.endsWith(".odt", Qt::CaseInsensitive) || fn.endsWith(".htm", Qt::CaseInsensitive) || fn.endsWith(".html", Qt::CaseInsensitive) || fn.endsWith(".txt", Qt::CaseInsensitive)) )
        fn += ".txt";
    setCurrentFileName(fn);
    return fileSave();
}

void MainWindow::clipboardDataChanged()
{
#ifndef QT_NO_CLIPBOARD
    if (const QMimeData *md = QApplication::clipboard()->mimeData())
        actionPaste->setEnabled(md->hasText());
#endif
}
void MainWindow::about()
{
    QDialog *diag = new QDialog(this);
    QImage im = QImage("Me.jpg").scaled(200,300);
    QLabel* lab = new QLabel(diag);
    diag->setModal(true);
    diag->setWindowTitle("About");
    QLabel* lab2 = new QLabel(diag);
    QPushButton* cl = new QPushButton(tr("&Close"),diag);
    lab->setPixmap(QPixmap::fromImage(im));
    connect(cl,SIGNAL(clicked()),diag,SLOT(close()));
    cl->move(250,120);
    lab2->setText("Author: Teregulov Timur\nBuild date: 26.11.2020\nBuild Qt version: 4.8.7\nLaunch Qt version: 4.8.7");
    lab2->move(220,50);
    diag->resize(370,330);
    diag->show();
}
void MainWindow::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    textEdit->mergeCurrentCharFormat(format);
}

void MainWindow::settings(){
    QTextCharFormat fmt;
    fmt.setFont(QFontDialog::getFont(0,textEdit->font()));
    mergeFormatOnWordOrSelection(fmt);
}
void MainWindow::setupStatusBar(){
    QSplitter *sp = new QSplitter(this);
    connect(textEdit,SIGNAL(cursorPositionChanged()),this,SLOT(setText()));
    connect(textEdit->document(),SIGNAL(contentsChanged()),this,SLOT(setLWCS()));
    statusBar()->addWidget(sp);
    sp->addWidget(rowcur);
    sp->addWidget(colcur);
    sp = new QSplitter(this);
    statusBar()->addWidget(sp);
    sp->addWidget(time);
    sp = new QSplitter(this);
    statusBar()->addWidget(sp);
    sp->addWidget(lines);
    sp->addWidget(words);
    sp->addWidget(chars);
    sp->addWidget(size);
}
void MainWindow::setText(){
    QTextCursor cursor = textEdit->textCursor();
    int pos = cursor.positionInBlock();
    colcur->setText("Column - " + QString::number(pos));
    rowcur->setText("Row - " + QString::number(cursor.blockNumber()+1));
}
void MainWindow::setTime(){
    QDateTime *dt = new QDateTime(QDateTime::currentDateTime());
    time->setText("Last changing: " + dt->toString());
}
void MainWindow::setLWCS(){
    lines->setText("Lines - " + QString::number(textEdit->blockCount()));
    int wordCount = textEdit->toPlainText().split(QRegularExpression("(\\s|\\n|\\r)+"), Qt::SkipEmptyParts).count();
    words->setText("Words - " + QString::number(wordCount));
    QString tmp = textEdit->toPlainText();
    tmp.replace("\n","");
    chars->setText("Characters - " + QString::number(tmp.length()));
    QFile f(fileName);
    double s = f.size();
    size->setText("Size - " + QString::number(s/1024));
}
void MainWindow::ChangeBgColor(){
    QColorDialog *cd = new QColorDialog(this);
    cd->setOption(QColorDialog::NoButtons);
    connect(cd,SIGNAL(currentColorChanged(QColor)),this,SLOT(setBgColor(QColor)));
    cd->show();
}
void MainWindow::setBgColor(QColor col){
    QPalette pal = textEdit->palette();
    pal.setColor(QPalette::Base,col);
    bgColor = col;
    textEdit->setPalette(pal);
}
void MainWindow::ChangeClColor(){
    QColorDialog *cd = new QColorDialog(this);
    cd->setOption(QColorDialog::NoButtons);
    connect(cd,SIGNAL(currentColorChanged(QColor)),textEdit,SLOT(setLineColor(QColor)));
    connect(cd,SIGNAL(currentColorChanged(QColor)), textEdit, SLOT(highlightCurrentLine()));
    lineColor = textEdit->getLineColor();
    cd->show();
}
void MainWindow::save_set(){
    QSettings settings("settings.ini",QSettings::IniFormat);
    settings.beginGroup("settings");
    settings.setValue("Line_Color",lineColor);
    settings.setValue("Background_Color",bgColor);
    settings.setValue("Font",textEdit->currentCharFormat());
    settings.endGroup();
}
