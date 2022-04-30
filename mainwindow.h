#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QTranslator>
#include <memory>
#include <keypressevent.h>
#include <widgetsettings.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum WR
    {
        READABLE,
        READABLE_ONLY,
        WRITABLE,
    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QString pathDir;
    QDialog* wdtHelp;
    KeyPressEvent* keyPressEvent;
    WidgetSettings* wdgtSettings;

    QTextEdit* textEdit;
    QLabel* statusLabel;

    QTranslator translator;

    WR ReadableWritable;
    QAction* changeReadableWritable;


private:
    void setRW(WR wr);
    void openFile_(WR wr);
    void retranslate();

private slots:
    void setRussianLanguage();
    void setEnglishLanguage();

    void newFile();
    void openFile();
    void openFileReadableOnly();

    void saveFile();

    void callingHelp();
    void changeReadableWritable_triggered();

    void quit();
    void assignKeyboardShortcuts();
};
#endif // MAINWINDOW_H
