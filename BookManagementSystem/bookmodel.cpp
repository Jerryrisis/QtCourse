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

    int bookId = m_books.at(row)["id"].toInt();

    // 从数据库中删除
    if (DatabaseManager::instance().deleteBook(bookId)) {
        // 从模型中删除该行
        beginRemoveRows(QModelIndex(), row, row);
        m_books.removeAt(row);
        endRemoveRows();

        // 更新状态信息
        emit dataChanged(createIndex(0, 0), createIndex(rowCount()-1, columnCount()-1));
        return true;
    }

    return false;
}

bool BookModel::addBook(const QVariantMap &bookData)
{
    // 先添加到数据库
    if (DatabaseManager::instance().addBook(bookData)) {
        // 成功添加到数据库后，刷新显示
        refreshData();
        return true;
    }
    return false;
}

bool BookModel::updateBook(int row, const QVariantMap &bookData)
{
    if (row < 0 || row >= m_books.size())
        return false;

    int bookId = m_books.at(row)["id"].toInt();

    // 更新数据库
    if (DatabaseManager::instance().updateBook(bookId, bookData)) {
        // 更新成功后刷新数据
        refreshData();
        return true;
    }

    return false;
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
    beginResetModel();  // 通知视图数据将要重置

    m_books.clear();    // 清空现有数据

    // 从数据库获取真实数据
    if (m_currentFilter.isEmpty()) {
        // 没有搜索关键词，获取所有图书
        m_books = DatabaseManager::instance().getAllBooks();
    } else {
        // 有搜索关键词，执行搜索
        m_books = DatabaseManager::instance().searchBooks(m_currentFilter);
    }

    endResetModel();  // 通知视图数据重置完成

    // 输出调试信息
    if (m_books.isEmpty()) {
        if (m_currentFilter.isEmpty()) {
            qDebug() << "数据库中没有图书数据，请先添加图书";
        } else {
            qDebug() << "未找到包含关键词 '" << m_currentFilter << "' 的图书";
        }
    } else {
        qDebug() << "从数据库加载了" << m_books.size() << "本图书";
    }
}


bool BookModel::canDeleteBook(int row) const
{
    if (row < 0 || row >= m_books.size())
        return false;

    const QVariantMap &book = m_books.at(row);
    int availableCount = book["available_count"].toInt();
    int totalCount = book["total_count"].toInt();

    // 只有当所有图书都可借时才能删除
    return (availableCount == totalCount);
}
