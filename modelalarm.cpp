#include "modelalarm.h"

ModelAlarm::ModelAlarm(QObject *parent) : QSqlQueryModel(parent)
{

}

void ModelAlarm::refresh(const QDateTime &beg, const QDateTime &end)
{
    QSqlQuery query;
    query.prepare("select \"TM\", \"MESS\", \"CATEG\" from \"DBAMsg_archMess\" where \"TM\" between :t1 and :t2 order by \"TM\"");
    query.bindValue(":t1",beg.toTime_t());
    query.bindValue(":t2",end.toTime_t());
    if (query.exec()){
        this->setQuery(query);
        this->setHeaderData(0,Qt::Horizontal,"Дата и время");
        this->setHeaderData(1,Qt::Horizontal,"Сообщение");
        this->setHeaderData(2,Qt::Horizontal,"Категория");
    } else {
        QMessageBox::critical(NULL,"Ошибка",query.lastError().text(),QMessageBox::Ok);
    }
}

QVariant ModelAlarm::data(const QModelIndex &item, int role) const
{
    if (role==Qt::DisplayRole && item.column()==0){
        return QDateTime::fromTime_t(this->data(item,Qt::EditRole).toULongLong()).toString("dd.MM.yy hh:mm:ss");
    }
    return QSqlQueryModel::data(item,role);
}
