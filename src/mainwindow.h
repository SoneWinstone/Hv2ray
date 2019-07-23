#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define confDir "conf/"
#define confDatabase "conf/conf.db"
#include <QMainWindow>
#include "confedit.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include <QScrollBar>

#include "subscribeeditor.h"
#include "config.h"

class v2Instance;
namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    v2Instance *v2Inst;
    QSystemTrayIcon *hTray;
    QMenu *trayMenu = new QMenu(this);
    QMenu *popMenu = new QMenu(this);
    QScrollBar *bar;
    ~MainWindow();

private:
    void closeEvent(QCloseEvent *);
    void createTrayAction();
    void init();
    
public slots:
    void on_restartButton_clicked();

private slots:
    void onTimeout();
    void changeNode();
    void on_actionEdit_triggered();
    void on_actionSubscribe_triggered();
    void on_actionExisting_config_triggered();
    void updateConfTable();
    void delConf();
    void showMenu(QPoint pos);
    void updateLog();
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void select_triggered();
    void on_clbutton_clicked();
    void on_rtButton_clicked();
    void geneConf(int idIntable);
    void on_actionVmess_triggered();
    void on_activatedTray(QSystemTrayIcon::ActivationReason reason);
    void toggleMainWindowVisibility();
    void quit();
    void on_actionExit_triggered();
    void renameRow();
    void scrollToBottom();
    void on_actionPreferences_triggered();

private:
    Ui::MainWindow *ui;
    subscribeeditor *subscriber;
    int             iConfRows;
    int             iCurRow;
    QStringList     listStrIPs;
    QString         strCmd = "curl -m 3 --retry 0 --socks5 127.0.0.1:" + QString::number(SOCKS_PORT) + " ";
    int             iListIndex = 0;
};

#endif // MAINWINDOW_H
