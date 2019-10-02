#include "plotdata.h"

PlotData::PlotData(QWidget *parent) : QWidget(parent)
{
    this->setLayout(new QVBoxLayout());
    srcTSt1 = new SrcPlot("Тепература стенда №1, град.",QColor("#ae77c3"),true,this);
    this->layout()->addWidget(srcTSt1);

    srcTSt2 = new SrcPlot("Тепература стенда №2, град.",QColor("#ffa500"),true,this);
    this->layout()->addWidget(srcTSt2);

    srcTSt3 = new SrcPlot("Тепература стенда №3, град.",QColor("#0000ff"),true,this);
    this->layout()->addWidget(srcTSt3);

    srcTHeat = new SrcPlot("Тепература нагревательного колпака, град.",QColor("#90ee90"),true,this);
    this->layout()->addWidget(srcTHeat);

    srcUst = new SrcPlot("Тепература заданная, град.",QColor("#00ffff"),true,this);
    this->layout()->addWidget(srcUst);

    srcTH2O = new SrcPlot("Тепература воды, град.",QColor("#ff6347"),false,this);
    this->layout()->addWidget(srcTH2O);

    srcPN2St1 = new SrcPlot("Давление N2 Ст. 1, кПа",QColor("#005500"),false,this);
    this->layout()->addWidget(srcPN2St1);

    srcPN2St2 = new SrcPlot("Давление N2 Ст. 2, кПа",QColor("#af8647"),false,this);
    this->layout()->addWidget(srcPN2St2);

    srcPN2St3 = new SrcPlot("Давление N2 Ст. 3, кПа",QColor("#00557f"),false,this);
    this->layout()->addWidget(srcPN2St3);
}

ulong PlotData::updData(SrcPlot *s, const QString &tablename, const QDateTime &beg, const QDateTime &end)
{
    s->clear();
    ulong max=0;
    QSqlQuery query;
    query.prepare("select \"TM\", \"VAL\" from "+tablename+" where \"TM\" between :t1 and :t2 order by \"TM\"");
    query.bindValue(":t1",beg.toTime_t());
    query.bindValue(":t2",end.toTime_t());
    if (query.exec()){
        while (query.next()){
            s->addData(query.value(0).toULongLong(),query.value(1).toDouble());
            if (max<query.value(0).toULongLong()){
                max=query.value(0).toULongLong();
            }
        }
        s->plotData();
    } else {
        QMessageBox::critical(this,"Ошибка",query.lastError().text(),QMessageBox::Ok);
    }
    return max;
}


void PlotData::refresh(const QDateTime &beg, const QDateTime &end)
{
    QVector<ulong> m;
    ulong max=0;
    m.push_back(updData(srcTSt1,QString("\"DBAVl_archVal_TempSt1_var\""),beg,end));
    m.push_back(updData(srcTSt2,QString("\"DBAVl_archVal_TempSt2_var\""),beg,end));
    m.push_back(updData(srcTSt3,QString("\"DBAVl_archVal_TempSt3_var\""),beg,end));
    m.push_back(updData(srcTHeat,QString("\"DBAVl_archVal_TempHeat_var\""),beg,end));
    m.push_back(updData(srcUst,QString("\"DBAVl_archVal_Tust_var\""),beg,end));
    m.push_back(updData(srcTH2O,QString("\"DBAVl_archVal_TH2O_var\""),beg,end));
    m.push_back(updData(srcPN2St1,QString("\"DBAVl_archVal_PresN2St1_var\""),beg,end));
    m.push_back(updData(srcPN2St2,QString("\"DBAVl_archVal_PresN2St2_var\""),beg,end));
    m.push_back(updData(srcPN2St3,QString("\"DBAVl_archVal_PresN2St3_var\""),beg,end));

    for (int i=0; i<m.size(); i++){
        if (max<m.at(i)){
            max=m.at(i);
        }
    }

    Plot::instance()->setXRange(beg.toTime_t(),/*end.toTime_t()*/max);
    Plot::instance()->replot();
}
