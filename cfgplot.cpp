#include "cfgplot.h"
#include "ui_cfgplot.h"

Plot* Plot::plot_instance=0;

CfgPlot::CfgPlot(QwtPlot *plot, QWidget *parent) :
    QWidget(parent), qwtPlot(plot),
    ui(new Ui::CfgPlot)
{
    ui->setupUi(this);
    qwtPlot->setAxisScale(QwtPlot::yLeft,ui->spinBox_Ymin->value(),ui->spinBox_Ymax->value());
    qwtPlot->setAxisMaxMajor(QwtPlot::xBottom,ui->spinBox_Nx->value());
    qwtPlot->setAxisMaxMajor(QwtPlot::yLeft,ui->spinBox_Ny->value());

    connect(ui->spinBox_Nx,SIGNAL(valueChanged(int)),this,SLOT(setNx(int)));
    connect(ui->spinBox_Ny,SIGNAL(valueChanged(int)),this,SLOT(setNy(int)));
    connect(ui->spinBox_Ymax,SIGNAL(valueChanged(int)),this,SLOT(setYmax(int)));
    connect(ui->spinBox_Ymin,SIGNAL(valueChanged(int)),this,SLOT(setYmin(int)));
}

CfgPlot::~CfgPlot()
{
    delete ui;
}

void CfgPlot::setNx(int value)
{
    qwtPlot->setAxisMaxMajor(QwtPlot::xBottom,value);
    qwtPlot->replot();
}

void CfgPlot::setNy(int value)
{
    qwtPlot->setAxisMaxMajor(QwtPlot::yLeft,value);
    qwtPlot->replot();
}

void CfgPlot::setYmax(int value)
{
    qwtPlot->setAxisScale(QwtPlot::yLeft,ui->spinBox_Ymin->value(),value);
    qwtPlot->replot();
}

void CfgPlot::setYmin(int value)
{
    qwtPlot->setAxisScale(QwtPlot::yLeft,value,ui->spinBox_Ymax->value());
    qwtPlot->replot();
}

TimeScaleDraw::TimeScaleDraw()
{

}

QwtText TimeScaleDraw::label(double v) const
{
    return QDateTime::fromTime_t((qulonglong)v).toString("hh:mm");
}


TimePicker::TimePicker(int xAxis, int yAxis, QwtPicker::RubberBand rubberBand, QwtPicker::DisplayMode trackerMode, QwtPlotCanvas *canvas):
    QwtPlotPicker(xAxis,yAxis,rubberBand,trackerMode,canvas)
{
}

QwtText TimePicker::trackerTextF(const QPointF &pos) const
{
    return QDateTime::fromTime_t((qulonglong)pos.x()).toString("dd.MM.yy hh:mm:ss")+" ,"+QString::number(pos.y(),'f',0);
}


Plot::Plot(QWidget *parent) : QwtPlot(parent)
{
    setAutoReplot(false);
    setPalette(Qt::gray);
    setAxisTitle(QwtPlot::xBottom,QString::fromLocal8Bit("t, мин."));
    setAxisTitle(QwtPlot::yLeft,QString::fromLocal8Bit("T, град."));
    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableXMin(true);
    grid->setMajorPen(QPen(Qt::black,0,Qt::DotLine));
    grid->setMinorPen(QPen(Qt::gray,0,Qt::DotLine));
    grid->attach(this);
    setAxisScale(QwtPlot::xBottom,0,0);

    timeScaleDraw = new TimeScaleDraw();
    setAxisScaleDraw(QwtPlot::xBottom, timeScaleDraw);

    QwtPlotCanvas *canvas = new QwtPlotCanvas(this);

    timePicker = new TimePicker(QwtPlot::xBottom, QwtPlot::yLeft, QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn, canvas);
    timePicker->setTrackerPen(QColor(Qt::black));

    setCanvas(canvas);

}

Plot *Plot::instance()
{
    if (plot_instance == 0)
        plot_instance = new Plot;
    return plot_instance;
}

void Plot::setXRange(double min, double max)
{
    setAxisScale(QwtPlot::xBottom,min,max);
}
