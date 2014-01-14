#include "window.h"

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    funPlot = new QwtPlot;
    funPlot->setTitle(QwtText("MKE"));
    funPlot->insertLegend(new QwtLegend());
    // Create curves and attach them to plots
    QPen rPen = QPen(Qt::red);
    QPen bPen = QPen(Qt::blue);
    rPen.setStyle(Qt::DashDotLine);
    bPen.setStyle(Qt::DashLine);

    aCurve = new QwtPlotCurve;
    aCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    aCurve->setPen(rPen);
    aCurve->setTitle("Analit");
    aCurve->attach(funPlot);

    rCurve = new QwtPlotCurve;
    rCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    rCurve->setPen(bPen);
    rCurve->setTitle("МКЭ");
    rCurve->attach(funPlot);

    // Set layouts
    QHBoxLayout *plotsLayout = new QHBoxLayout;
    plotsLayout->setSpacing(10);
    plotsLayout->addWidget(funPlot);
    //plotsLayout->addWidget(derPlot);

    QHBoxLayout *grid = new QHBoxLayout;
    //grid->addWidget(createTypeGroup());
    grid->addWidget(createNumberGroup());
    grid->addWidget(createNonExclusiveGroup());
    grid->addWidget(createPushButtonGroup());

    QVBoxLayout *widgetLayout = new QVBoxLayout;
    widgetLayout->addLayout(plotsLayout);
    widgetLayout->addLayout(grid);

    setLayout(widgetLayout);
    showMaximized();
}

QGroupBox *Window::createTypeGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Тип функции формы"));
    groupBox->setFlat(true);
    linearForm = new QRadioButton(tr("Линейная функция формы"));
    quadraticForm = new QRadioButton(tr("Квадратичная фукнция формы"));
    linearForm->setChecked(true);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(linearForm);
    vbox->addWidget(quadraticForm);
    //vbox->addStretch(1);
    groupBox->setLayout(vbox);
    return groupBox;
}

QGroupBox *Window::createNumberGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Количество элементов"));
    groupBox->setFlat(true);
    el20 = new QRadioButton(tr("20 элементов"));
    el40 = new QRadioButton(tr("40 элементов"));
    el20->setChecked(true);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(el20);
    vbox->addWidget(el40);
    //vbox->addStretch(1);
    groupBox->setLayout(vbox);

    return groupBox;
}

QGroupBox *Window::createNonExclusiveGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Вид решения"));
    groupBox->setFlat(true);

    checkBoxAnalitic = new QCheckBox(tr("Аналитическое решение"));
    checkBoxMKE = new QCheckBox(tr("МКЭ"));
    checkBoxMKE->setChecked(true);
    checkBoxAnalitic->setChecked(true);
    connect(checkBoxAnalitic,SIGNAL(stateChanged(int)), this, SLOT(analiticCheck(int)));
    connect(checkBoxMKE,SIGNAL(stateChanged(int)), this, SLOT(realCheck(int)));
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(checkBoxAnalitic);
    vbox->addWidget(checkBoxMKE);
    //vbox->addStretch(1);
    groupBox->setLayout(vbox);

    return groupBox;
}

QGroupBox *Window::createPushButtonGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Решение"));
    groupBox->setFlat(true);

    solveButton = new QPushButton(tr("Рассчитать"));
    error = new QLabel();
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(solveButton);
    vbox->addWidget(error);
    //vbox->addStretch(1);
    groupBox->setLayout(vbox);
    connect(solveButton, SIGNAL(clicked()), this, SLOT(calculate()));
    return groupBox;
}

void Window::calculate(){
    QString s;
    int n;
    int a = 1, b = 9;
    double du2Coef = 20;
    double duCoef = -5;
    double freeCoef = 3;
    double duNach = -6;
    double uNach = 10;
    if(el20->isChecked()){
        n = 20;
    }
    else{
        n = 40;
    }
    solve(n, a ,b,du2Coef, duCoef, freeCoef,duNach, uNach);
    //QMessageBox::information(0, "", s);
}

