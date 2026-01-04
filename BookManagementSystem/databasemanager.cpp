#include "databasemanager.h"
#include <QSqlRecord>

// 单例实例
DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

// 私有构造函数
DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{
    // 设置数据库连接名称（避免重复连接）
    static int connectionCount = 0;
    QString connectionName = QString("LibraryConnection%1").arg(++connectionCount);

    // 配置SQLite数据库
    m_database = QSqlDatabase::addDatabase("QSQLITE", connectionName);

    // 设置数据库文件路径（存储在用户的应用数据目录）
    QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(dbPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString dbFile = dbPath + "/library.db";
    m_database.setDatabaseName(dbFile);
    qDebug() << "Database path:" << dbFile;
}

DatabaseManager::~DatabaseManager()
{
    if (m_database.isOpen()) {
        m_database.close();
    }
}

// 初始化数据库
bool DatabaseManager::initializeDatabase()
{
    if (!m_database.open()) {
        qDebug() << "Error: Failed to open database:" << m_database.lastError();
        return false;
    }

    if (!createTables()) {
        qDebug() << "Error: Failed to create tables:" << m_database.lastError();
        return false;
    }

    // 如果是首次创建数据库，可以添加一些测试数据
    QSqlQuery query(m_database);
    query.exec("SELECT COUNT(*) FROM books");
    if (query.next() && query.value(0).toInt() == 0) {
        addTestData();
    }

    return true;
}

// 创建数据库表
bool DatabaseManager::createTables()
{
    QSqlQuery query(m_database);

    // 图书表
    QString bookTableSql = R"(
        CREATE TABLE IF NOT EXISTS books (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            isbn TEXT UNIQUE NOT NULL,
            title TEXT NOT NULL,
            author TEXT NOT NULL,
            publisher TEXT,
            publish_date DATE,
            category TEXT,
            total_count INTEGER DEFAULT 1,
            available_count INTEGER DEFAULT 1,
            price REAL DEFAULT 0.0,
            description TEXT
        )
    )";

    // 读者表
    QString readerTableSql = R"(
        CREATE TABLE IF NOT EXISTS readers (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            reader_id TEXT UNIQUE NOT NULL,
            name TEXT NOT NULL,
            gender TEXT,
            phone TEXT,
            email TEXT,
            address TEXT,
            register_date DATE NOT NULL
        )
    )";

    // 借阅记录表
    QString borrowTableSql = R"(
        CREATE TABLE IF NOT EXISTS borrow_records (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            book_id INTEGER NOT NULL,
            reader_id INTEGER NOT NULL,
            borrow_date DATE NOT NULL,
            due_date DATE NOT NULL,
            return_date DATE,
            status TEXT DEFAULT '借出',
            FOREIGN KEY(book_id) REFERENCES books(id),
            FOREIGN KEY(reader_id) REFERENCES readers(id)
        )
    )";

    bool success = true;
    success = success && query.exec(bookTableSql);
    success = success && query.exec(readerTableSql);
    success = success && query.exec(borrowTableSql);

    return success;
}

// 添加测试数据
void DatabaseManager::addTestData()
{
    // 添加测试图书
    QVariantMap book1;
    book1["isbn"] = "978-7-302-12345-6";
    book1["title"] = "Qt 5开发实战";
    book1["author"] = "张三";
    book1["publisher"] = "清华大学出版社";
    book1["publish_date"] = QDate(2022, 1, 1);
    book1["category"] = "计算机";
    book1["total_count"] = 5;
    book1["available_count"] = 5;
    book1["price"] = 89.9;
    book1["description"] = "Qt 5开发入门教程";
    addBook(book1);

    QVariantMap book2;
    book2["isbn"] = "978-7-115-23456-7";
    book2["title"] = "C++ Primer";
    book2["author"] = "李四";
    book2["publisher"] = "人民邮电出版社";
    book2["publish_date"] = QDate(2020, 6, 1);
    book2["category"] = "计算机";
    book2["total_count"] = 3;
    book2["available_count"] = 3;
    book2["price"] = 128.0;
    book2["description"] = "C++经典教程";
    addBook(book2);

    // 添加测试读者
    QVariantMap reader1;
    reader1["reader_id"] = "R2023001";
    reader1["name"] = "王小明";
    reader1["gender"] = "男";
    reader1["phone"] = "13800138000";
    reader1["email"] = "wang@example.com";
    reader1["address"] = "北京市海淀区";
    reader1["register_date"] = QDate::currentDate();
    addReader(reader1);

    QVariantMap reader2;
    reader2["reader_id"] = "R2023002";
    reader2["name"] = "李小红";
    reader2["gender"] = "女";
    reader2["phone"] = "13900139000";
    reader2["email"] = "li@example.com";
    reader2["address"] = "上海市浦东新区";
    reader2["register_date"] = QDate::currentDate();
    addReader(reader2);
}

