#ifndef BOOKMODEL_H
#define BOOKMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QVariantMap>
#include <QColor>
#include <QFlags>
#include <QDebug>

class BookModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit BookModel(QObject *parent = nullptr);

    // 必须重写的虚函数
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // 自定义功能函数
    void refreshData();  // 刷新数据
    QVariantMap getBookData(int row) const;  // 获取单行数据
    bool removeBook(int row);  // 删除图书
    bool addBook(const QVariantMap &bookData);  // 添加图书
    bool updateBook(int row, const QVariantMap &bookData);  // 更新图书
    void searchBooks(const QString &keyword);  // 搜索图书

    // 获取统计信息
    int getTotalBooks() const { return m_books.size(); }
    int getAvailableBooks() const;

private:
    QVector<QVariantMap> m_books;  // 存储图书数据的容器
    QStringList m_headers;         // 表格列标题
    QString m_currentFilter;       // 当前搜索关键词

    void loadBooksFromDatabase();  // 从数据库加载数据
};

#endif // BOOKMODEL_H
