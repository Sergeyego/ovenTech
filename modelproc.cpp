#include "modelproc.h"

ModelProc::ModelProc(QObject *parent) : DbTableModel("techproc",parent)
{
    relProgs = new DbRelation("select p.id, pr.nam ||' ' || d.diam ||' '|| p.nam from techProgs as p "
                              "left join provol as pr on p.id_prov=pr.id "
                              "left join diam as d on p.id_diam=d.id "
                              "order by pr.nam, d.diam, p.nam",0,1,this);
    addColumn("id","ID",true,TYPE_INT);
    addColumn("tm","Дата и время",false,TYPE_INT);
    addColumn("stend","Ст.",false,TYPE_INT);
    addColumn("id_prg","Программа",false,TYPE_STRING,NULL,relProgs);
    addColumn("tbeg","Нач. t",false,TYPE_DOUBLE,new QDoubleValidator(0,850,1,this));
    addColumn("timbeg","Н.выд.",false,TYPE_DOUBLE,new QDoubleValidator(0,1000,0,this));
    setSort("techproc.tm");
}

QVariant ModelProc::data(const QModelIndex &index, int role) const
{
    if (index.column()==1 && role==Qt::DisplayRole){
        return QDateTime::fromTime_t(this->data(index,Qt::EditRole).toULongLong()).toString("dd.MM.yy hh:mm");
    }
    return DbTableModel::data(index,role);
}

void ModelProc::setDbeg(QDate d)
{
    dBeg=d;
}

void ModelProc::setDend(QDate d)
{
    dEnd=d;
}

bool ModelProc::select()
{
    setFilter("techproc.tm between "+QString::number(QDateTime(dBeg).toTime_t())+" and "+QString::number(QDateTime(dEnd,QTime(23,59,59)).toTime_t()));
    return DbTableModel::select();
}
