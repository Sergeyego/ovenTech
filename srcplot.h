#ifndef SRCPLOT_H
#define SRCPLOT_H

#include <QWidget>
#include <qwt_plot_curve.h>
#include <QtGui>
#include <qwt_plot.h>
#include "cfgplot.h"
#include <QSqlQuery>
#include <QPushButton>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QColorDialog>

struct pnts{
    QVector<double> x;
    QVector<double> y;
};

class SrcPlot : public QWidget
{
    Q_OBJECT
public:
    explicit SrcPlot(const QString nam, QColor color, bool checked, QWidget *parent = 0);
    QwtPlotCurve* getCurve();
    void addData(const double &x, const double &y);
    void clear();
    void plotData();
private:
    pnts data;
    QwtPlotCurve *curve;
    QPushButton *btn;
    QCheckBox *chb;
    void setCurveColor(QColor color);
signals:
    
private slots:
    void chColor();
    void chVisible(bool v);
    
};

#endif // SRCPLOT_H