// 图书管理相关方法实现
bool DatabaseManager::addBook(const QVariantMap &bookData)
{
    QSqlQuery query(m_database);
    query.prepare(R"(
        INSERT INTO books (isbn, title, author, publisher, publish_date,
                          category, total_count, available_count, price, description)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
    )");

    query.addBindValue(bookData["isbn"]);
    query.addBindValue(bookData["title"]);
    query.addBindValue(bookData["author"]);
    query.addBindValue(bookData["publisher"]);
    query.addBindValue(bookData["publish_date"]);
    query.addBindValue(bookData["category"]);
    query.addBindValue(bookData["total_count"]);
    query.addBindValue(bookData["available_count"]);
    query.addBindValue(bookData["price"]);
    query.addBindValue(bookData["description"]);

    return query.exec();
}

bool DatabaseManager::updateBook(int id, const QVariantMap &bookData)
{
    QSqlQuery query(m_database);
    query.prepare(R"(
        UPDATE books SET isbn=?, title=?, author=?, publisher=?, publish_date=?,
                        category=?, total_count=?, available_count=?, price=?, description=?
        WHERE id=?
    )");

    query.addBindValue(bookData["isbn"]);
    query.addBindValue(bookData["title"]);
    query.addBindValue(bookData["author"]);
    query.addBindValue(bookData["publisher"]);
    query.addBindValue(bookData["publish_date"]);
    query.addBindValue(bookData["category"]);
    query.addBindValue(bookData["total_count"]);
    query.addBindValue(bookData["available_count"]);
    query.addBindValue(bookData["price"]);
    query.addBindValue(bookData["description"]);
    query.addBindValue(id);

    return query.exec();
}

bool DatabaseManager::deleteBook(int id)
{
    QSqlQuery query(m_database);
    query.prepare("DELETE FROM books WHERE id = ?");
    query.addBindValue(id);
    return query.exec();
}

QVector<QVariantMap> DatabaseManager::getAllBooks()
{
    QVector<QVariantMap> books;
    QSqlQuery query("SELECT * FROM books ORDER BY id DESC", m_database);

    while (query.next()) {
        QVariantMap book;
        QSqlRecord record = query.record();
        for (int i = 0; i < record.count(); i++) {
            book[record.fieldName(i)] = query.value(i);
        }
        books.append(book);
    }

    return books;
}

QVector<QVariantMap> DatabaseManager::searchBooks(const QString &keyword)
{
    QVector<QVariantMap> books;
    QSqlQuery query(m_database);
    query.prepare(R"(
        SELECT * FROM books
        WHERE title LIKE ? OR author LIKE ? OR isbn LIKE ? OR category LIKE ?
        ORDER BY id DESC
    )");

    QString searchPattern = "%" + keyword + "%";
    query.addBindValue(searchPattern);
    query.addBindValue(searchPattern);
    query.addBindValue(searchPattern);
    query.addBindValue(searchPattern);

    if (query.exec()) {
        while (query.next()) {
            QVariantMap book;
            QSqlRecord record = query.record();
            for (int i = 0; i < record.count(); i++) {
                book[record.fieldName(i)] = query.value(i);
            }
            books.append(book);
        }
    }

    return books;
}

QVariantMap DatabaseManager::getBookById(int id)
{
    QVariantMap book;
    QSqlQuery query(m_database);
    query.prepare("SELECT * FROM books WHERE id = ?");
    query.addBindValue(id);

    if (query.exec() && query.next()) {
        QSqlRecord record = query.record();
        for (int i = 0; i < record.count(); i++) {
            book[record.fieldName(i)] = query.value(i);
        }
    }

    return book;
}

