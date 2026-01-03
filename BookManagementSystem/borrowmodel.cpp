#include "borrowmodel.h"
#include "databasemanager.h"
#include <QColor>
#include <QDate>
#include <QDebug>

BorrowModel::BorrowModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_headers << "记录ID" << "图书" << "读者" << "借书日期" << "应还日期" << "还书日期" << "状态" << "逾期天数";

    // 初始化筛选条件为空
    m_currentStatusFilter = "";
    m_currentKeyword = "";

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
        case 6: {
            // 状态列：如果是借出且已逾期，显示"逾期"
            QString status = record["status"].toString();
            if (status == "借出") {
                QDate dueDate = record["due_date"].toDate();
                if (dueDate < QDate::currentDate()) {
                    return QString("逾期");
                }
            }
            return status;
        }
        case 7: { // 逾期天数
            if (record["status"].toString() == "借出") {
                QDate dueDate = record["due_date"].toDate();
                QDate currentDate = QDate::currentDate();
                if (dueDate < currentDate) {
                    int overdueDays = dueDate.daysTo(currentDate);
                    return overdueDays;
                }
            }
            return QVariant(); // 未逾期或已还，不显示天数
        }
        default: return QVariant();
        }
    }

    // 设置颜色：逾期记录用红色标出
    if (role == Qt::ForegroundRole && col == 6) { // 状态列
        QString status = record["status"].toString();
        if (status == "借出") {
            QDate dueDate = record["due_date"].toDate();
            if (dueDate < QDate::currentDate()) {
                return QColor(Qt::red);
            }
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
    if (row >= 0 && row < m_records.size()) {
        return m_records.at(row);
    }
    return QVariantMap();
}

void BorrowModel::setFilterStatus(const QString &status)
{
    m_currentStatusFilter = status;
    refreshData();
}

void BorrowModel::searchRecords(const QString &keyword)
{
    m_currentKeyword = keyword.trimmed();
    refreshData();
}

void BorrowModel::loadDataFromDatabase()
{
    beginResetModel();

    // 从数据库获取原始数据
    QVector<QVariantMap> allRecords = DatabaseManager::instance().getAllBorrowRecords();
    m_records.clear();

    // 筛选逻辑
    for (const auto &record : allRecords) {
        QString status = record["status"].toString();
        bool shouldInclude = true;

        // 根据状态筛选
        if (!m_currentStatusFilter.isEmpty() && m_currentStatusFilter != "全部") {
            if (m_currentStatusFilter == "逾期") {
                // 逾期状态：状态为"借出"且应还日期小于今天
                if (status == "借出") {
                    QDate dueDate = record["due_date"].toDate();
                    if (dueDate >= QDate::currentDate()) {
                        shouldInclude = false;  // 未逾期，不包含
                    }
                } else {
                    shouldInclude = false;  // 不是"借出"状态，不包含
                }
            } else if (status != m_currentStatusFilter) {
                shouldInclude = false;  // 状态不匹配，不包含
            }
        }

        // 根据关键词筛选
        if (shouldInclude && !m_currentKeyword.isEmpty()) {
            QString bookTitle = record["book_title"].toString();
            QString readerName = record["reader_name"].toString();
            QString isbn = record["book_isbn"].toString();

            if (!bookTitle.contains(m_currentKeyword, Qt::CaseInsensitive) &&
                !readerName.contains(m_currentKeyword, Qt::CaseInsensitive) &&
                !isbn.contains(m_currentKeyword, Qt::CaseInsensitive)) {
                shouldInclude = false;
            }
        }

        if (shouldInclude) {
            m_records.append(record);
        }
    }

    endResetModel();
    qDebug() << "加载了" << m_records.size() << "条借阅记录，筛选条件：状态="
             << m_currentStatusFilter << "，关键词=" << m_currentKeyword;
}
