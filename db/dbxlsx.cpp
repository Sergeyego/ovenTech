#include "dbxlsx.h"

DbXlsx::DbXlsx(QTableView *v, QString head, QObject *parent) : QObject(parent), viewer(v), title(head)
{

}

void DbXlsx::saveToFile()
{
    int rows,cols;
    rows=viewer->model()->rowCount();
    cols=viewer->model()->columnCount();

    if (rows*cols>1){
        Document xlsx;
        Worksheet *ws=xlsx.currentWorksheet();
        ws->writeString(CellReference("A1"),title);
        Format strFormat;
        strFormat.setBorderStyle(Format::BorderThin);
        Format numFormat;
        numFormat.setBorderStyle(Format::BorderThin);

        int m=1;
        ws->setRowHeight(2,2,viewer->verticalHeader()->height()/18.0);
        for(int i=0;i<cols;i++) {
            if (!viewer->isColumnHidden(i)) {
                QString hCubeell=viewer->model()->headerData(i,Qt::Horizontal).toString();
                hCubeell.replace(QChar('\n'),QChar('\n'));
                ws->writeString(2,m,hCubeell,strFormat);
                ws->setColumnWidth(m,m,viewer->columnWidth(i)/9.0);
                m++;
            }
        }

        DbTableModel *sqlModel = qobject_cast<DbTableModel *>(viewer->model());

        for (int i=0;i<rows;i++){
            m=1;
            for(int j=0;j<cols;j++){
                if (!viewer->isColumnHidden(i)) {
                    int role=Qt::EditRole;
                    int dec=0;
                    if (sqlModel){
                        if (sqlModel->relation(j) || sqlModel->columnType(j)==TYPE_BOOL || sqlModel->columnType(j)==TYPE_INTBOOL || sqlModel->columnType(j)==TYPE_DATE){
                            role=Qt::DisplayRole;
                        }
                        if (sqlModel->validator(j)){
                            QDoubleValidator *doublevalidator = qobject_cast<QDoubleValidator*>(sqlModel->validator(j));
                            if (doublevalidator) dec=doublevalidator->decimals();
                        }
                    }
                    QVariant value=viewer->model()->data(viewer->model()->index(i,j),role);
                    if ((value.typeName()==QString("double"))||value.typeName()==QString("int")){
                        if (dec>=1){
                            QString fmt=QString("0.%1").arg((0),dec,'d',0,QChar('0'));
                            numFormat.setNumberFormat(fmt);
                        } else {
                            numFormat.setNumberFormat("0");
                        }
                        ws->writeNumeric(i+3,m,value.toDouble(),numFormat);
                    } else {
                        ws->writeString(i+3,m,value.toString(),strFormat);
                    }
                    m++;
                }
            }
        }

        QDir dir(QDir::homePath());
        QString filename = QFileDialog::getSaveFileName(nullptr,QString::fromUtf8("Сохранить документ"),
                                                        dir.path()+"/"+title+".xlsx",
                                                        QString::fromUtf8("Documents (*.xlsx)") );
        if (!filename.isEmpty()){
            xlsx.saveAs(filename);
        }
    }
}
