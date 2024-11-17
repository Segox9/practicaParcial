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

/*void MainWindow::raices()
{
    QString funtion = ui->funtion->text().trimmed();
    qDebug() << "Funcion a buscar raices " << funtion;
    double xlr = ui->xleft->text().toDouble();
    double xrr = ui->xright->text().toDouble();
    double tolerance = 1e-6;
    double x;
    parser.DefineVar(L"x", &x);
    if((funtion.compare("f(x)", Qt::CaseInsensitive) == 0) ||
        (funtion.compare("f", Qt::CaseInsensitive) == 0)){
        parser.SetExpr(ui->fx->text().toStdWString());
        x = xlr;
        double yizq = parser.Eval();
        x = xrr;
        double yder = parser.Eval();
        if((yizq * yder) < 0){
            while((xrr - xlr) > tolerance){
                double xmed = (xrr + xlr)/2.0;
                x = xlr;
                yizq = parser.Eval();
                x = xmed;
                double ymed = parser.Eval();
                if((yizq * ymed) < 0){
                    xrr = xmed;
                } else{
                    xlr = xmed;
                }
            }
            x = (xrr + xlr)/2.0;
            double y = parser.Eval();
            marcarPuntos(x, y);
        } else{
            error("No se encontraron raices");
        }
    } else{
        error("Sintaxis de funcion mal ingresada");
        error("Ingrese f(x), g(x), f o g");
    }
    if((funtion.compare("g(x)", Qt::CaseInsensitive) == 0) ||
        (funtion.compare("g", Qt::CaseInsensitive) == 0)){
        parser.SetExpr(ui->gx->text().toStdWString());
        x = xlr;
        double yizq = parser.Eval();
        x = xrr;
        double yder = parser.Eval();
        if((yizq * yder) < 0){
            while((xrr - xlr) > tolerance){
                double xmed = (xrr + xlr)/2.0;
                x = xlr;
                yizq = parser.Eval();
                x = xmed;
                double ymed = parser.Eval();
                if((yizq * ymed) < 0){
                    xrr = xmed;
                } else{
                    xlr = xmed;
                }
            }
            x = (xrr + xlr)/2.0;
            double y = parser.Eval();
            marcarPuntos(x, y);
        } else{
            error("No se encontraron raices");
        }
    } else{
        error("Sintaxis de funcion mal ingresada");
        error("Ingrese f(x), g(x), f o g");
    }
}*/
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

}

void MainWindow::interseccion(double x1, double x2)
{

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


