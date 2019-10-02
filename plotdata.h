#ifndef PLOTDATA_H
#define PLOTDATA_H

#include <QWidget>
#include "srcplot.h"
#include <QMessageBox>
#include <QSqlError>

class PlotData : public QWidget
{
    Q_OBJECT
public:
    explicit PlotData(QWidget *parent = nullptr);
private:
    SrcPlot *srcTSt1;
    SrcPlot *srcTSt2;
    SrcPlot *srcTSt3;
    SrcPlot *srcTHeat;
    SrcPlot *srcUst;
    SrcPlot *srcTH2O;
    SrcPlot *srcPN2St1;
    SrcPlot *srcPN2St2;
    SrcPlot *srcPN2St3;
    ulong updData(SrcPlot *s, const QString &tablename, const QDateTime &beg, const QDateTime &end);

signals:

public slots:
    void refresh(const QDateTime &beg, const QDateTime &end);
};

#endif // PLOTDATA_H
