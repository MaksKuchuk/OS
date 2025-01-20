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

            this->minn = std::stod(v[1]);
            this->maxx = std::stod(v[1]);
            QLineSeries* series = new QLineSeries();
            for (size_t i = 0; i < v.size(); i += 2) {
                series->append(std::stoll(v[i]), std::stod(v[i + 1]));

                if (std::stod(v[i + 1]) > maxx) {
                    this->maxx = std::stod(v[i + 1]);
                }
                if (std::stod(v[i + 1]) < minn) {
                    this->minn = std::stod(v[i + 1]);
                }

                QDateTime ts;
                if (t == 'c') {
                    ts.setTime_t(std::stoll(v[i]));
                    secTable->setItem(i/2, 0, new QTableWidgetItem(ts.toString(Qt::SystemLocaleShortDate)));
                    secTable->setItem(i/2, 1, new QTableWidgetItem(QString::fromStdString(v[i + 1])));
                } else if (t == 'r') {
                    hourTable->setItem(i/2, 0, new QTableWidgetItem(ts.toString(Qt::SystemLocaleShortDate)));
                    hourTable->setItem(i/2, 1, new QTableWidgetItem(QString::fromStdString(v[i + 1])));
                } else if (t == 'y') {
                    dayTable->setItem(i/2, 0, new QTableWidgetItem(ts.toString(Qt::SystemLocaleShortDate)));
                    dayTable->setItem(i/2, 1, new QTableWidgetItem(QString::fromStdString(v[i + 1])));
                }
            }

            // qDebug() << minn << " " << maxx;

            // QDateTimeAxis *axisX = new QDateTimeAxis;
            // axisX->setTickCount(4);
            // axisX->setFormat("hh:mm:ss");
            // axisX->setTitleText("Time");

            // QValueAxis *axisY = new QValueAxis;
            // axisY->setLabelFormat("%.2f");
            // axisY->setTitleText("Temperature");

            QDateTime tsmin, tsmax;
            if (t == 'c') {
                secChart->chart()->removeAllSeries();
                // secChart->chart()->removeAxis(secChart->chart()->axisX());
                // secChart->chart()->removeAxis(secChart->chart()->axisY());
                secData = series;
                secChart->chart()->addSeries(secData);
                secChart->chart()->axisY()->setRange(minn, maxx);

                tsmin.setTime_t(std::stoll(v[0]));
                tsmax.setTime_t(std::stoll(v[v.size() - 2]));
                secChart->chart()->axisX()->setRange(tsmin, tsmax);

                // secChart->chart()->addAxis(axisX, Qt::AlignBottom);
                secData->attachAxis(secChart->chart()->axisX());
                // secChart->chart()->addAxis(axisY, Qt::AlignLeft);
                secData->attachAxis(secChart->chart()->axisY());
                secChart->repaint();
            } else if (t == 'r') {
                hourChart->chart()->removeAllSeries();
                hourData = series;
                hourChart->chart()->addSeries(hourData);
                hourChart->chart()->axisY()->setRange(minn, maxx);

                tsmin.setTime_t(std::stoll(v[0]));
                tsmax.setTime_t(std::stoll(v[v.size() - 2]));
                hourChart->chart()->axisX()->setRange(tsmin, tsmax);

                // secChart->chart()->addAxis(axisX, Qt::AlignBottom);
                hourData->attachAxis(hourChart->chart()->axisX());
                // secChart->chart()->addAxis(axisY, Qt::AlignLeft);
                hourData->attachAxis(hourChart->chart()->axisY());
                hourChart->repaint();
            } else if (t == 'y') {
                dayChart->chart()->removeAllSeries();
                dayData = series;
                dayChart->chart()->addSeries(dayData);
                dayChart->chart()->axisY()->setRange(minn, maxx);

                tsmin.setTime_t(std::stoll(v[0]));
                tsmax.setTime_t(std::stoll(v[v.size() - 2]));
                dayChart->chart()->axisX()->setRange(tsmin, tsmax);

                // secChart->chart()->addAxis(axisX, Qt::AlignBottom);
                dayData->attachAxis(dayChart->chart()->axisX());
                // secChart->chart()->addAxis(axisY, Qt::AlignLeft);
                dayData->attachAxis(dayChart->chart()->axisY());
                dayChart->repaint();
            }
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
    axisX->setTickCount(4);
    axisX->setFormat("hh:mm:ss");
    axisX->setTitleText("Time");
    chart->addAxis(axisX, Qt::AlignBottom);
    secData->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%.5f");
    axisY->setTitleText("Temperature");
    // axisY->setRange(se, 17);
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
    chart->setTitle("Temperature every hour");
    axisX = new QDateTimeAxis;
    axisX->setTickCount(4);
    axisX->setFormat("dd.mm.yy hh");
    axisX->setTitleText("Time");
    chart->addAxis(axisX, Qt::AlignBottom);
    hourData->attachAxis(axisX);

    axisY = new QValueAxis;
    axisY->setLabelFormat("%.5f");
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
    chart->setTitle("Temperature every day");
    axisX = new QDateTimeAxis;
    axisX->setTickCount(4);
    axisX->setFormat("dd.mm.yy");
    axisX->setTitleText("Time");
    chart->addAxis(axisX, Qt::AlignBottom);
    dayData->attachAxis(axisX);

    axisY = new QValueAxis;
    axisY->setLabelFormat("%.5f");
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

    this->switchTableGraph();
}

void MainWindow::switchTableGraph() {
    request.setUrl(QUrl(QString::fromStdString(std::string("http://") + IP + ":" + IP_PORT + "/sec")));
    manager->get(request);

    request.setUrl(QUrl(QString::fromStdString(std::string("http://") + IP + ":" + IP_PORT + "/hour")));
    manager->get(request);

    request.setUrl(QUrl(QString::fromStdString(std::string("http://") + IP + ":" + IP_PORT + "/day")));
    manager->get(request);
}