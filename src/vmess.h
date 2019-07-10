#ifndef VMESS_H
#define VMESS_H

#include <QDialog>

namespace Ui
{
    class vmess;
}

class VmessConf {
public:
    QString add;
    QString host;
    QString id;
    QString net;
    QString path;
    QString port;
    QString ps;
    QString tls;
    int v;
    int aid;
    QString type;
};

class vmess : public QDialog
{
    Q_OBJECT

public:
    explicit vmess(QWidget *parent = nullptr);
    ~vmess();
    bool addVmess(const QString& vmess, const int subId);
    VmessConf paresVmess(const QString& vmess);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::vmess *ui;
};

#endif // VMESS_H
