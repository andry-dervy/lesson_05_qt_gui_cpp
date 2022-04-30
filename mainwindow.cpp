#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QToolBar"
#include "QFileDialog"
#include "QTextStream"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , pathDir(QDir::currentPath())
    , wdtHelp(nullptr)
    , keyPressEvent(new KeyPressEvent(this))
    , wdgtSettings(nullptr)
{
    ui->setupUi(this);

    QMenu* menuFile = new QMenu(this);
    menuFile->setObjectName("menuFile");
    ui->menubar->addMenu(menuFile);
    QAction* actNew = new QAction(this);
    actNew->setObjectName("actNew");
    menuFile->addAction(actNew);
    connect(actNew, &QAction::triggered, this, &MainWindow::newFile);
    QAction* actOpen = new QAction(this);
    actOpen->setObjectName("actOpen");
    menuFile->addAction(actOpen);
    connect(actOpen, &QAction::triggered, this, &MainWindow::openFile);
    QAction* actOpenReadableOnly = new QAction(this);
    actOpenReadableOnly->setObjectName("actOpenReadableOnly");
    menuFile->addAction(actOpenReadableOnly);
    connect(actOpenReadableOnly, &QAction::triggered, this, &MainWindow::openFileReadableOnly);
    QAction* actSave = new QAction(this);
    actSave->setObjectName("actSave");
    menuFile->addAction(actSave);
    connect(actSave, &QAction::triggered, this, &MainWindow::saveFile);

    QMenu* menuSettings = new QMenu(this);
    menuSettings->setObjectName("menuSettings");
    ui->menubar->addMenu(menuSettings);
    QAction* actAssignKeyboardShortcuts = new QAction(this);
    actAssignKeyboardShortcuts->setObjectName("actAssignKeyboardShortcuts");
    menuSettings->addAction(actAssignKeyboardShortcuts);
    connect(actAssignKeyboardShortcuts, &QAction::triggered, this, &MainWindow::assignKeyboardShortcuts);

    QMenu* menuLanguage = new QMenu(this);
    menuLanguage->setObjectName("menuLanguage");
    ui->menubar->addMenu(menuLanguage);
    QAction* actRussian = new QAction(this);
    actRussian->setObjectName("actRussian");
    menuLanguage->addAction(actRussian);
    connect(actRussian, &QAction::triggered, this, &MainWindow::setRussianLanguage);
    QAction* actEnglish = new QAction(this);
    actEnglish->setObjectName("actEnglish");
    menuLanguage->addAction(actEnglish);
    connect(actEnglish, &QAction::triggered, this, &MainWindow::setEnglishLanguage);

    QMenu* menuHelp = new QMenu(this);
    menuHelp->setObjectName("menuHelp");
    ui->menubar->addMenu(menuHelp);
    QAction* actCallingHelp = new QAction(this);
    actCallingHelp->setObjectName("actCallingHelp");
    menuHelp->addAction(actCallingHelp);
    connect(actCallingHelp, &QAction::triggered, this, &MainWindow::callingHelp);

    connect(menuHelp, &QMenu::triggered, this, &MainWindow::callingHelp);

    textEdit = new QTextEdit(this);
    textEdit->installEventFilter(keyPressEvent);
    setCentralWidget(textEdit);

    statusLabel = new QLabel(this);
    statusBar()->addWidget(statusLabel);

    QToolBar* toptb = new QToolBar(this);
    QAction* toolBarNewFile = new QAction(this);
    toolBarNewFile->setObjectName("toolBarNewFile");
    toolBarNewFile->setIcon(QPixmap(":/icons/new.png"));
    connect(toolBarNewFile, &QAction::triggered, this, &MainWindow::newFile);
    toptb->addAction(toolBarNewFile);
    QAction* toolBarOpenFile = new QAction(this);
    toolBarOpenFile->setObjectName("toolBarOpenFile");
    toolBarOpenFile->setIcon(QPixmap(":/icons/open.png"));
    connect(toolBarOpenFile, &QAction::triggered, this, &MainWindow::openFile);
    toptb->addAction(toolBarOpenFile);
    QAction* toolBarSaveFile = new QAction(this);
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

    translator.load(":/language/QtLanguage_ru.qm");
    qApp->installTranslator(&translator);
    retranslate();

    installEventFilter(keyPressEvent);
    connect(keyPressEvent, &KeyPressEvent::newFile, this, &MainWindow::newFile);
    connect(keyPressEvent, &KeyPressEvent::openFile, this, &MainWindow::openFile);
    connect(keyPressEvent, &KeyPressEvent::saveFile, this, &MainWindow::saveFile);
    connect(keyPressEvent, &KeyPressEvent::quit, this, &MainWindow::quit);
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

void MainWindow::openFile_(WR wr)
{
    QFileDialog fileDialog;

    QString fileName = fileDialog.getOpenFileName(this,
        tr("Открыть файл"), pathDir, tr("Текстовые файлы (*.txt)"));

    if(fileName.isEmpty()) return;

    int index = fileName.indexOf(".txt");
    if(index == -1 || fileName.length() - 4 != index) return;

    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::ExistingOnly)) return;

    QTextStream stream(&file);
    textEdit->setPlainText(stream.readAll());
    file.close();

    pathDir = fileDialog.directory().absolutePath();
    statusLabel->setText(fileName);
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
    QFileDialog fileDialog;

    QString fileName = fileDialog.getSaveFileName(this,
        tr("Сохранить файл"), pathDir, tr("Текстовые файлы (*.txt)"));

    if(fileName.isEmpty()) return;

    int index = fileName.indexOf(".txt");
    if(index == -1 || fileName.length() - 4 != index)
    {
        fileName += ".txt";
    }

    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Truncate)) return;

    QTextStream stream(&file);
    stream << textEdit->toPlainText();
    file.close();

    pathDir = fileDialog.directory().absolutePath();
    statusLabel->setText(fileName);
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
    QPlainTextEdit* textHelp = new QPlainTextEdit(textStream.readAll(),this);
    textHelp->setReadOnly(true);
    QVBoxLayout* vBoxLayout = new QVBoxLayout(wdtHelp);
    vBoxLayout->addWidget(textHelp);
    wdtHelp->setLayout(vBoxLayout);
    wdtHelp->setMinimumSize(QSize(200,200));
    wdtHelp->setWindowTitle(tr("Справка"));
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

    QMenu* menuFile = findChild<QMenu*>("menuFile");
    if(menuFile != nullptr) menuFile->setTitle(tr("&Файл"));
    QAction* actNew = findChild<QAction*>("actNew");
    if(actNew != nullptr) actNew->setText(tr("Новый файл"));
    QAction* actOpen = findChild<QAction*>("actOpen");
    if(actOpen != nullptr) actOpen->setText(tr("Открыть файл"));
    QAction* actOpenReadableOnly = findChild<QAction*>("actOpenReadableOnly");
    if(actOpenReadableOnly != nullptr) actOpenReadableOnly->setText(tr("Открыть файл только для чтения"));
    QAction* actSave = findChild<QAction*>("actSave");
    if(actSave != nullptr) actSave->setText(tr("Сохранить файл"));

    QMenu* menuSettings = findChild<QMenu*>("menuSettings");
    if(menuSettings != nullptr) menuSettings->setTitle(tr("&Настройки"));
    QAction* actAssignKeyboardShortcuts = findChild<QAction*>("actAssignKeyboardShortcuts");
    if(actAssignKeyboardShortcuts != nullptr) actAssignKeyboardShortcuts->setText(tr("Установка горячик клавиш"));

    QMenu* menuLanguage = findChild<QMenu*>("menuLanguage");
    if(menuLanguage != nullptr) menuLanguage->setTitle(tr("&Язык интерфейса"));
    QAction* actRussian = findChild<QAction*>("actRussian");
    if(actRussian != nullptr) actRussian->setText(tr("Русский"));
    QAction* actEnglish = findChild<QAction*>("actEnglish");
    if(actEnglish != nullptr) actEnglish->setText(tr("Английский"));

    QMenu* menuHelp = findChild<QMenu*>("menuHelp");
    if(menuHelp != nullptr) menuHelp->setTitle(tr("&Справка"));
    QAction* actCallingHelp = findChild<QAction*>("actCallingHelp");
    if(actCallingHelp != nullptr) actCallingHelp->setText(tr("Вызов справки"));

    QAction* toolBarNewFile = findChild<QAction*>("toolBarNewFile");
    if(toolBarNewFile != nullptr) toolBarNewFile->setIconText(tr("Новый файл"));
    QAction* toolBarOpenFile = findChild<QAction*>("toolBarOpenFile");
    if(toolBarOpenFile != nullptr) toolBarOpenFile->setIconText(tr("Открыть файл"));
    QAction* toolBarSaveFile = findChild<QAction*>("toolBarSaveFile");
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

    if(wdgtSettings != nullptr) wdgtSettings->retranslate();
}

void MainWindow::setRussianLanguage()
{
    translator.load(":/language/QtLanguage_ru.qm");
    qApp->installTranslator(&translator);
    retranslate();
}

void MainWindow::setEnglishLanguage()
{
    translator.load(":/language/QtLanguage_en.qm");
    qApp->installTranslator(&translator);
    retranslate();
}

