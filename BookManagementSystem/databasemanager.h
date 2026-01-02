#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVector>
#include <QVariantMap>
#include <QDate>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    // 单例模式，获取唯一实例
    static DatabaseManager& instance();

    // 初始化数据库连接
    bool initializeDatabase();

    // 图书管理
    bool addBook(const QVariantMap &bookData);
    bool updateBook(int id, const QVariantMap &bookData);
    bool deleteBook(int id);
    QVector<QVariantMap> getAllBooks();
    QVector<QVariantMap> searchBooks(const QString &keyword);
    QVariantMap getBookById(int id);

    // 读者管理
    bool addReader(const QVariantMap &readerData);
    bool updateReader(int id, const QVariantMap &readerData);
    bool deleteReader(int id);
    QVector<QVariantMap> getAllReaders();
    QVector<QVariantMap> searchReaders(const QString &keyword);

    // 借阅管理
    bool borrowBook(int bookId, int readerId, const QDate &dueDate);
    bool returnBook(int recordId);
    QVector<QVariantMap> getAllBorrowRecords();
    QVector<QVariantMap> getBorrowRecordsByReader(int readerId);
    QVector<QVariantMap> getOverdueBooks();

    // 统计数据
    int getBookCount();
    int getReaderCount();
    int getBorrowedCount();

private:
    // 私有构造函数，防止外部实例化
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    // 创建数据库表
    bool createTables();

    // 添加一些测试数据（开发阶段使用）
    void addTestData();

    QSqlDatabase m_database;
};

#endif // DATABASEMANAGER_H
