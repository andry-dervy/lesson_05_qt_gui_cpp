#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QToolBar"
#include "QFileDialog"
#include "QTextStream"
#include "QDockWidget"

void MainWindow::setMenu()
{
    auto menuFile = new QMenu(this);
    menuFile->setObjectName("menuFile");
    ui->menubar->addMenu(menuFile);
    auto actNew = new QAction(this);
    actNew->setObjectName("actNew");
    menuFile->addAction(actNew);
    connect(actNew, &QAction::triggered, this, &MainWindow::newFile);
    auto actOpen = new QAction(this);
    actOpen->setObjectName("actOpen");
    menuFile->addAction(actOpen);
    connect(actOpen, &QAction::triggered, this, &MainWindow::openFile);
    auto actOpenReadableOnly = new QAction(this);
    actOpenReadableOnly->setObjectName("actOpenReadableOnly");
    menuFile->addAction(actOpenReadableOnly);
    connect(actOpenReadableOnly, &QAction::triggered, this, &MainWindow::openFileReadableOnly);
    auto actSave = new QAction(this);
    actSave->setObjectName("actSave");
    menuFile->addAction(actSave);
    connect(actSave, &QAction::triggered, this, &MainWindow::saveFile);

    auto menuView = new QMenu(this);
    menuView->setObjectName("menuView");
    ui->menubar->addMenu(menuView);
    auto actViewTreeDirs = new QAction(this);
    actViewTreeDirs->setObjectName("actViewTreeDirs");
    menuView->addAction(actViewTreeDirs);
    connect(actViewTreeDirs, &QAction::triggered, this, &MainWindow::viewTreeDirs);

    auto menuSettings = new QMenu(this);
    menuSettings->setObjectName("menuSettings");
    ui->menubar->addMenu(menuSettings);
    auto actAssignKeyboardShortcuts = new QAction(this);
    actAssignKeyboardShortcuts->setObjectName("actAssignKeyboardShortcuts");
    menuSettings->addAction(actAssignKeyboardShortcuts);
    connect(actAssignKeyboardShortcuts, &QAction::triggered, this, &MainWindow::assignKeyboardShortcuts);

    auto menuTheme = new QMenu(this);
    menuTheme->setObjectName("menuTheme");
    menuSettings->addMenu(menuTheme);
    auto actThemeMailSy = new QAction(this);
    actThemeMailSy->setObjectName("actThemeMailSy");
    menuTheme->addAction(actThemeMailSy);
    connect(actThemeMailSy, &QAction::triggered, this, &MainWindow::setThemeMailSy);
    auto actThemeIntegrid = new QAction(this);
    actThemeIntegrid->setObjectName("actThemeIntegrid");
    menuTheme->addAction(actThemeIntegrid);
    connect(actThemeIntegrid, &QAction::triggered, this, &MainWindow::setThemeIntegrid);

    auto menuLanguage = new QMenu(this);
    menuLanguage->setObjectName("menuLanguage");
    ui->menubar->addMenu(menuLanguage);
    auto actRussian = new QAction(this);
    actRussian->setObjectName("actRussian");
    menuLanguage->addAction(actRussian);
    connect(actRussian, &QAction::triggered, this, &MainWindow::setRussianLanguage);
    auto actEnglish = new QAction(this);
    actEnglish->setObjectName("actEnglish");
    menuLanguage->addAction(actEnglish);
    connect(actEnglish, &QAction::triggered, this, &MainWindow::setEnglishLanguage);

    auto menuHelp = new QMenu(this);
    menuHelp->setObjectName("menuHelp");
    ui->menubar->addMenu(menuHelp);
    auto actCallingHelp = new QAction(this);
    actCallingHelp->setObjectName("actCallingHelp");
    menuHelp->addAction(actCallingHelp);
    connect(actCallingHelp, &QAction::triggered, this, &MainWindow::callingHelp);

    connect(menuHelp, &QMenu::triggered, this, &MainWindow::callingHelp);

    setThemeIntegrid();
}

