#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //------------------------------------
    connect(ui->plot, SIGNAL(clicked()), this, SLOT(plot()));
    connect(ui->froot, SIGNAL(clicked()), this, SLOT(rootFx()));
    connect(ui->groot, SIGNAL(clicked()), this, SLOT(rootGx()));
    connect(ui->refresh, SIGNAL(clicked()), this, SLOT(refresh()));
    connect(ui->area, SIGNAL(clicked()), this, SLOT(area()));
    connect(ui->interseccion, SIGNAL(clicked()), this, SLOT(interseccion()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::plot()
{
    //Defino la funcion
    double xi = ui->xi->text().toDouble();
    double xf = ui->xf->text().toDouble();
    double dx = 1e-4;
    int n = (xf - xi)/dx;
    qDebug() << "Tamaño " << n;
    x.resize(n);
    f.resize(n);
    g.resize(n);
    parser.DefineVar(L"x", x.data());
    for(int i=0; i<n; i++)
        x[i] = xi + double(i)*dx;

    //Calculo las funciones
    parser.SetExpr(ui->fx->text().toStdWString());
    parser.Eval(f.data(), f.size());
    parser.SetExpr(ui->gx->text().toStdWString());
    parser.Eval(g.data(), g.size());

    if(ui->widget->graphCount() == 0){
        ui->widget->addGraph();
        ui->widget->graph(0)->setData(x, f);
        ui->widget->graph(0)->setPen(QPen(Qt::blue, 2));
        ui->widget->addGraph();
        ui->widget->graph(1)->setData(x, g);
        ui->widget->graph(1)->setPen(QPen(Qt::red, 2));
        ui->widget->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);
        ui->widget->rescaleAxes();
        ui->widget->rescaleAxes();
        ui->widget->replot();
    }
}

void MainWindow::marcarPuntos(double xroot, double yroot){
    if(ui->widget->graphCount() <= 2){
        ui->widget->addGraph();
        ui->widget->graph(2);
        ui->widget->graph(2)->setLineStyle(QCPGraph::lsNone);
        ui->widget->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::red), QBrush(Qt::red), 4));
    }
    QVector<double> x(1), y(1);
    x[0] = xroot;
    y[0] = yroot;
    ui->widget->graph(2)->setData(x, y);
    ui->widget->replot();
}
void MainWindow::rootFx(){
    double xr = ui->xright->text().toDouble();
    double xl = ui->xleft->text().toDouble();
    double x;
    double tolerance = 1e-8;
    parser.DefineVar(L"x", &x);
    parser.SetExpr(ui->fx->text().toStdWString());
    x = xl;
    double yl = parser.Eval();
    x = xr;
    double yr = parser.Eval();
    if((yl * yr) < 0){
        while((xr - xl) > tolerance){
            double xmed = (xr + xl)/2.0;
            x = xl;
            yl = parser.Eval();
            x = xmed;
            double ymed = parser.Eval();
            if((ymed * yl) < 0){
                xr = xmed;
            } else{
                xl = xmed;
            }
        }
        x = (xr + xl)/2.0;
        qDebug() << "Raiz de f(x): " << x;
        double y = parser.Eval();
        qDebug() << "f(x): " << y;
        marcarPuntos(x, y);
    } else{
        error("No se encontraron raices en el intervalo dado");
    }
}
void MainWindow::rootGx(){
    double xr = ui->xright->text().toDouble();
    double xl = ui->xleft->text().toDouble();
    double x;
    double tolerance = 1e-8;
    parser.DefineVar(L"x", &x);
    parser.SetExpr(ui->gx->text().toStdWString());
    x = xl;
    double yl = parser.Eval();
    x = xr;
    double yr = parser.Eval();
    if((yl * yr) < 0){
        while((xr - xl) > tolerance){
            double xmed = (xr + xl)/2.0;
            x = xl;
            yl = parser.Eval();
            x = xmed;
            double ymed = parser.Eval();
            if((ymed * yl) < 0){
                xr = xmed;
            } else{
                xl = xmed;
            }
        }
        x = (xr + xl)/2.0;
        qDebug() << "Raiz de f(x): " << x;
        double y = parser.Eval();
        qDebug() << "f(x): " << y;
        marcarPuntos(x, y);
    } else{
        error("No se encontraron raices en el intervalo dado");
    }
}
void MainWindow::area()
{
    int xi, xf;
    Busqueda(xi, xf);
    //pregunto si las funciones se intersectan (caso particular), sino se calcula el area como esta abajo
    double areaFx, areaGx;
    areaFx = areaTrapecios(xi, xf, f);
    areaGx = areaTrapecios(xi, xf, g);
    qDebug() << "Area de f(x): " << areaFx;
    qDebug() << "Area de g(x): " << areaGx;
    double areaTot;
    if(areaFx > areaGx){
        areaTot = areaFx - areaGx;
    } else{
        areaTot = areaGx - areaFx;
    }
    if((areaFx < 0) && (areaGx < 0)){
        areaTot = areaFx + areaGx;
    }
    qDebug() << "Area total: " << areaTot;
}
void MainWindow::Busqueda(int &liminf, int &limsup){
    double xla = ui->xia->text().toDouble();
    double xra = ui->xfa->text().toDouble();
    double dx = 1e-4;
    liminf = (xla - x[0])/dx;
    limsup = (xra - x[0])/dx;
    qDebug() << "xi: " << liminf;
    qDebug() << "xf: " << limsup;
}

