#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadsettings();

    ui->dateEditBeg->setDate(QDate::currentDate().addMonths(-1));
    ui->dateEditEnd->setDate(QDate::currentDate().addDays(1));

    ui->tabGraph->layout()->addWidget(Plot::instance());
    cfgPlot = new CfgPlot(Plot::instance(),this);
    ui->tabTime->layout()->addWidget(cfgPlot);

    modelAlarm = new ModelAlarm(this);
    modelAlarm->refresh(QDateTime(),QDateTime());
    ui->tableViewAlarm->setModel(modelAlarm);
    ui->tableViewAlarm->verticalHeader()->setDefaultSectionSize(ui->tableViewAlarm->verticalHeader()->fontMetrics().height()*1.5);
    ui->tableViewAlarm->setColumnWidth(0,120);
    ui->tableViewAlarm->setColumnWidth(1,320);
    ui->tableViewAlarm->setColumnWidth(2,100);

    modelProc = new ModelProc(this);
    modelProc->setDbeg(ui->dateEditBeg->date());
    modelProc->setDend(ui->dateEditEnd->date());
    modelProc->select();

    ui->tableViewProc->setModel(modelProc);
    ui->tableViewProc->setColumnHidden(0,true);
    ui->tableViewProc->setColumnWidth(1,115);
    ui->tableViewProc->setColumnWidth(2,25);
    ui->tableViewProc->setColumnWidth(3,200);
    ui->tableViewProc->setColumnWidth(4,45);
    ui->tableViewProc->setColumnWidth(5,45);

    plotData = new PlotData(this);
    ui->verticalLayoutData->addWidget(plotData);

    connect(ui->cmdUpdObj,SIGNAL(clicked(bool)),modelProc,SLOT(select()));
    connect(ui->dateEditBeg,SIGNAL(dateChanged(QDate)),modelProc,SLOT(setDbeg(QDate)));
    connect(ui->dateEditEnd,SIGNAL(dateChanged(QDate)),modelProc,SLOT(setDend(QDate)));
    connect(ui->tableViewProc->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(updCont(QModelIndex)));
    connect(ui->cmdUpdTime,SIGNAL(clicked(bool)),this,SLOT(updCont()));

    connect(ui->actionPrint,SIGNAL(triggered(bool)),this,SLOT(print()));
    connect(ui->actionExit,SIGNAL(triggered(bool)),this,SLOT(close()));
    connect(ui->actionTech,SIGNAL(triggered(bool)),this,SLOT(edtProg()));
    connect(ui->actionObj,SIGNAL(triggered(bool)),this,SLOT(cubeObj()));
    connect(ui->actionEnerg,SIGNAL(triggered(bool)),this,SLOT(cubeEnerg()));

    if (ui->tableViewProc->model()->rowCount()) ui->tableViewProc->selectRow(0);
}

MainWindow::~MainWindow()
{
    savesettings();
    delete ui;
}

void MainWindow::print()
{
    QwtPlotRenderer renderer;
    renderer.setDiscardFlag( QwtPlotRenderer::DiscardBackground );
    renderer.setDiscardFlag( QwtPlotRenderer::DiscardCanvasBackground );
    renderer.setDiscardFlag( QwtPlotRenderer::DiscardCanvasFrame );
    renderer.setLayoutFlag( QwtPlotRenderer::FrameWithScales );
    QPrinter printer;
    printer.setColorMode(QPrinter::Color);
    QPrintDialog dialog(&printer,this);
    if (dialog.exec()){
        QPainter painter(&printer);
        QRect page = printer.pageRect();
        QRect rect(page.x(), page.y(), page.width()*0.95, page.height()*0.95);
        renderer.render(Plot::instance(),&painter,rect);
    }
}

void MainWindow::edtProg()
{
    DialogProg d;
    d.exec();
    modelProc->relation(3)->model()->refresh();
}

void MainWindow::updCont(QModelIndex index)
{
    //int id_proc=ui->tableViewProc->model()->data(ui->tableViewProc->model()->index(index.row(),0),Qt::EditRole).toInt();
    if (!index.isValid()) return;
    QDateTime beg=QDateTime::fromTime_t(ui->tableViewProc->model()->data(ui->tableViewProc->model()->index(index.row(),1),Qt::EditRole).toULongLong());
    QDateTime end=beg.addDays(1);
    int stend=ui->tableViewProc->model()->data(ui->tableViewProc->model()->index(index.row(),2),Qt::EditRole).toInt();
    QString nam=ui->tableViewProc->model()->data(ui->tableViewProc->model()->index(index.row(),3),Qt::DisplayRole).toString();
    QSqlQuery query;
    query.prepare("select min(tm) from techproc where tm > :t");
    query.bindValue(":t",beg.toTime_t());
    if (query.exec()){
        while (query.next()){
            qulonglong te=query.value(0).toULongLong();
            if (te>0){
                end=QDateTime::fromTime_t(query.value(0).toULongLong());
            }
        }
    } else {
        QMessageBox::critical(this,"Ошибка",query.lastError().text(),QMessageBox::Ok);
    }
    if (end>beg.addDays(1)){
        end=beg.addDays(1);
    }
    Plot::instance()->setTitle(beg.toString("dd.MM.yy hh:mm")+" - "+end.toString("dd.MM.yy hh:mm")+"\n"+nam+" Стенд №"+QString::number(stend));
    ui->dateTimeEditBeg->setDateTime(beg);
    ui->dateTimeEditEnd->setDateTime(end);
    updCont(beg,end);
}

