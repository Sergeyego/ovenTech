#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "modelproc.h"
#include "cfgplot.h"
#include "qwt_plot_renderer.h"
#include <QPrinter>
#include <QPrintDialog>
#include "dialogprog.h"
#include "plotdata.h"
#include "modelalarm.h"
#include "olap/cubewidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void print();
    void edtProg();
    void updCont(QModelIndex index);
    void updCont(const QDateTime &beg, const QDateTime &end);
    void updCont();
    void cubeObj();
    void cubeEnerg();

private:
    Ui::MainWindow *ui;
    ModelProc *modelProc;
    CfgPlot *cfgPlot;
    PlotData *plotData;
    ModelAlarm *modelAlarm;
    void loadsettings();
    void savesettings();
};

#endif // MAINWINDOW_H
