#include "tableview.h"

TableView::TableView(QWidget *parent) : QTableView(parent)
{
    verticalHeader()->setDefaultSectionSize(verticalHeader()->fontMetrics().height()*1.5);
}

void TableView::resizeToContents()
{
    if (!model()) return;
    int n=model()->columnCount();
    int m=model()->rowCount();
    int max=0;
    QStringList l;
    QString s;
    /*for (int i=0; i<model()->rowCount(); i++){
        max=1;
        for (int j=0; j<n; j++){
            s=model()->data(model()->index(i,j)).toString();
            l=s.split("\n");
            if (max<l.size()) max=l.size();
        }
        setRowHeight(i,max*ui->tableView->fontMetrics().height()+12);
    }*/
    for (int i=0; i<n; i++){
        s=model()->headerData(i,Qt::Horizontal).toString();
        max=horizontalHeader()->fontMetrics().width(s);
        for (int j=0; j<m; j++){
            s=model()->data(model()->index(j,i)).toString();
            l=s.split("\n");
            for (int k=0; k<l.size(); k++){
                if (max<fontMetrics().width(l.at(k)))
                    max=fontMetrics().width(l.at(k));
            }
        }
        setColumnWidth(i,max+12);
    }
}

void TableView::save(QString fnam, int dec)
{
    int rows,cols;
    rows=this->model()->rowCount();
    cols=this->model()->columnCount();

    if (rows*cols>1){
        Document xlsx;
        Worksheet *ws=xlsx.currentWorksheet();
        ws->writeString(CellReference("A1"),fnam);
        Format strFormat;
        strFormat.setBorderStyle(Format::BorderThin);
        Format numFormat;
        numFormat.setBorderStyle(Format::BorderThin);

        int m=2;
        ws->setRowHeight(2,2,this->verticalHeader()->height()/14.0);
        for(int i=0;i<cols;i++) {
            if (!this->isColumnHidden(i)) {
                QString hCubeell=this->model()->headerData(i,Qt::Horizontal).toString();
                hCubeell.replace(QChar('\n'),QChar('\n'));
                ws->writeString(2,m,hCubeell,strFormat);
                ws->setColumnWidth(m,m,this->columnWidth(i)/8.0);
                m++;
            }
        }

        if (!this->verticalHeader()->isHidden()){
            m=3;
            ws->setColumnWidth(1,1,this->verticalHeader()->width()/7.0);
            ws->writeBlank(2,1,strFormat);
            for(int j=0;j<rows;j++) {
                if (!this->isRowHidden(j)) {
                    QString hCubeell=this->model()->headerData(j,Qt::Vertical).toString();
                    hCubeell.replace(QChar('\n'),QChar('\n'));
                    ws->writeString(m,1,hCubeell,strFormat);
                    m++;
                }
            }
        }

        for (int i=0;i<rows;i++){
            m=2;
            for(int j=0;j<cols;j++){
                if (!this->isColumnHidden(i)) {
                    int role=Qt::EditRole;
                    QVariant value=this->model()->data(this->model()->index(i,j),role);
                    int d=(value.typeName()==QString("int"))? 0 : dec;
                    if (d<0){
                        d=3;
                    }
                    if ((value.typeName()==QString("double"))||value.typeName()==QString("int")){
                        if (d>=1){
                            QString fmt=QString("0.%1").arg((0),d,'d',0,QChar('0'));
                            numFormat.setNumberFormat(fmt);
                        } else {
                            numFormat.setNumberFormat("0");
                        }
                        if (!model()->data(this->model()->index(i,j),Qt::DisplayRole).toString().isEmpty()){
                            ws->writeNumeric(i+3,m,value.toDouble(),numFormat);
                        } else {
                            ws->writeBlank(i+3,m,numFormat);
                        }
                    } else {
                        ws->writeString(i+3,m,value.toString(),strFormat);
                    }
                    m++;
                }
            }
        }

        QDir dir(QDir::homePath());
        QString filename = QFileDialog::getSaveFileName(nullptr,QString::fromUtf8("Сохранить документ"),
                                                        dir.path()+"/"+fnam+".xlsx",
                                                        QString::fromUtf8("Documents (*.xlsx)") );
        if (!filename.isEmpty()){
            xlsx.saveAs(filename);
        }
    }
}
