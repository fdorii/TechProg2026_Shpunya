#ifndef STATISTICWINDOW_H
#define STATISTICWINDOW_H

#include <QWidget>
#include <QJsonObject>

namespace Ui {
class StatisticForm;
}

class StatisticForm : public QWidget
{
    Q_OBJECT
public:
    explicit StatisticForm(const QString &login, QWidget *parent = nullptr);
    ~StatisticForm();

    void setStatistics(const QJsonObject &stats);

private:
    Ui::StatisticForm *ui;
    QString m_login;
};

#endif