// 读者管理相关方法实现
bool DatabaseManager::addReader(const QVariantMap &readerData)
{
    QSqlQuery query(m_database);
    query.prepare(R"(
        INSERT INTO readers (reader_id, name, gender, phone, email, address, register_date)
        VALUES (?, ?, ?, ?, ?, ?, ?)
    )");

    query.addBindValue(readerData["reader_id"]);
    query.addBindValue(readerData["name"]);
    query.addBindValue(readerData["gender"]);
    query.addBindValue(readerData["phone"]);
    query.addBindValue(readerData["email"]);
    query.addBindValue(readerData["address"]);
    query.addBindValue(readerData["register_date"]);

    return query.exec();
}


bool DatabaseManager::updateReader(int id, const QVariantMap &readerData)
{
    QSqlQuery query(m_database);
    query.prepare(R"(
        UPDATE readers SET reader_id=?, name=?, gender=?, phone=?, email=?, address=?
        WHERE id=?
    )");

    query.addBindValue(readerData["reader_id"]);
    query.addBindValue(readerData["name"]);
    query.addBindValue(readerData["gender"]);
    query.addBindValue(readerData["phone"]);
    query.addBindValue(readerData["email"]);
    query.addBindValue(readerData["address"]);
    query.addBindValue(id);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "更新读者失败:" << query.lastError();
        return false;
    }
}

bool DatabaseManager::deleteReader(int id)
{
    QSqlQuery query(m_database);
    query.prepare("DELETE FROM readers WHERE id = ?");
    query.addBindValue(id);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "删除读者失败:" << query.lastError();
        return false;
    }
}

QVector<QVariantMap> DatabaseManager::getAllReaders()
{
    QVector<QVariantMap> readers;
    QSqlQuery query("SELECT * FROM readers ORDER BY id DESC", m_database);

    while (query.next()) {
        QVariantMap reader;
        QSqlRecord record = query.record();
        for (int i = 0; i < record.count(); i++) {
            reader[record.fieldName(i)] = query.value(i);
        }
        readers.append(reader);
    }

    return readers;
}

QVector<QVariantMap> DatabaseManager::searchReaders(const QString &keyword)
{
    QVector<QVariantMap> readers;
    QSqlQuery query(m_database);
    query.prepare(R"(
        SELECT * FROM readers
        WHERE name LIKE ? OR reader_id LIKE ? OR phone LIKE ? OR email LIKE ?
        ORDER BY id DESC
    )");

    QString searchPattern = "%" + keyword + "%";
    query.addBindValue(searchPattern);
    query.addBindValue(searchPattern);
    query.addBindValue(searchPattern);
    query.addBindValue(searchPattern);

    if (query.exec()) {
        while (query.next()) {
            QVariantMap reader;
            QSqlRecord record = query.record();
            for (int i = 0; i < record.count(); i++) {
                reader[record.fieldName(i)] = query.value(i);
            }
            readers.append(reader);
        }
    }

    return readers;
}

// 借阅管理相关方法实现
bool DatabaseManager::borrowBook(int bookId, int readerId, const QDate &dueDate)
{
    QSqlQuery query(m_database);

    // 检查图书是否可借
    query.prepare("SELECT available_count FROM books WHERE id = ?");
    query.addBindValue(bookId);

    if (!query.exec() || !query.next()) {
        return false;
    }

    int availableCount = query.value(0).toInt();
    if (availableCount <= 0) {
        return false;
    }

    // 减少可借数量
    query.prepare("UPDATE books SET available_count = available_count - 1 WHERE id = ?");
    query.addBindValue(bookId);
    if (!query.exec()) {
        return false;
    }

    // 创建借阅记录
    query.prepare(R"(
        INSERT INTO borrow_records (book_id, reader_id, borrow_date, due_date, status)
        VALUES (?, ?, ?, ?, ?)
    )");

    query.addBindValue(bookId);
    query.addBindValue(readerId);
    query.addBindValue(QDate::currentDate());
    query.addBindValue(dueDate);
    query.addBindValue("借出");

    return query.exec();
}