void MainWindow::setToolBar()
{
    auto toptb = new QToolBar(this);
    auto toolBarNewFile = new QAction(this);
    toolBarNewFile->setObjectName("toolBarNewFile");
    toolBarNewFile->setIcon(QPixmap(":/icons/new.png"));
    connect(toolBarNewFile, &QAction::triggered, this, &MainWindow::newFile);
    toptb->addAction(toolBarNewFile);
    auto toolBarOpenFile = new QAction(this);
    toolBarOpenFile->setObjectName("toolBarOpenFile");
    toolBarOpenFile->setIcon(QPixmap(":/icons/open.png"));
    connect(toolBarOpenFile, &QAction::triggered, this, &MainWindow::openFile);
    toptb->addAction(toolBarOpenFile);
    auto toolBarSaveFile = new QAction(this);
    toolBarSaveFile->setObjectName("toolBarSaveFile");
    toolBarSaveFile->setIcon(QPixmap(":/icons/save.png"));
    connect(toolBarSaveFile, &QAction::triggered, this, &MainWindow::saveFile);
    toptb->addAction(toolBarSaveFile);
    toptb->addSeparator();

    changeReadableWritable = new QAction(this);
    changeReadableWritable->setObjectName("changeReadableWritable");
    setRW(WR::WRITABLE);
    connect(changeReadableWritable, &QAction::triggered, this, &MainWindow::changeReadableWritable_triggered);
    toptb->addAction(changeReadableWritable);

    addToolBar(Qt::TopToolBarArea,toptb);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , pathDir(QDir::currentPath())
    , wdtHelp(nullptr)
    , keyPressEvent(new KeyPressEvent(this))
    , wdgtSettings(nullptr)
    , wdgtTreeDirs(nullptr)
{
    ui->setupUi(this);

    textEdit = new QTextEdit(this);
    textEdit->installEventFilter(keyPressEvent);
    setCentralWidget(textEdit);

    statusLabel = new QLabel(this);
    statusBar()->addWidget(statusLabel);

    installEventFilter(keyPressEvent);
    connect(keyPressEvent, &KeyPressEvent::newFile, this, &MainWindow::newFile);
    connect(keyPressEvent, &KeyPressEvent::openFile, this, &MainWindow::openFile);
    connect(keyPressEvent, &KeyPressEvent::saveFile, this, &MainWindow::saveFile);
    connect(keyPressEvent, &KeyPressEvent::quit, this, &MainWindow::quit);

    setMenu();
    setToolBar();

    setRussianLanguage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newFile()
{
    textEdit->clear();
    statusLabel->clear();
    setRW(WR::WRITABLE);
}

void MainWindow::openFilePath(QString& fileName)
{
    if(fileName.isEmpty()) return;

    int index = fileName.indexOf(".txt");
    if(index == -1 || fileName.length() - 4 != index) return;

    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::ExistingOnly)) return;

    QTextStream stream(&file);
    textEdit->setPlainText(stream.readAll());
    file.close();
    statusLabel->setText(fileName);
}

void MainWindow::openFile_(WR wr)
{
    QFileDialog fileDialog;

    QString fileName = fileDialog.getOpenFileName(this,
        tr("Открыть файл"), pathDir, tr("Текстовые файлы (*.txt)"));

    openFilePath(fileName);

    pathDir = fileDialog.directory().absolutePath();
    setRW(wr);
}

void MainWindow::openFile()
{
    openFile_(WR::WRITABLE);
}

void MainWindow::openFileReadableOnly()
{
    openFile_(WR::READABLE_ONLY);
}

void MainWindow::saveFile()
{
    QString fileName;
    if(statusLabel->text().isEmpty())
    {
        QFileDialog fileDialog;

        fileName = fileDialog.getSaveFileName(this,
            tr("Сохранить файл"), pathDir, tr("Текстовые файлы (*.txt)"));

        if(fileName.isEmpty()) return;

        int index = fileName.indexOf(".txt");
        if(index == -1 || fileName.length() - 4 != index)
        {
            fileName += ".txt";
        }

        pathDir = fileDialog.directory().absolutePath();
        statusLabel->setText(fileName);
    }
    else
    {
        fileName = statusLabel->text();
    }

    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Truncate)) return;

    QTextStream stream(&file);
    stream << textEdit->toPlainText();
    file.close();
}

void MainWindow::callingHelp()
{
    if(wdtHelp != nullptr)
    {
        wdtHelp->show();
        return;
    }

    QFile file(":/help/help.txt");
    if(!file.open(QIODevice::OpenModeFlag::ReadOnly)) return;

    wdtHelp = new QDialog(this);
    QTextStream textStream(&file);
    auto textHelp = new QPlainTextEdit(textStream.readAll(),this);
    textHelp->setReadOnly(true);
    auto vBoxLayout = new QVBoxLayout(wdtHelp);
    vBoxLayout->addWidget(textHelp);
    wdtHelp->setLayout(vBoxLayout);
    wdtHelp->setMinimumSize(QSize(200,200));
    retranslate();
    wdtHelp->show();
}

