#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAbout_triggered();

    void on_actionSearch_triggered();

    void on_actionReplace_triggered();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    bool on_actionSaveAs_triggered();

    void on_TextEdit_textChanged();

    void on_actionUndo_triggered();

    void on_actionCut_triggered();

    void on_actionRedo_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_TextEdit_copyAvailable(bool b);

    void on_TextEdit_redoAvailable(bool b);

    void on_TextEdit_undoAvailable(bool b);

    void on_actionFontColor_triggered();

    void on_actionEditBackgroundColor_triggered();

    void on_actionFontBackgroundcolor_triggered();

    void on_action_WordWrap_triggered();

    void on_actionFontStyle_triggered();

    void on_actionToolBar_triggered();

    void on_actionStatusBar_triggered();

    void on_actionExit_triggered();

    void on_actionAllSelect_triggered();

    void on_TextEdit_cursorPositionChanged();

    void on_actionLineNum_triggered();

private:
    Ui::MainWindow *ui;
    QLabel statusCursorLabel;
    QLabel statusLabel;

    QString FilePath;
    bool textChanged;

    bool userEditConfirmed();
};
#endif // MAINWINDOW_H
