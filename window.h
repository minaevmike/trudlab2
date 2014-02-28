#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QRadioButton>
#include <iostream>
#include <iomanip>
#include <QPointF>
#include <QPolygonF>
#include <QLabel>

//-----------------------------------------------------------------
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>
//-----------------------------------------------------------------
#include <math.h>
class QPushButton;
//-----------------------------------------------------------------
class QwtPlot;
class QwtPlotCurve;
class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = 0);
    ~Window();
private:
    QwtPlot *funPlot;
    QwtPlotCurve *sinFunCurve, *sinDerCurve;
    QwtPlotCurve *cubFunCurve, *cubDerCurve, *aCurve, *rCurve;
    QPushButton *sinButton, *cubButton;
    QGroupBox *createTypeGroup();
    QGroupBox *createNumberGroup();
    QGroupBox *createNonExclusiveGroup();
    QGroupBox *createPushButtonGroup();
    QCheckBox *checkBoxMKE, *checkBoxAnalitic;
    QLabel *error;
    QPushButton *solveButton;
    void solve(int N, double a, double b, double du2Coef, double duCoef, double uCoef,double freeCoef, double duNach, double uNach, int method = 1);
    QRadioButton *el40, *el20, *linearForm, *quadraticForm;
    void print(QVector<QVector<double> > matrix);
    void print(QVector<double> v);
    QVector<double> solveProgon(QVector<QVector<double> > &matrix, QVector<double> v, int index);
    void fillLinear(QVector<QVector<double> > &matrix, double L, double du2Coef, double duCoef, double uCoef, double freeCoef);
    void fillQuadratic(QVector<QVector<double> > &matrix, double L, double du2Coef, double duCoef, double uCoef, double freeCoef);
    QVector<double> analiticSolve(int N, double a, double L);
    int lcm(int x, int y);

private slots:
    void calculate();
    void analiticCheck(int state);
    void realCheck(int state);
};

#endif // WINDOW_H
