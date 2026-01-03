#include "borrowmodel.h"
#include "databasemanager.h"
#include <QColor>
#include <QDate>
#include <QDebug>

BorrowModel::BorrowModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_headers << "记录ID" << "图书" << "读者" << "借书日期" << "应还日期" << "还书日期" << "状态" << "逾期天数";
    refreshData();
}

int BorrowModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_records.size();
}

int BorrowModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_headers.size();
}

QVariant BorrowModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_records.size() || index.row() < 0)
        return QVariant();

    int row = index.row();
    int col = index.column();
    const QVariantMap &record = m_records.at(row);

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (col) {
        case 0: return record["id"]; // 记录ID
        case 1: return record["book_title"]; // 书名
        case 2: return record["reader_name"]; // 读者名
        case 3: return record["borrow_date"]; // 借书日期
        case 4: return record["due_date"]; // 应还日期
        case 5: return record["return_date"]; // 还书日期
        case 6: return record["status"]; // 状态
        case 7: { // 逾期天数
            int overdueDays = record["overdue_days"].toInt();
            return overdueDays > 0 ? QString::number(overdueDays) : QVariant(""); // 未逾期不显示
        }
        default: return QVariant();
        }
    }

    // 设置颜色：逾期记录用红色标出
    if (role == Qt::ForegroundRole) {
        QString status = record["status"].toString();
        int overdueDays = record["overdue_days"].toInt();
        if (status == "借出" && overdueDays > 0) {
            return QColor(Qt::red);
        }
    }

    // 状态列居中显示
    if (role == Qt::TextAlignmentRole) {
        if (col == 6 || col == 7) { // 状态和逾期天数列
            return Qt::AlignCenter;
        }
        return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);
    }

    return QVariant();
}

QVariant BorrowModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        if (section >= 0 && section < m_headers.size())
            return m_headers.at(section);
    }
    return QVariant();
}

void BorrowModel::refreshData()
{
    beginResetModel();
    loadDataFromDatabase();
    endResetModel();
}

QVariantMap BorrowModel::getRecordData(int row) const
{
    if (row >= 0 && row < m_records.size())
        return m_records.at(row);
    return QVariantMap();
}

void BorrowModel::setFilterStatus(const QString &status)
{
    m_currentStatusFilter = status;
    refreshData();
}

void BorrowModel::searchRecords(const QString &keyword)
{
    m_currentKeyword = keyword;
    refreshData();
}

void BorrowModel::loadDataFromDatabase()
{
    m_records.clear();
    // 本次提交先加载全部记录，筛选功能下次完善
    m_records = DatabaseManager::instance().getAllBorrowRecords();

    // 控制台输出，方便调试
    qDebug() << "加载了" << m_records.size() << "条借阅记录";
}
