#include "readermodel.h"
#include "databasemanager.h"
#include <QColor>
#include <QDebug>

ReaderModel::ReaderModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_headers << "ID" << "读者编号" << "姓名" << "性别" << "电话" << "邮箱" << "地址" << "注册日期";
    refreshData();
}

int ReaderModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_readers.size();
}

int ReaderModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_headers.size();
}

QVariant ReaderModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_readers.size() || index.row() < 0)
        return QVariant();

    int row = index.row();
    int col = index.column();
    const QVariantMap &reader = m_readers.at(row);

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (col) {
        case 0: return reader["id"];  // ID
        case 1: return reader["reader_id"];  // 读者编号
        case 2: return reader["name"];  // 姓名
        case 3: return reader["gender"];  // 性别
        case 4: return reader["phone"];  // 电话
        case 5: return reader["email"];  // 邮箱
        case 6: return reader["address"];  // 地址
        case 7: return reader["register_date"];  // 注册日期
        default: return QVariant();
        }
    }

    if (role == Qt::TextAlignmentRole) {
        if (col == 0)
            return static_cast<int>(Qt::AlignCenter);  // ID居中
        return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);  // 其他列左对齐
    }

    return QVariant();
}

QVariant ReaderModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        if (section >= 0 && section < m_headers.size())
            return m_headers.at(section);
    } else if (orientation == Qt::Vertical) {
        return section + 1;
    }

    return QVariant();
}

void ReaderModel::refreshData()
{
    beginResetModel();

    // 从数据库加载读者数据
    if (m_currentFilter.isEmpty()) {
        m_readers = DatabaseManager::instance().getAllReaders();
    } else {
        m_readers = DatabaseManager::instance().searchReaders(m_currentFilter);
    }

    endResetModel();

    emit dataChanged(createIndex(0, 0),
                     createIndex(rowCount()-1, columnCount()-1));
}

QVariantMap ReaderModel::getReaderData(int row) const
{
    if (row >= 0 && row < m_readers.size())
        return m_readers.at(row);

    return QVariantMap();
}

bool ReaderModel::removeReader(int row)
{
    if (row < 0 || row >= m_readers.size())
        return false;

    int readerId = m_readers.at(row)["id"].toInt();

    // 从数据库删除
    if (DatabaseManager::instance().deleteReader(readerId)) {
        // 从模型中删除该行
        beginRemoveRows(QModelIndex(), row, row);
        m_readers.removeAt(row);
        endRemoveRows();
        return true;
    }

    return false;
}

bool ReaderModel::addReader(const QVariantMap &readerData)
{
    // 添加到数据库
    if (DatabaseManager::instance().addReader(readerData)) {
        refreshData();
        return true;
    }
    return false;
}

bool ReaderModel::updateReader(int row, const QVariantMap &readerData)
{
    if (row < 0 || row >= m_readers.size())
        return false;

    int readerId = m_readers.at(row)["id"].toInt();

    // 更新数据库
    if (DatabaseManager::instance().updateReader(readerId, readerData)) {
        // 更新成功后刷新数据
        refreshData();
        return true;
    }

    return false;
}

void ReaderModel::searchReaders(const QString &keyword)
{
    m_currentFilter = keyword.trimmed();
    refreshData();
}

void ReaderModel::loadReadersFromDatabase()
{
    // 这个方法不再需要，因为refreshData已经调用getAllReaders
}