double MainWindow::areaTrapecios(int x1, int x2, QVector<double> fun)
{
    double area = 0, h1=0, h2=0;
    for(int i=x1; i<x2; i++){
        h1 = fun[i+1] - fun[i];
        h2 = fun[i];
        double delta_x = x[i + 1] - x[i];
        area += delta_x*((h1/2) + h2);
    }
    return area;
}

double MainWindow::areaRectangulo(int x1, int x2, QVector<double> fun)
{
    double area = 0, base, alt;
    for(int i=x1; i<x2; i++){
        alt = fun[i+1] - fun[i];
        base = x[i+1] - x[i];
        area += alt*base;
    }
    return area;
}
QVector<double> MainWindow::buscarIntersecciones(const QVector<double> &x, const QVector<double> &fx, const QVector<double> &gx){
    QVector<double> intersectionsX; // Para guardar los valores de x de los puntos de intersección
    QVector<double> intersectionsY; // Para guardar los valores de y en los puntos de intersección

    for (int i = 0; i < x.size() - 1; i++) {
        double y1 = fx[i] - gx[i];     // Diferencia en el punto actual
        double y2 = fx[i+1] - gx[i+1]; // Diferencia en el siguiente punto

        // Si hay cambio de signo
        if (((y1 > 0) && (y2 < 0)) || ((y1 < 0) && (y2 > 0))) {
            // Interpolación lineal para aproximar x en la intersección
            double xIntersect = x[i] - (y1 * (x[i+1] - x[i]) / (y2 - y1));
            double yIntersect = fx[i] + ((fx[i+1] - fx[i]) * (xIntersect - x[i]) / (x[i+1] - x[i]));

            intersectionsX.append(xIntersect); // Agregar x de intersección
            intersectionsY.append(yIntersect); // Agregar y de intersección
        }
    }

    // Opcional: Imprimir puntos de intersección
    for (int i = 0; i < intersectionsX.size(); i++) {
        qDebug() << "Intersección en x =" << intersectionsX[i] << ", y =" << intersectionsY[i];
    }

    return intersectionsX; // Devuelve los valores de x donde ocurre la intersección
}
void MainWindow::interseccion()
{
    QVector<double> interX = buscarIntersecciones(x, f, g);

}


double MainWindow::newton_rapshon(double x0)
{

}
void MainWindow::refresh(){
    ui->widget->clearGraphs();
    ui->widget->replot();
}
void MainWindow::error(const QString &mensaje)
{
    QMessageBox::critical(this, "Error", mensaje);
}


