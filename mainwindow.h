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
#include <widgettreedirs.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class LanguageVisitor;

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

    void retranslate();

private:
    Ui::MainWindow *ui;

    QString pathDir;
    QDialog* wdtHelp;
    KeyPressEvent* keyPressEvent;
    WidgetSettings* wdgtSettings;
    WidgetTreeDirs* wdgtTreeDirs;

    QTextEdit* textEdit;
    QLabel* statusLabel;

    WR ReadableWritable;
    QAction* changeReadableWritable;

    void acceptLanguage(std::shared_ptr<LanguageVisitor>& lv);

private:
    void setMenu();
    void setToolBar();

    void setRW(WR wr);
    void openFile_(WR wr);

private slots:
    void setRussianLanguage();
    void setEnglishLanguage();

    void setThemeMailSy();
    void setThemeIntegrid();

    void viewTreeDirs();
    void newFile();
    void openFilePath(QString& path);
    void openFile();
    void openFileReadableOnly();

    void saveFile();

    void callingHelp();
    void changeReadableWritable_triggered();

    void quit();
    void assignKeyboardShortcuts();
};

class LanguageVisitor
{
public:
    virtual ~LanguageVisitor(){}
    void retranslate(MainWindow* mainWindow);
protected:
    QTranslator translator;
};

class LanguageRussianVisitor: public LanguageVisitor
{
public:
    LanguageRussianVisitor();
};

class LanguageEnglishVisitor: public LanguageVisitor
{
public:
    LanguageEnglishVisitor();
};

#endif // MAINWINDOW_H