bool DatabaseManager::returnBook(int recordId)
{
    QSqlQuery query(m_database);

    // 首先获取借阅记录的图书ID
    query.prepare("SELECT book_id FROM borrow_records WHERE id = ? AND status = '借出'");
    query.addBindValue(recordId);

    if (!query.exec() || !query.next()) {
        qDebug() << "找不到可归还的借阅记录或记录状态错误:" << recordId;
        return false;
    }

    int bookId = query.value(0).toInt();

    // 开启事务
    m_database.transaction();

    try {
        // 1. 更新借阅记录状态
        query.prepare("UPDATE borrow_records SET return_date = ?, status = '已还' WHERE id = ?");
        query.addBindValue(QDate::currentDate());
        query.addBindValue(recordId);

        if (!query.exec()) {
            throw std::runtime_error("更新借阅记录失败");
        }

        // 2. 恢复图书的可借数量
        query.prepare("UPDATE books SET available_count = available_count + 1 WHERE id = ?");
        query.addBindValue(bookId);

        if (!query.exec()) {
            throw std::runtime_error("更新图书可借数量失败");
        }

        // 提交事务
        m_database.commit();
        qDebug() << "还书成功，记录ID:" << recordId << "，图书ID:" << bookId;
        return true;

    } catch (const std::exception &e) {
        // 回滚事务
        m_database.rollback();
        qDebug() << "还书失败:" << e.what();
        return false;
    }
}

QVector<QVariantMap> DatabaseManager::getAllBorrowRecords()
{
    QVector<QVariantMap> records;
    QSqlQuery query(m_database);

    // 关键查询：连接三张表，获取完整的借阅信息
    QString sql = R"(
        SELECT
            br.id,
            br.borrow_date,
            br.due_date,
            br.return_date,
            br.status,
            b.id as book_id,
            b.title as book_title,
            b.isbn as book_isbn,
            r.id as reader_id,
            r.name as reader_name,
            r.reader_id as reader_number
        FROM borrow_records br
        LEFT JOIN books b ON br.book_id = b.id
        LEFT JOIN readers r ON br.reader_id = r.id
        ORDER BY br.borrow_date DESC, br.id DESC
    )";

    if (!query.exec(sql)) {
        qDebug() << "获取借阅记录失败:" << query.lastError();
        return records;
    }

    while (query.next()) {
        QVariantMap record;
        QSqlRecord rec = query.record();
        for (int i = 0; i < rec.count(); ++i) {
            record[rec.fieldName(i)] = query.value(i);
        }

        // 计算并添加一个“逾期天数”字段（如果逾期）
        if (record["status"].toString() == "借出") {
            QDate dueDate = query.value("due_date").toDate();
            QDate currentDate = QDate::currentDate();
            if (dueDate < currentDate) {
                int overdueDays = dueDate.daysTo(currentDate);
                record["overdue_days"] = overdueDays;
            } else {
                record["overdue_days"] = 0;
            }
        } else {
            record["overdue_days"] = 0;
        }

        records.append(record);
    }

    return records;
}

QVector<QVariantMap> DatabaseManager::getBorrowRecordsByReader(int readerId)
{
    QVector<QVariantMap> records;
    QSqlQuery query(m_database);
    query.prepare(R"(
        SELECT br.*, b.title, b.isbn
        FROM borrow_records br
        LEFT JOIN books b ON br.book_id = b.id
        WHERE br.reader_id = ?
        ORDER BY br.borrow_date DESC
    )");
    query.addBindValue(readerId);

    if (query.exec()) {
        while (query.next()) {
            QVariantMap record;
            QSqlRecord sqlRecord = query.record();
            for (int i = 0; i < sqlRecord.count(); i++) {
                record[sqlRecord.fieldName(i)] = query.value(i);
            }
            records.append(record);
        }
    }

    return records;
}

QVector<QVariantMap> DatabaseManager::getOverdueBooks()
{
    QVector<QVariantMap> records;
    QSqlQuery query(m_database);
    query.prepare(R"(
        SELECT br.*, b.title, b.isbn, r.name as reader_name, r.reader_id
        FROM borrow_records br
        LEFT JOIN books b ON br.book_id = b.id
        LEFT JOIN readers r ON br.reader_id = r.id
        WHERE br.status = '借出' AND br.due_date < ?
        ORDER BY br.due_date
    )");
    query.addBindValue(QDate::currentDate());

    if (query.exec()) {
        while (query.next()) {
            QVariantMap record;
            QSqlRecord sqlRecord = query.record();
            for (int i = 0; i < sqlRecord.count(); i++) {
                record[sqlRecord.fieldName(i)] = query.value(i);
            }
            records.append(record);
        }
    }

    return records;
}

