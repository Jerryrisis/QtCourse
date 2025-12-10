 #include "idatabase.h"
#include <QUuid>

void IDatabase::ininDatabase()
{
    database=QSqlDatabase::addDatabase("QSQLITE");
    QString aFile = "D:/QTLab4/Lab4.db";

    database.setDatabaseName(aFile);


    if(!database.open())
    {
        qDebug()<<"failed to open database" << database.lastError();
    }
    else
    {
        qDebug()<<"open database is ok";
        qDebug()<<"Database connected:" << database.isOpen();
        QSqlQuery query;
        if(query.exec("PRAGMA table_info(User)"))
        {
            qDebug() << "=== User表结构 ===";
            while(query.next())
            {
                qDebug() << "字段" << query.value(1).toString() << "类型:" << query.value(2).toString();
            }
        }
    }

    //qDebug() << "Database tables:" << database.tables();

}

bool IDatabase::initPatientModel()
{
    patientTabModel = new QSqlTableModel(this, database);
    patientTabModel->setTable("patient");
    patientTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //patientTabModel->setSort(patientTabModel->fieldIndex("name"),Qt::AscendingOrder);
    patientTabModel->setSort(patientTabModel->fieldIndex("id"), Qt::AscendingOrder);
    if(!(patientTabModel->select()))
        return false;

    thePatientSelection = new QItemSelectionModel(patientTabModel);
    return true;
}

bool IDatabase::searchPatient(QString filter)
{
    patientTabModel->setFilter(filter);
    return patientTabModel->select();
}

bool IDatabase::deleteCurrentPatient()
{
    QModelIndex curIndex = thePatientSelection->currentIndex();
    patientTabModel->removeRow(curIndex.row());
    patientTabModel->submitAll();
    patientTabModel->select();
}

bool IDatabase::submitPatientEdit()
{
    return patientTabModel->submitAll();
}

void IDatabase::revertPatientEdit()
{
    patientTabModel->revertAll();
}

int IDatabase::addNewPatient()
{
    patientTabModel->insertRow(patientTabModel->rowCount(), QModelIndex());

    QModelIndex curIndex = patientTabModel->index(patientTabModel->rowCount() - 1, 1);

    int curRecNo = curIndex.row();
    QSqlRecord curRec = patientTabModel->record(curRecNo);
    curRec.setValue("CREATEDTIMESTAMP", QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    curRec.setValue("ID",QUuid::createUuid().toString(QUuid::WithoutBraces));
    patientTabModel->setRecord(curRecNo, curRec);

    return curIndex.row();
}

QString IDatabase::userLogin(QString userName, QString password)
{
    QSqlQuery query;
    query.prepare("select USERNAME,PASSWORD from User where USERNAME = :USER");
    query.bindValue(":USER",userName);
    query.exec();
    if(query.first() && query.value("USERNAME").isValid())
    {
        QString passwd = query.value("PASSWORD").toString();
        if(passwd == password)
        {
            qDebug()<<"loginOK";
            return "loginOK";
        }
        else
        {
            qDebug()<<"wrongPassword";
            return "wrongPassword";
        }
    }
    else
    {
        qDebug()<<"no such user";
        return "wrongUsername";
    }
    query.first();
}



IDatabase::IDatabase(QObject *parent)
    : QObject{parent}
{
    ininDatabase();
}
