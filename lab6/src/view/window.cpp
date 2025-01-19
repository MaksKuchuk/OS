#include "window.hpp"

#include <iostream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QSizePolicy>
#include <QStringList>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <format>
#include <string>
#include <QNetworkReply>
#include "../definitions.hpp"
#include "../utils.hpp"

MainWindow::MainWindow(QWidget* parent):QMainWindow(parent), manager(new QNetworkAccessManager()) {
    auto centralWidget = new QWidget(this);
    auto maincont = new QVBoxLayout();
    auto mainLayout = new QHBoxLayout();
    centralWidget->setLayout(maincont);
    this->setCentralWidget(centralWidget);

    maincont->addLayout(mainLayout);

    secLayout = new QVBoxLayout();
    hourLayout = new QVBoxLayout();
    dayLayout = new QVBoxLayout();

    secData = new QLineSeries();
    hourData = new QLineSeries();
    dayData = new QLineSeries();

    mainLayout->addLayout(secLayout);
    mainLayout->addLayout(hourLayout);
    mainLayout->addLayout(dayLayout);


    btn = new QPushButton("Switch chart", centralWidget);
    maincont->addWidget(btn);

    QStringList tHeaders = {"Time", "Temperature"};

    //----------------------------------------------------- Network
    manager = new QNetworkAccessManager();
    QObject::connect(manager, &QNetworkAccessManager::finished,
        this, [=](QNetworkReply *reply) {
            if (reply->error()) {
                qDebug() << reply->errorString();
                return;
            }
            std::string u = reply->url().url().toStdString();
            char t = u[u.size() - 1];

            QString answer = reply->readAll();
            if (answer.isEmpty()) return;
            auto v = split(answer.toStdString(), std::string(" "));

            if (t == 'c') {
                secTable->setRowCount(v.size() / 2);
            } else if (t == 'r') {
                hourTable->setRowCount(v.size() / 2);
            } else if (t == 'y') {
                dayTable->setRowCount(v.size() / 2);
            }

            QLineSeries* series = new QLineSeries();
            for (size_t i = 0; i < v.size(); i += 2) {
                series->append(std::stoll(v[i]), std::stod(v[i + 1]));

                if (t == 'c') {
                    secTable->setItem(i/2, 0, new QTableWidgetItem(QString::fromStdString(v[i])));
                    secTable->setItem(i/2, 1, new QTableWidgetItem(QString::fromStdString(v[i + 1])));
                } else if (t == 'r') {
                    hourChart->chart()->removeSeries(hourData);
                    hourData = series;
                    hourChart->chart()->addSeries(hourData);
                    hourChart->repaint();
                } else if (t == 'y') {
                    dayChart->chart()->removeSeries(dayData);
                    dayData = series;
                    dayChart->chart()->addSeries(dayData);
                    dayChart->repaint();
                }
            }

            if (t == 'c') {
                secChart->chart()->removeSeries(secData);
                secData = series;
                secChart->chart()->addSeries(secData);
                secChart->repaint();
            } else if (t == 'r') {
                hourChart->chart()->removeSeries(hourData);
                hourData = series;
                hourChart->chart()->addSeries(hourData);
                hourChart->repaint();
            } else if (t == 'y') {
                dayChart->chart()->removeSeries(dayData);
                dayData = series;
                dayChart->chart()->addSeries(dayData);
                dayChart->repaint();
            }

            qDebug() << answer;
        }
    );

    //----------------------------------------------------- Seconds
    auto titleLabel = new QLabel(centralWidget);
    titleLabel->setText("Seconds");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font: 18pt;");
    titleLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

    secTable = new QTableWidget(100, 2, this);
    secTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
    secTable->setHorizontalHeaderLabels(tHeaders);

    secTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    secTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    secTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    
    QChart *chart = new QChart();
    chart->addSeries(secData);
    chart->legend()->hide();
    chart->setTitle("Temperature every second");
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("MMM yyyy");
    axisX->setTitleText("Time");
    chart->addAxis(axisX, Qt::AlignBottom);
    secData->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Temperature");
    chart->addAxis(axisY, Qt::AlignLeft);
    secData->attachAxis(axisY);

    secChart = new QChartView(chart);
    secChart->setRenderHint(QPainter::Antialiasing);
    secChart->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    secLayout->addWidget(titleLabel, Qt::AlignCenter);
    secLayout->addWidget(secTable);
    //----------------------------------------------------- Seconds

    //----------------------------------------------------- Hours
    titleLabel = new QLabel(centralWidget);
    titleLabel->setText("Hours");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font: 18pt;");
    titleLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

    hourTable = new QTableWidget(100, 2, this);
    hourTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
    hourTable->setHorizontalHeaderLabels(tHeaders);

    hourTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    hourTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    hourTable->setEditTriggers(QAbstractItemView::NoEditTriggers);


    chart = new QChart();
    chart->addSeries(hourData);
    chart->legend()->hide();
    chart->setTitle("Temperature every second");
    axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("MMM yyyy");
    axisX->setTitleText("Time");
    chart->addAxis(axisX, Qt::AlignBottom);
    hourData->attachAxis(axisX);

    axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Temperature");
    chart->addAxis(axisY, Qt::AlignLeft);
    hourData->attachAxis(axisY);

    hourChart = new QChartView(chart);
    hourChart->setRenderHint(QPainter::Antialiasing);
    hourChart->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    hourLayout->addWidget(titleLabel);
    hourLayout->addWidget(hourTable);
    //----------------------------------------------------- Hours

    //----------------------------------------------------- Days
    titleLabel = new QLabel(centralWidget);
    titleLabel->setText("Days");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font: 18pt;");
    titleLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

    dayTable = new QTableWidget(100, 2, this);
    dayTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
    dayTable->setHorizontalHeaderLabels(tHeaders);

    dayTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    dayTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    dayTable->setEditTriggers(QAbstractItemView::NoEditTriggers);


    chart = new QChart();
    chart->addSeries(dayData);
    chart->legend()->hide();
    chart->setTitle("Temperature every second");
    axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("MMM yyyy");
    axisX->setTitleText("Time");
    chart->addAxis(axisX, Qt::AlignBottom);
    dayData->attachAxis(axisX);

    axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Temperature");
    chart->addAxis(axisY, Qt::AlignLeft);
    dayData->attachAxis(axisY);

    dayChart = new QChartView(chart);
    dayChart->setRenderHint(QPainter::Antialiasing);
    dayChart->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    dayLayout->addWidget(titleLabel);
    dayLayout->addWidget(dayTable);
    //----------------------------------------------------- Days

    QObject::connect(this->btn, &QPushButton::clicked, this, [this](){
        this->switchTableGraph();
        if (this->state) {
            secLayout->replaceWidget(secChart, secTable);
            hourLayout->replaceWidget(hourChart, hourTable);
            dayLayout->replaceWidget(dayChart, dayTable);
            secChart->setVisible(false);
            secTable->setVisible(true);
            hourChart->setVisible(false);
            hourTable->setVisible(true);
            dayChart->setVisible(false);
            dayTable->setVisible(true);
        } else {
            secLayout->replaceWidget(secTable, secChart);
            hourLayout->replaceWidget(hourTable, hourChart);
            dayLayout->replaceWidget(dayTable, dayChart);
            secChart->setVisible(true);
            secTable->setVisible(false);
            hourChart->setVisible(true);
            hourTable->setVisible(false);
            dayChart->setVisible(true);
            dayTable->setVisible(false);
        }
        this->state = !this->state;
    });
}

void MainWindow::switchTableGraph() {
    request.setUrl(QUrl(QString::fromStdString(std::string("http://") + IP + ":" + IP_PORT + "/sec")));
    manager->get(request);

    request.setUrl(QUrl(QString::fromStdString(std::string("http://") + IP + ":" + IP_PORT + "/hour")));
    manager->get(request);

    request.setUrl(QUrl(QString::fromStdString(std::string("http://") + IP + ":" + IP_PORT + "/day")));
    manager->get(request);
}