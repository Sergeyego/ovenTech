#ifndef DIALOGPROG_H
#define DIALOGPROG_H

#include <QDialog>
#include "db/dbtablemodel.h"
#include "db/dbmapper.h"

namespace Ui {
class DialogProg;
}

class DialogProg : public QDialog
{
    Q_OBJECT

public:
    explicit DialogProg(QWidget *parent = 0);
    ~DialogProg();

private slots:
    void updCont(int index);

private:
    Ui::DialogProg *ui;
    DbRelation *relProvol;
    DbRelation *relDiam;
    DbMapper *mapper;
    DbTableModel *modelProg;
    DbTableModel *modelCont;
};

#endif // DIALOGPROG_H
