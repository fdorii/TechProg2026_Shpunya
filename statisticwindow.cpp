#include "statisticwindow.h"
#include "ui_statisticwindow.h"

StatisticForm::StatisticForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticForm)
{
    ui->setupUi(this);
}

StatisticForm::~StatisticForm()
{
    delete ui;
}
