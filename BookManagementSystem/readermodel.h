#ifndef READERMODEL_H
#define READERMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QVariantMap>

class ReaderModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ReaderModel(QObject *parent = nullptr);

    // 必须重写的虚函数
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // 自定义函数
    void refreshData();
    QVariantMap getReaderData(int row) const;
    bool removeReader(int row);
    bool addReader(const QVariantMap &readerData);
    bool updateReader(int row, const QVariantMap &readerData);
    void searchReaders(const QString &keyword);

    // 统计信息
    int getTotalReaders() const { return m_readers.size(); }

private:
    QVector<QVariantMap> m_readers;
    QStringList m_headers;
    QString m_currentFilter;

    void loadReadersFromDatabase();
};

#endif // READERMODEL_H
