#ifndef MODELALARM_H
#define MODELALARM_H
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDateTime>

class ModelAlarm : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelAlarm(QObject *parent=nullptr);
    void refresh(const QDateTime &beg, const QDateTime &end);
    QVariant data(const QModelIndex &item, int role) const;
};

#endif // MODELALARM_H
