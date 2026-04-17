#ifndef STATISTICWINDOW_H
#define STATISTICWINDOW_H

#include <QWidget>

namespace Ui {
class StatisticForm;
}

class StatisticForm : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticForm(QWidget *parent = nullptr);
    ~StatisticForm();

private:
    Ui::StatisticForm *ui;
};

#endif // STATISTICWINDOW_H
