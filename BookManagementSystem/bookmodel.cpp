#include "bookmodel.h"
#include "databasemanager.h"
#include <QDebug>
#include <QDate>

BookModel::BookModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    // 设置表格列标题
    m_headers << "ID" << "ISBN" << "书名" << "作者" << "出版社"
              << "出版日期" << "分类" << "总数量" << "可借数量" << "价格(元)";

    // 初始加载数据
    refreshData();
}

int BookModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_books.size();  // 返回数据行数
}

int BookModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_headers.size();  // 返回列数
}

QVariant BookModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_books.size() || index.row() < 0)
        return QVariant();

    int row = index.row();
    int col = index.column();
    const QVariantMap &book = m_books.at(row);

    // 显示文本数据
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (col) {
        case 0: return book["id"];  // ID
        case 1: return book["isbn"];  // ISBN
        case 2: return book["title"];  // 书名
        case 3: return book["author"];  // 作者
        case 4: return book["publisher"];  // 出版社
        case 5: return book["publish_date"];  // 出版日期
        case 6: return book["category"];  // 分类
        case 7: return book["total_count"];  // 总数量
        case 8: return book["available_count"];  // 可借数量
        case 9: return book["price"];  // 价格
        default: return QVariant();
        }
    }

    // 设置文本对齐方式
    if (role == Qt::TextAlignmentRole) {
        if (col >= 7 && col <= 9) {  // 数字列居中对齐
            return Qt::AlignCenter;
        }
        return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);
    }

    // 设置可借数量的颜色（红色表示不可借）
    if (role == Qt::ForegroundRole && col == 8) {
        int available = book["available_count"].toInt();
        if (available <= 0) {
            return QColor(Qt::red);  // 红色表示不可借
        } else if (available < 3) {
            return QColor(Qt::darkYellow);  // 黄色表示数量少
        }
    }

    return QVariant();
}

QVariant BookModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        // 返回列标题
        if (section >= 0 && section < m_headers.size())
            return m_headers.at(section);
    } else if (orientation == Qt::Vertical) {
        // 返回行号（从1开始）
        return section + 1;
    }

    return QVariant();
}

bool BookModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    int row = index.row();
    int col = index.column();

    if (row < 0 || row >= m_books.size())
        return false;

    QVariantMap &book = m_books[row];
    QString fieldName;

    // 根据列索引确定要修改的字段
    switch (col) {
    case 1: fieldName = "isbn"; break;
    case 2: fieldName = "title"; break;
    case 3: fieldName = "author"; break;
    case 4: fieldName = "publisher"; break;
    case 5: fieldName = "publish_date"; break;
    case 6: fieldName = "category"; break;
    case 7: fieldName = "total_count"; break;
    case 8: fieldName = "available_count"; break;
    case 9: fieldName = "price"; break;
    default: return false;
    }

    // 更新数据
    book[fieldName] = value;

    // 通知视图数据已更改
    emit dataChanged(index, index, {role});

    // 这里应该将修改保存到数据库
    // 暂时先更新内存数据，后续会添加数据库更新

    return true;
}

Qt::ItemFlags BookModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = QAbstractTableModel::flags(index);

    // ID列（第0列）不可编辑，其他列可编辑
    if (index.column() != 0) {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}

void BookModel::refreshData()
{
    beginResetModel();  // 通知视图数据将要重置

    loadBooksFromDatabase();  // 从数据库重新加载数据

    endResetModel();  // 通知视图数据重置完成

    emit dataChanged(createIndex(0, 0),
                     createIndex(rowCount()-1, columnCount()-1));
}

QVariantMap BookModel::getBookData(int row) const
{
    if (row >= 0 && row < m_books.size())
        return m_books.at(row);

    return QVariantMap();
}

bool BookModel::removeBook(int row)
{
    if (row < 0 || row >= m_books.size())
        return false;

    beginRemoveRows(QModelIndex(), row, row);  // 通知视图开始删除行

    // 从内存中移除
    m_books.removeAt(row);

    endRemoveRows();  // 通知视图删除完成

    // 注意：这里只是从模型中移除，还需要从数据库删除
    // 数据库删除将在后续与DatabaseManager整合时添加

    return true;
}

bool BookModel::addBook(const QVariantMap &bookData)
{
    int newRow = m_books.size();

    beginInsertRows(QModelIndex(), newRow, newRow);  // 通知视图开始插入行

    // 添加到内存数据
    m_books.append(bookData);

    endInsertRows();  // 通知视图插入完成

    // 注意：这里只是添加到模型，还需要添加到数据库
    // 数据库添加将在后续与DatabaseManager整合时添加

    return true;
}

bool BookModel::updateBook(int row, const QVariantMap &bookData)
{
    if (row < 0 || row >= m_books.size())
        return false;

    // 更新内存数据
    m_books[row] = bookData;

    // 通知视图该行数据已更改
    emit dataChanged(createIndex(row, 0),
                     createIndex(row, columnCount()-1));

    // 注意：这里只是更新模型，还需要更新数据库
    // 数据库更新将在后续与DatabaseManager整合时添加

    return true;
}

void BookModel::searchBooks(const QString &keyword)
{
    m_currentFilter = keyword.trimmed();
    refreshData();  // 刷新数据时会应用筛选
}

int BookModel::getAvailableBooks() const
{
    int availableCount = 0;
    for (const auto &book : m_books) {
        availableCount += book["available_count"].toInt();
    }
    return availableCount;
}

void BookModel::loadBooksFromDatabase()
{
    m_books.clear();

    // 临时测试数据 - 后续会替换为从DatabaseManager获取真实数据
    QVariantMap book1;
    book1["id"] = 1;
    book1["isbn"] = "978-7-302-12345-6";
    book1["title"] = "Qt 5开发实战";
    book1["author"] = "张三";
    book1["publisher"] = "清华大学出版社";
    book1["publish_date"] = "2022-01-01";
    book1["category"] = "计算机";
    book1["total_count"] = 5;
    book1["available_count"] = 3;
    book1["price"] = 89.9;
    m_books.append(book1);

    QVariantMap book2;
    book2["id"] = 2;
    book2["isbn"] = "978-7-115-23456-7";
    book2["title"] = "C++ Primer";
    book2["author"] = "李四";
    book2["publisher"] = "人民邮电出版社";
    book2["publish_date"] = "2020-06-01";
    book2["category"] = "计算机";
    book2["total_count"] = 3;
    book2["available_count"] = 0;
    book2["price"] = 128.0;
    m_books.append(book2);

    QVariantMap book3;
    book3["id"] = 3;
    book3["isbn"] = "978-7-111-34567-8";
    book3["title"] = "深入理解计算机系统";
    book3["author"] = "王五";
    book3["publisher"] = "机械工业出版社";
    book3["publish_date"] = "2019-03-15";
    book3["category"] = "计算机";
    book3["total_count"] = 2;
    book3["available_count"] = 1;
    book3["price"] = 139.0;
    m_books.append(book3);

    // 如果有关键词，进行筛选
    if (!m_currentFilter.isEmpty()) {
        QVector<QVariantMap> filteredBooks;
        QString filter = m_currentFilter.toLower();

        for (const auto &book : m_books) {
            if (book["title"].toString().toLower().contains(filter) ||
                book["author"].toString().toLower().contains(filter) ||
                book["isbn"].toString().toLower().contains(filter) ||
                book["publisher"].toString().toLower().contains(filter)) {
                filteredBooks.append(book);
            }
        }

        m_books = filteredBooks;
    }
}
