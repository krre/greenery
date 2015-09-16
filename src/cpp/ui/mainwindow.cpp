#include "mainwindow.h"
#include "osgwidget.h"
#include "version.h"
#include "dialog/projectdialog.h"
#include "../settings.h"
#include <QtCore/QRect>
#include <QApplication>
#include <QDebug>
#include <QScreen>
#include <QGuiApplication>
#include <QMenuBar>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QFileDialog>
#include <QFileInfo>

extern QSharedPointer<Settings> settings;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Greenery");
    setMinimumSize(160, 160);
    setCentralWidget(&tabWidget);

    createMenus();
    setupTabWidget();
    loadSettings();
}

void MainWindow::addTab(QString &name, QString &path)
{
    int index = tabWidget.count();
    OsgWidget* osgWidget = new OsgWidget(this, Qt::Widget, osgViewer::CompositeViewer::SingleThreaded);
    osgWidget->setProjectPath(path);
    tabWidget.addTab(osgWidget, name);
    tabWidget.setCurrentIndex(index);
}

void MainWindow::onNewTab() {
    ProjectDialog* pd = new ProjectDialog(this);
    pd->exec();
}

void MainWindow::open() {
    QString filePath = QFileDialog::getOpenFileName(this,
                                tr("Open Sprout project"),
                                settings.data()->getRecentDirectory(),
                                tr("Sprout Files (*.sprout);;All Files (*)"));
    if (!filePath.isEmpty()) {
        for (int i = 0; i < tabWidget.count(); i++) {
            OsgWidget *osgWidget = static_cast<OsgWidget*>(tabWidget.widget(i));
            if (filePath ==osgWidget->projectPath()) {
                QMessageBox::warning(this, tr("Warning"), QDialog::tr("File already loaded"));
                return;
            }
        }
    }

    QFileInfo fileInfo(filePath);
    QString name = fileInfo.fileName().remove(".sprout");
    addTab(name, filePath);
}

void MainWindow::quitApp() {
    close();
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About Greenery"),
            QString("<h3>Greenery %1</h3>Build date: %2<br>Copyright (c) 2015, Vladimir Zarypov").arg(Version::full()).arg(__DATE__));
}

void MainWindow::aboutQt() {
    QMessageBox::aboutQt(this);
}

void MainWindow::onCloseTab(int index)
{
    tabWidget.removeTab(index);
}

void MainWindow::onCloseCurrentTab()
{
    onCloseTab(tabWidget.currentIndex());
}

void MainWindow::onCloseAllTabs()
{
    int count = tabWidget.count();
    for (int i = 0; i < count; i++) {
        onCloseTab(0);
    }
}

void MainWindow::onCloseOthersTabs()
{
    int count = tabWidget.count();
    QWidget *activeTab = tabWidget.currentWidget();
    for (int i = 0; i < count; i++) {
        if (tabWidget.widget(0) != activeTab)
            onCloseTab(0);
        else
            onCloseTab(1);
    }
}

void MainWindow::onActiveTabChanged(int index)
{
    Q_UNUSED(index)
}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(tr("New Project..."), this, SLOT(onNewTab()), Qt::CTRL + Qt::Key_N);
    fileMenu->addAction(tr("Open Project..."), this, SLOT(open()), Qt::CTRL + Qt::Key_O);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Close"), this, SLOT(onCloseCurrentTab()), Qt::CTRL + Qt::Key_W);
    fileMenu->addAction(tr("Close All"), this, SLOT(onCloseAllTabs()), Qt::CTRL + Qt::SHIFT + Qt::Key_W);
    fileMenu->addAction(tr("Close Others"), this, SLOT(onCloseOthersTabs()), Qt::CTRL + Qt::ALT + Qt::Key_W);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Quit"), this, SLOT(about()), Qt::CTRL + Qt::Key_Q);

    helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(tr("About..."), this, SLOT(about()));
    helpMenu->addAction(tr("About Qt..."), this, SLOT(aboutQt()));
}

void MainWindow::setupTabWidget()
{
    tabWidget.setTabsClosable(true);
    tabWidget.setMovable(true);
    connect(&tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(onCloseTab(int)));
    connect(&tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onActiveTabChanged(int)));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    saveSettings();
}

void MainWindow::saveSettings()
{
    QMap<QString, int> map;
    map["x"] = x();
    map["y"] = y();
    map["width"] = width();
    map["height"] = height();
    ::settings.data()->setGeometry(map);
}

void MainWindow::loadSettings()
{
    QMap<QString, int>map = ::settings.data()->getGeometry();
    if (map.isEmpty()) {
        // move window to center screen
        auto *screen = QGuiApplication::primaryScreen();
        auto screenSize = screen->size();
        int width = 800;
        int height = 600;
        int x = (screenSize.width() - width) / 2;
        int y = (screenSize.height() - height) / 2;
        setGeometry(x, y, width, height);
    } else {
        setGeometry(map["x"], map["y"], map["width"], map["height"]);
    }
}

