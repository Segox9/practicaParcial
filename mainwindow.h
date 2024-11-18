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
    double newton_rapshon(double x0);
    void error(const QString &mensaje);
    void marcarPuntos(double xroot, double yroot);
    void Busqueda(int &liminf, int &limsup);
    double areaTrapecios(int x1, int x2, QVector<double> fun);
    double areaRectangulo(int x1, int x2, QVector<double> fun);
    QVector<double> buscarIntersecciones(const QVector<double> &x, const QVector<double> &fx, const QVector<double> &gx);
public slots:
    void plot();
    void refresh();
    void area();
    void rootFx();
    void rootGx();
    void interseccion();
};
#endif // MAINWINDOW_H