void MainWindow::setRW(WR wr)
{
    ReadableWritable = wr;
    switch (wr) {
    case WR::READABLE:
    case WR::READABLE_ONLY:
        changeReadableWritable->setIcon(QPixmap(":/icons/readable.png"));
        changeReadableWritable->setIconText(tr("Нередактируемый текст"));
        textEdit->setReadOnly(true);
        break;
    case WR::WRITABLE:
        changeReadableWritable->setIcon(QPixmap(":/icons/writable.png"));
        changeReadableWritable->setIconText(tr("Редактируемый текст"));
        textEdit->setReadOnly(false);
        break;
    }
}

void MainWindow::changeReadableWritable_triggered()
{
    switch (ReadableWritable) {
    case WR::READABLE:
        setRW(WR::WRITABLE);
        break;
    case WR::READABLE_ONLY:
        break;
    case WR::WRITABLE:
        setRW(WR::READABLE);
        break;
    }
}

void MainWindow::quit()
{
    qApp->quit();
}

void MainWindow::assignKeyboardShortcuts()
{
    if(wdgtSettings != nullptr)
    {
        wdgtSettings->show();
        return;
    }

    wdgtSettings = new WidgetSettings(this);
    connect(wdgtSettings, &WidgetSettings::setHotKey, keyPressEvent, &KeyPressEvent::setHotKey);
    wdgtSettings->show();
}

void MainWindow::retranslate()
{
    setWindowTitle(tr("Простой текстовый редактор"));

    auto menuFile = findChild<QMenu*>("menuFile");
    if(menuFile != nullptr) menuFile->setTitle(tr("&Файл"));
    auto actNew = findChild<QAction*>("actNew");
    if(actNew != nullptr) actNew->setText(tr("Новый файл"));
    auto actOpen = findChild<QAction*>("actOpen");
    if(actOpen != nullptr) actOpen->setText(tr("Открыть файл"));
    auto actOpenReadableOnly = findChild<QAction*>("actOpenReadableOnly");
    if(actOpenReadableOnly != nullptr) actOpenReadableOnly->setText(tr("Открыть файл только для чтения"));
    auto actSave = findChild<QAction*>("actSave");
    if(actSave != nullptr) actSave->setText(tr("Сохранить файл"));

    auto menuView = findChild<QMenu*>("menuView");
    if(menuView != nullptr) menuView->setTitle(tr("&Вид"));
    auto actViewTreeDirs = findChild<QAction*>("actViewTreeDirs");
    if(actViewTreeDirs != nullptr) actViewTreeDirs->setText(tr("Дерево каталогов"));

    auto menuSettings = findChild<QMenu*>("menuSettings");
    if(menuSettings != nullptr) menuSettings->setTitle(tr("&Настройки"));
    auto actAssignKeyboardShortcuts = findChild<QAction*>("actAssignKeyboardShortcuts");
    if(actAssignKeyboardShortcuts != nullptr) actAssignKeyboardShortcuts->setText(tr("Установка горячик клавиш"));
    auto menuTheme = findChild<QMenu*>("menuTheme");
    if(menuTheme != nullptr) menuTheme->setTitle(tr("&Выбор темы"));
    auto actThemeMailSy = findChild<QAction*>("actThemeMailSy");
    if(actThemeMailSy != nullptr) actThemeMailSy->setText(tr("Тема MailSy"));
    auto actThemeIntegrid = findChild<QAction*>("actThemeIntegrid");
    if(actThemeIntegrid != nullptr) actThemeIntegrid->setText(tr("Тема Integrid"));

    auto menuLanguage = findChild<QMenu*>("menuLanguage");
    if(menuLanguage != nullptr) menuLanguage->setTitle(tr("&Язык интерфейса"));
    auto actRussian = findChild<QAction*>("actRussian");
    if(actRussian != nullptr) actRussian->setText(tr("Русский"));
    auto actEnglish = findChild<QAction*>("actEnglish");
    if(actEnglish != nullptr) actEnglish->setText(tr("Английский"));

    auto menuHelp = findChild<QMenu*>("menuHelp");
    if(menuHelp != nullptr) menuHelp->setTitle(tr("&Справка"));
    auto actCallingHelp = findChild<QAction*>("actCallingHelp");
    if(actCallingHelp != nullptr) actCallingHelp->setText(tr("Вызов справки"));

    auto toolBarNewFile = findChild<QAction*>("toolBarNewFile");
    if(toolBarNewFile != nullptr) toolBarNewFile->setIconText(tr("Новый файл"));
    auto toolBarOpenFile = findChild<QAction*>("toolBarOpenFile");
    if(toolBarOpenFile != nullptr) toolBarOpenFile->setIconText(tr("Открыть файл"));
    auto toolBarSaveFile = findChild<QAction*>("toolBarSaveFile");
    if(toolBarSaveFile != nullptr) toolBarSaveFile->setIconText(tr("Сохранить файл"));

    switch (ReadableWritable) {
    case WR::READABLE:
    case WR::READABLE_ONLY:
        changeReadableWritable->setIconText(tr("Нередактируемый текст"));
        break;
    case WR::WRITABLE:
        changeReadableWritable->setIconText(tr("Редактируемый текст"));
        break;
    }

    if(wdtHelp != nullptr)
        wdtHelp->setWindowTitle(tr("Справка"));

    if(wdgtSettings != nullptr)
        wdgtSettings->retranslate();

    if(wdgtTreeDirs != nullptr)
        wdgtTreeDirs->retranslate();
}