void MainWindow::updCont(const QDateTime &beg, const QDateTime &end)
{
    plotData->refresh(beg,end);
    modelAlarm->refresh(beg,end);
}

void MainWindow::updCont()
{
    QDateTime beg=ui->dateTimeEditBeg->dateTime();
    QDateTime end=ui->dateTimeEditEnd->dateTime();
    Plot::instance()->setTitle(beg.toString("dd.MM.yy hh:mm")+" - "+end.toString("dd.MM.yy hh:mm"));
    updCont(beg,end);
}

void MainWindow::cubeObj()
{
    QStringList axes;
    axes.push_back(tr("Стенд"));
    axes.push_back(tr("Марка"));
    axes.push_back(tr("Диаметр"));
    axes.push_back(tr("Название программы"));
    axes.push_back(tr("Год"));
    axes.push_back(tr("Месяц"));
    axes.push_back(tr("День"));
    QString query("select t.stend, pr.nam, d.diam,  p.nam, "
                  "substr(cast(to_timestamp(t.tm) as char(32)),1,4) as yr, "
                  "substr(cast(to_timestamp(t.tm) as char(32)),1,7) as mn, "
                  "substr(cast(to_timestamp(t.tm) as char(32)),1,10) as dy, "
                  "1 "
                  "from techproc as t "
                  "inner join techprogs as p on t.id_prg=p.id "
                  "inner join provol as pr on p.id_prov=pr.id "
                  "inner join diam as d on p.id_diam=d.id "
                  "where date_trunc('day',to_timestamp(t.tm)) between :d1 and :d2");
    CubeWidget *kvoCube = new CubeWidget(("Количество отжигов"),axes,query,0);
    kvoCube->show();
    connect(kvoCube,SIGNAL(sigClose()),kvoCube,SLOT(deleteLater()));
}

void MainWindow::cubeEnerg()
{
    QStringList axes;
    axes.push_back(tr("Стенд"));
    axes.push_back(tr("Марка"));
    axes.push_back(tr("Диаметр"));
    axes.push_back(tr("Название программы"));
    axes.push_back(tr("Год"));
    axes.push_back(tr("Месяц"));
    axes.push_back(tr("День"));
    QString query("select t.stend, pr.nam, d.diam,  p.nam, "
                  "substr(cast(to_timestamp(t.tm) as char(32)),1,4) as yr, "
                  "substr(cast(to_timestamp(t.tm) as char(32)),1,7) as mn, "
                  "substr(cast(to_timestamp(t.tm) as char(32)),1,10) as dy, "
                  "(select \"VAL\" from \"DBAVl_archVal_data_CQA\" "
                  "where \"TM\"=COALESCE((select max(\"TM\") from \"DBAVl_archVal_data_CQA\" where \"TM\"<=(select min(tm) from techproc where tm>t.tm)), "
                  "(select \"TM\" from \"DBAVl_archVal_data_CQA\" where \"TM\"=( select max(\"TM\") from \"DBAVl_archVal_data_CQA\" where date_trunc('day',to_timestamp(\"TM\"))<= :d2 )))) - "
                  "(select \"VAL\" from \"DBAVl_archVal_data_CQA\" where \"TM\"=(select max(\"TM\") from \"DBAVl_archVal_data_CQA\" where \"TM\"<=t.tm)) "
                  "from techproc as t "
                  "inner join techprogs as p on t.id_prg=p.id "
                  "inner join provol as pr on p.id_prov=pr.id "
                  "inner join diam as d on p.id_diam=d.id "
                  "where date_trunc('day',to_timestamp(t.tm)) between :d1 and :d2");
    CubeWidget *energCube = new CubeWidget(("Расход энергии, кВт*ч"),axes,query,0);
    energCube->show();
    connect(energCube,SIGNAL(sigClose()),energCube,SLOT(deleteLater()));
}

void MainWindow::loadsettings()
{
    QSettings settings("szsm", "ovenTech");
    this->restoreGeometry(settings.value("geometry").toByteArray());
    this->ui->splitter->restoreState(settings.value("splitter_width").toByteArray());
}

void MainWindow::savesettings()
{
    QSettings settings("szsm", "ovenTech");
    settings.setValue("geometry", this->saveGeometry());
    settings.setValue("splitter_width",ui->splitter->saveState());
}
