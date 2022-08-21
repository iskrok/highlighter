#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "highlighter.h"
#include "codeeditor.h"
#include <QMainWindow>
#include <QMap>
#include <QPointer>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QRadioButton>
QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QFontComboBox)
QT_FORWARD_DECLARE_CLASS(QTextEdit)
QT_FORWARD_DECLARE_CLASS(QTextCharFormat)
QT_FORWARD_DECLARE_CLASS(QMenu)

class QTextEdit;

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

protected:
    virtual void closeEvent(QCloseEvent *e);


private:
    void setupFileAndEditActions();
    void setupTextActions();
    void setupViewActions();
    bool load(const QString &f);
    bool maybeSave();
    void setCurrentFileName(const QString &fileName);

private slots:
    void fileNew();
    void fileOpen();
    bool fileSave();
    bool fileSaveAs();
    void settings();
    void clipboardDataChanged();
    void about();
    void setText();
    void setTime();
    void setLWCS();
    void ChangeBgColor();
    void setBgColor(QColor);
    void ChangeClColor();
    void save_set();
private:
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void setupStatusBar();
    QAction *actionSave,
        *actionUndo,
        *actionRedo,
        *actionCut,
        *actionCopy,
        *actionPaste,
        *actionFind,
        *actionFindAndReplace,
        *actionSelectAll,
        *actionWrap,
        *actionSettings,
        *actionBackgroundColor,
        *actionCurLineColor,
        *actionNumLines,
        *actionToolBar,
        *actionStatusBar,
        *actionHighLight,
        *actionHighLighthStyle;

    QToolBar *tb;
    QString fileName;
    CodeEditor *textEdit;
    Highlighter *highlighter;
    QLabel* colcur;
    QLabel* rowcur;
    QLabel *time;
    QLabel *lines;
    QLabel *words;
    QLabel *chars;
    QLabel *size;
    QColor bgColor;
    QColor lineColor;
    QFont currentFont;
    QMenu *actionChooseHighLight;
    QRadioButton *C89,*C03,*C11,*C17,*C20;
};

#endif
