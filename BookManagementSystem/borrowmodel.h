#ifndef BORROWMODEL_H
#define BORROWMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QVariantMap>

class BorrowModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit BorrowModel(QObject *parent = nullptr);

    // 重写基类函数
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // 自定义功能
    void refreshData();
    QVariantMap getRecordData(int row) const;
    void setFilterStatus(const QString &status); // 按状态筛选
    void searchRecords(const QString &keyword);

private:
    QVector<QVariantMap> m_records;
    QStringList m_headers;
    QString m_currentStatusFilter;
    QString m_currentKeyword;

    void loadDataFromDatabase();
};

#endif // BORROWMODEL_H
