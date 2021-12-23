#include "dialogprog.h"
#include "ui_dialogprog.h"

DialogProg::DialogProg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogProg)
{
    ui->setupUi(this);

    modelProg = new DbTableModel("techprogs",this);
    modelProg->addColumn("id","ID",true,TYPE_INT);
    modelProg->addColumn("nam","Название",false,TYPE_STRING);
    modelProg->setSort("techprogs.id");
    modelProg->select();

    modelCont = new DbTableModel("techprogsdata",this);
    modelCont->addColumn("id_progs","id_progs",true,TYPE_INT);
    modelCont->addColumn("step","Шаг",true,TYPE_INT,new QIntValidator(1,10,this));
    modelCont->addColumn("tim","Время, мин",false,TYPE_INT,new QIntValidator(1,1000,this));
    modelCont->addColumn("temp","Температура, град.",false,TYPE_DOUBLE,new QDoubleValidator(0,850,1,this));
    modelCont->setSort("techprogsdata.step");

    ui->tableViewCont->setModel(modelCont);
    ui->tableViewCont->setColumnHidden(0,true);
    ui->tableViewCont->setColumnWidth(1,50);
    ui->tableViewCont->setColumnWidth(2,120);
    ui->tableViewCont->setColumnWidth(3,120);

    ui->tableViewProg->setModel(modelProg);
    ui->tableViewProg->setColumnWidth(0,50);
    ui->tableViewProg->setColumnWidth(1,170);

    mapper = new DbMapper(ui->tableViewProg,this);
    ui->verticalLayoutCont->addWidget(mapper);
    mapper->addMapping(ui->lineEditName,1);
    mapper->addEmptyLock(ui->tableViewCont);

    connect(mapper,SIGNAL(currentIndexChanged(int)),this,SLOT(updCont(int)));

    if (ui->tableViewProg->model()->rowCount()) ui->tableViewProg->selectRow(0);
}

DialogProg::~DialogProg()
{
    delete ui;
}

void DialogProg::updCont(int index)
{
    int id_progs=ui->tableViewProg->model()->data(ui->tableViewProg->model()->index(index,0),Qt::EditRole).toInt();
    modelCont->setFilter("techprogsdata.id_progs="+QString::number(id_progs));
    modelCont->setDefaultValue(0,id_progs);
    modelCont->select();
}
