#ifndef MODELPROC_H
#define MODELPROC_H
#include "db/dbtablemodel.h"

class ModelProc : public DbTableModel
{
    Q_OBJECT
public:
    ModelProc(QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role) const;
public slots:
    void setDbeg(QDate d);
    void setDend(QDate d);
    bool select();
private:
    QDate dBeg, dEnd;
    DbRelation *relProgs;
};

#endif // MODELPROC_H