// 获取可借的图书（available_count > 0）
QVector<QVariantMap> DatabaseManager::getAvailableBooks()
{
    QVector<QVariantMap> books;
    QSqlQuery query("SELECT * FROM books WHERE available_count > 0 ORDER BY title", m_database);

    while (query.next()) {
        QVariantMap book;
        QSqlRecord record = query.record();
        for (int i = 0; i < record.count(); i++) {
            book[record.fieldName(i)] = query.value(i);
        }
        books.append(book);
    }
    return books;
}

// 检查读者是否存在
bool DatabaseManager::readerExists(int readerId)
{
    QSqlQuery query(m_database);
    query.prepare("SELECT COUNT(*) FROM readers WHERE id = ?");
    query.addBindValue(readerId);
    return query.exec() && query.next() && query.value(0).toInt() > 0;
}

// 检查图书是否可借
bool DatabaseManager::isBookAvailable(int bookId)
{
    QSqlQuery query(m_database);
    query.prepare("SELECT available_count FROM books WHERE id = ?");
    query.addBindValue(bookId);
    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

// 统计数据
int DatabaseManager::getBookCount()
{
    QSqlQuery query("SELECT COUNT(*) FROM books", m_database);
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

int DatabaseManager::getReaderCount()
{
    QSqlQuery query("SELECT COUNT(*) FROM readers", m_database);
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

int DatabaseManager::getBorrowedCount()
{
    QSqlQuery query("SELECT COUNT(*) FROM borrow_records WHERE status = '借出'", m_database);
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}


QVector<QVariantMap> DatabaseManager::getBookBorrowRanking(int limit)
{
    QVector<QVariantMap> ranking;
    QSqlQuery query(m_database);

    QString sql = R"(
        SELECT
            b.id,
            b.title,
            b.author,
            b.isbn,
            COUNT(br.id) as borrow_count,
            b.available_count
        FROM books b
        LEFT JOIN borrow_records br ON b.id = br.book_id
        GROUP BY b.id, b.title, b.author, b.isbn
        ORDER BY borrow_count DESC, b.title
        LIMIT ?
    )";

    query.prepare(sql);
    query.addBindValue(limit);

    if (query.exec()) {
        while (query.next()) {
            QVariantMap bookStat;
            bookStat["id"] = query.value("id");
            bookStat["title"] = query.value("title");
            bookStat["author"] = query.value("author");
            bookStat["isbn"] = query.value("isbn");
            bookStat["borrow_count"] = query.value("borrow_count");
            bookStat["available_count"] = query.value("available_count");
            ranking.append(bookStat);
        }
    } else {
        qDebug() << "获取图书借阅排行失败:" << query.lastError();
    }

    return ranking;
}

QVector<QVariantMap> DatabaseManager::getReaderBorrowRanking(int limit)
{
    QVector<QVariantMap> ranking;
    QSqlQuery query(m_database);

    QString sql = R"(
        SELECT
            r.id,
            r.reader_id,
            r.name,
            r.gender,
            COUNT(br.id) as borrow_count,
            COUNT(CASE WHEN br.status = '借出' THEN 1 END) as active_count
        FROM readers r
        LEFT JOIN borrow_records br ON r.id = br.reader_id
        GROUP BY r.id, r.reader_id, r.name, r.gender
        ORDER BY borrow_count DESC, r.name
        LIMIT ?
    )";

    query.prepare(sql);
    query.addBindValue(limit);

    if (query.exec()) {
        while (query.next()) {
            QVariantMap readerStat;
            readerStat["id"] = query.value("id");
            readerStat["reader_id"] = query.value("reader_id");
            readerStat["name"] = query.value("name");
            readerStat["gender"] = query.value("gender");
            readerStat["borrow_count"] = query.value("borrow_count");
            readerStat["active_count"] = query.value("active_count");
            ranking.append(readerStat);
        }
    } else {
        qDebug() << "获取读者借阅排行失败:" << query.lastError();
    }

    return ranking;
}
