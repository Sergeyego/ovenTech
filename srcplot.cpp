#include "srcplot.h"

SrcPlot::SrcPlot(const QString nam, QColor color, bool checked, QWidget *parent) :
    QWidget(parent)
{
    this->setLayout(new QHBoxLayout);
    chb = new QCheckBox(nam,this);
    chb->setChecked(checked);
    btn = new QPushButton(tr("—"),this);
    btn->setMaximumSize(40,20);
    QFont font("Times", 25, QFont::Bold);
    btn->setFont(font);
    layout()->addWidget(chb);
    layout()->addWidget(btn);
    curve = new QwtPlotCurve();
    curve->setVisible(checked);
    curve->attach(Plot::instance());
    setCurveColor(color);
    connect(btn,SIGNAL(clicked()),this,SLOT(chColor()));
    connect(chb,SIGNAL(clicked(bool)),this,SLOT(chVisible(bool)));
}

QwtPlotCurve *SrcPlot::getCurve()
{
    return curve;
}

void SrcPlot::addData(const double &x, const double &y)
{
    data.x.push_back(x);
    data.y.push_back(y);
}

void SrcPlot::clear()
{
    data.x.clear();
    data.y.clear();
    curve->setSamples(data.x,data.y);
}

void SrcPlot::plotData()
{
    curve->setSamples(data.x,data.y);
}

void SrcPlot::setCurveColor(QColor color)
{
    QPalette tpalette = btn->palette();
    tpalette.setColor(QPalette::ButtonText,color);
    QPen pen=curve->pen();
    pen.setWidth(2);
    pen.setColor(color);
    btn->setPalette(tpalette);
    curve->setPen(pen);
    curve->plot()->replot();
}

void SrcPlot::chColor()
{
    QColor clr = QColorDialog::getColor(curve->pen().color());
    if (clr.isValid()) setCurveColor(clr);
}

void SrcPlot::chVisible(bool v)
{
    curve->setVisible(v);
    curve->plot()->replot();
}

