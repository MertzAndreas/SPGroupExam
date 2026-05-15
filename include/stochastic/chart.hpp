#pragma once

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <utility>
#include <vector>

namespace stochastic {
class Series {
  std::vector<std::pair<double, double>> points;
  std::string name;

public:
  Series(std::vector<std::pair<double, double>> points, std::string name)
      : points(std::move(points)), name(std::move(name)) {}

  int PopulateSeries(QLineSeries *series) const;
};

class Chart {
  std::vector<Series> series;
  std::string name;

public:
  Chart(std::vector<Series> series, std::string name)
      : series(std::move(series)), name(std::move(name)) {}

  int ViewChart(double end_time) const;
};
} // namespace stochastic