QVector<double> Window::solveProgon(QVector<QVector<double> > &matrix, QVector<double> v){
    QVector<double> P, Q, ans;
    int N = v.size();
    P.resize(N);
    ans.resize(N);
    Q.resize(N);
    P[1] = - matrix[1][2] / matrix[1][1];
    Q[1] = v[1]/matrix[1][1];
    //Pr9moy xod
    for(int i = 2; i < N - 1; ++i){
        P[i] = matrix[i][i+1]/(-matrix[i][i] - matrix[i][i-1]*P[i-1]);
        Q[i] =  (matrix[i][i-1] * Q[i-1] - v[i]) / (-matrix[i][i] - matrix[i][i-1] * P[i-1]);
    }
    //Obratniy xod
    ans[N-1] = (matrix[N-1][N-2] * Q[N-2] - v[N-1])/(-matrix[N-1][N-1] - matrix[N-1][N-2]*P[N-2]);
    for(int i = N - 2; i > 0; --i){
        ans[i] = P[i] * ans[i+1] + Q[i];
    }
    return ans;
}

void Window::fillLinear(QVector<QVector<double> > &matrix, double L, double du2Coef, double duCoef, double freeCoef){
    int N = matrix.size();
    for(int i = 0; i < N - 1; ++i){
        matrix[i][i] += L / 3 * freeCoef  - duCoef / 2 - (double)du2Coef/L;
        matrix[i][i+1] += L / 6 * freeCoef + duCoef / 2 + (double)du2Coef/L;
        matrix[i+1][i] += L / 6 * freeCoef - duCoef / 2 + (double)du2Coef/L;
        matrix[i+1][i+1] += L / 3 * freeCoef + duCoef / 2- (double)du2Coef/L;
    }
}

QVector<double> Window::analiticSolve(int N, double a, double L){
    QVector<double> analitic;
    for(int i = 0 ; i < N; ++i){
        double x = (double)a + L * i;
        analitic.push_back((double)-0.0564792 * pow(2.71828, -1.125 + 0.125 * x) * (-437.705 * cos(0.366572 * x) + sin(0.366572 * x) * (-202.738))
                    );
    }
    return analitic;
}

void Window::solve(int N, double a, double b, double du2Coef, double duCoef, double freeCoef, double duNach, double uNach){
    double L = (double)(b - a)/N;
    N++;
    QVector<QVector<double> > matrix;
    QVector<double> v, ans;
    v.resize(N);
    ans.resize(N);
    matrix.resize(N);
    for(int i = 0; i < N; ++i){
        matrix[i].resize(N);
    }

    fillLinear(matrix, L, du2Coef, duCoef, freeCoef);

    v[1] = -matrix[1][0] * uNach;
    v[N-1] = - duNach * du2Coef;

    ans = solveProgon(matrix, v);

    ans[0] = uNach;

    QVector<double> analitic = analiticSolve(N, a, L);

    QPolygonF aC, rC;
    double maxError = 0;
    for(int i = 0; i < N; ++i){
        double x = (double)a + L * i;
        if(fabs(ans[i] - analitic[i]) > maxError){
            maxError = fabs(ans[i] - analitic[i]);
        }
        aC << QPointF(x, ans[i]);
        rC << QPointF(x, analitic[i]);
    }
    aCurve->setSamples(aC);
    rCurve->setSamples(rC);
    error->setText(QString("Максимальная ошибка - ") + QString::number(maxError));
    funPlot->replot();
}

void Window::print(QVector<QVector<double> > matrix){
    for(int i = 0; i < matrix.size();++i){
        for(int j = 0; j < matrix.size(); ++j){
            std::cout << std::setw(4) << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Window::print(QVector<double> v){
    for(int i = 0;i < v.size();++i){
        std::cout << std::setw(4) << v[i] << " ";
    }
    std::cout << std::endl;
}

void Window::analiticCheck(int state){
    rCurve->setVisible(state);
    funPlot->replot();
}

void Window::realCheck(int state){
    aCurve->setVisible(state);
    funPlot->replot();
}

Window::~Window()
{

}