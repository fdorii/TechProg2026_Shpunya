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

private slots:
    void onServerMessage(const QString &msg);

private:
    Ui::StatisticForm *ui;
    QString m_currentLogin;
    bool m_statsReceived = false;

    void requestStats();
};

#endif // STATISTICWINDOW_H