void MainWindow::acceptLanguage(std::shared_ptr<LanguageVisitor>& lv)
{
    lv->retranslate(this);
}

void MainWindow::setRussianLanguage()
{
    std::shared_ptr<LanguageVisitor> languageVisitor =
            std::make_shared<LanguageRussianVisitor>();
    acceptLanguage(languageVisitor);

    auto actRussian = findChild<QAction*>("actRussian");
    if(actRussian != nullptr) actRussian->setIcon(QPixmap(":/icons/enable.png"));
    auto actEnglish = findChild<QAction*>("actEnglish");
    if(actEnglish != nullptr) actEnglish->setIcon(QPixmap());
}

void MainWindow::setEnglishLanguage()
{
    std::shared_ptr<LanguageVisitor> languageVisitor =
            std::make_shared<LanguageEnglishVisitor>();
    acceptLanguage(languageVisitor);

    auto actRussian = findChild<QAction*>("actRussian");
    if(actRussian != nullptr) actRussian->setIcon(QPixmap());
    auto actEnglish = findChild<QAction*>("actEnglish");
    if(actEnglish != nullptr) actEnglish->setIcon(QPixmap(":/icons/enable.png"));
}

void MainWindow::setThemeMailSy()
{
    QFile fileQss(":/styles/MailSy.qss");
    if(!fileQss.open(QFile::ReadOnly)) return;

    QTextStream stream(&fileQss);
    setStyleSheet(stream.readAll());
    fileQss.close();

    auto actThemeMailSy = findChild<QAction*>("actThemeMailSy");
    if(actThemeMailSy != nullptr) actThemeMailSy->setIcon(QPixmap(":/icons/enable.png"));
    auto actThemeIntegrid = findChild<QAction*>("actThemeIntegrid");
    if(actThemeIntegrid != nullptr) actThemeIntegrid->setIcon(QPixmap());
}

void MainWindow::setThemeIntegrid()
{
    QFile fileQss(":/styles/Integrid.qss");
    if(!fileQss.open(QFile::ReadOnly)) return;

    QTextStream stream(&fileQss);
    setStyleSheet(stream.readAll());
    fileQss.close();

    auto actThemeMailSy = findChild<QAction*>("actThemeMailSy");
    if(actThemeMailSy != nullptr) actThemeMailSy->setIcon(QPixmap());
    auto actThemeIntegrid = findChild<QAction*>("actThemeIntegrid");
    if(actThemeIntegrid != nullptr) actThemeIntegrid->setIcon(QPixmap(":/icons/enable.png"));
}

void MainWindow::viewTreeDirs()
{
    if(wdgtTreeDirs == nullptr)
    {
        wdgtTreeDirs = new WidgetTreeDirs(this);
        connect(wdgtTreeDirs, &WidgetTreeDirs::openFile, this, &MainWindow::openFilePath);
    }

    auto dockWidget = new QDockWidget(this);
    dockWidget->setWidget(wdgtTreeDirs);
    addDockWidget(Qt::LeftDockWidgetArea, dockWidget);
}

LanguageRussianVisitor::LanguageRussianVisitor()
{
    translator.load(":/language/QtLanguage_ru.qm");
    qApp->installTranslator(&translator);
}

LanguageEnglishVisitor::LanguageEnglishVisitor()
{
    translator.load(":/language/QtLanguage_en.qm");
    qApp->installTranslator(&translator);
}

void LanguageVisitor::retranslate(MainWindow* mainWindow)
{
    if(mainWindow == nullptr) return;
    mainWindow->retranslate();
}
