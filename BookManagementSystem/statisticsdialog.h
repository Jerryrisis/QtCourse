#ifndef STATISTICSDIALOG_H
#define STATISTICSDIALOG_H

#include <QDialog>

namespace Ui {
class StatisticsDialog;
}

class StatisticsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StatisticsDialog(QWidget *parent = nullptr);
    ~StatisticsDialog();

private slots:
    void onRefreshClicked();
    void loadBookRanking();
    void loadReaderRanking();
    void updateSummary();
    void onExportClicked();

private:
    Ui::StatisticsDialog *ui;
};

#endif // STATISTICSDIALOG_H
