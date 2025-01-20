#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QChart>
#include <QLineSeries>
#include <QVBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QChartView>

using namespace QtCharts;

class MainWindow : public QMainWindow {
    Q_OBJECT
    bool state = false;
    double minn;
    double maxx;

    QNetworkAccessManager* manager;
    QNetworkRequest request;

    QVBoxLayout* secLayout;
    QVBoxLayout* hourLayout;
    QVBoxLayout* dayLayout;

    QLineSeries* secData;
    QLineSeries* hourData;
    QLineSeries* dayData;

    QPushButton* btn;

    QTableWidget* secTable;
    QTableWidget* hourTable;
    QTableWidget* dayTable;

    QChartView* secChart;
    QChartView* hourChart;
    QChartView* dayChart;

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() = default;

    void switchTableGraph();
};