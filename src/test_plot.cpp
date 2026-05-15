#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QApplication>

int main() {
  int argc{0};
  QApplication app(argc, nullptr);

  QLineSeries *series = new QLineSeries();
  series->setName("Line");
  series->append(0, 0);
  series->append(1.1, 2.1);
  series->append(1.9, 3.3);
  series->append(2.1, 2.1);
  series->append(2.9, 4.9);
  series->append(3.4, 3.0);
  series->append(4.1, 3.3);

  QChart *chart = new QChart();
  chart->addSeries(series);
  chart->setTitle("Line Chart");
  chart->createDefaultAxes();

  QChartView chartView(chart);
  chartView.resize(600, 400);
  chartView.show();

  return app.exec();
}
