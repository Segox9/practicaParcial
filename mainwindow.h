#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "muParser.h"
#include <QMessageBox>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
//----------------------------------------
private:
    mu::Parser parser;
    QVector<double> x;
    QVector<double> f;
    QVector<double> g;
    void interseccion(double x1, double x2);
    double newton_rapshon(double x0);
    void error(const QString &mensaje);
    void marcarPuntos(double xroot, double yroot);
public slots:
    void plot();
    void refresh();
    void area();
    void rootFx();
    void rootGx();
};
#endif // MAINWINDOW_H
