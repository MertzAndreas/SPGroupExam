#pragma once

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <vector>

namespace stochastic {
class Series {
  const std::string name;
  const std::vector<double> values;

public:
  Series(std::string name, std::vector<double> values);

  int PopulateSeries(QLineSeries *series,
                     const std::vector<double> &timestamps) const;
};

class Chart {
  const std::vector<Series> series;
  const std::vector<double> timestamps;
  const std::string name;

public:
  Chart(std::vector<Series> series, std::vector<double> timestamps,
        std::string name);

  int ViewChart(double end_time) const;
};
} // namespace stochastic
