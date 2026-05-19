#include "stochastic/chart.hpp"

namespace stochastic {

int Series::PopulateSeries(QLineSeries *series,
                           const std::vector<double> &timestamps) const {
  series->setName(name.c_str());
  auto max = 0;
  for (size_t i = 0; i < values.size(); i++) {
    series->append(timestamps[i], values[i]);
    if (values[i] > max)
      max = values[i];
  }
  return max;
}

int Chart::ViewChart(double end_time) const {
  int argc{0};
  QApplication app(argc, nullptr);
  QChart *chart = new QChart();
  chart->setTitle(name.c_str());

  auto max = 0;
  for (const auto &s : series) {
    QLineSeries *series = new QLineSeries();
    auto y_max = s.PopulateSeries(series, timestamps);

    if (y_max > max)
      max = y_max;

    chart->addSeries(series);
  }

  chart->createDefaultAxes();
  chart->axes(Qt::Horizontal).back()->setRange(0, end_time);
  chart->axes(Qt::Vertical).back()->setRange(0, max);
  chart->axes(Qt::Horizontal).back()->setTitleText("time");
  chart->axes(Qt::Vertical).back()->setTitleText("quantity");

  QChartView chartView(chart);
  chartView.resize(600, 400);
  chartView.show();
  return app.exec();
};
} // namespace stochastic
