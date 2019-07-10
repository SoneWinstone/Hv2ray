#ifndef CONFEDIT_H
#define CONFEDIT_H

#include <QDialog>

namespace Ui
{
    class ConfEdit;
}

class vConfig
{
public:
    QString host;
    QString port;
    QString alias;
    QString uuid;
    QString alterid;
    QString security;
    int isCustom;
    int subId;
    vConfig *query(int id);
    int save();
    void getConfigFromDialog(Ui::ConfEdit *ui);
private:

};

class ConfEdit : public QDialog
{
    Q_OBJECT

public:
    explicit ConfEdit(QWidget *parent = nullptr);
    ~ConfEdit();
signals:
    void updateConfTable();
private slots:
    void on_ConfEdit_accepted();

private:
    Ui::ConfEdit *ui;

};


#endif // CONFEDIT_H
