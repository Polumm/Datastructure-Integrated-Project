#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<qdebug.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this,SIGNAL(generatePoints()),ui->drawWidget,SLOT(generatePoints()));
    connect(this, SIGNAL(generatePointsFromTextEdit(QString, QString)), ui->drawWidget_2, SLOT(generatePointsFromTextEdit(QString, QString)));
    connect(this, SIGNAL(generatePointsFromGeoJson(QString)), ui->drawWidget_2, SLOT(generatePointsFromGeoJson(QString)));
    connect(this,SIGNAL(runJarvis()),ui->drawWidget,SLOT(runJarvis()));
    connect(this,SIGNAL(runGraham()),ui->drawWidget,SLOT(runGraham()));
    connect(this,SIGNAL(runDivideAndConquer()),ui->drawWidget,SLOT(runDivideAndConquer()));
    connect(this,SIGNAL(clearAll()),ui->drawWidget,SLOT(clearAll()));
    connect(this, SIGNAL(clearAll()), ui->drawWidget_2, SLOT(clearAll()));
    connect(this, SIGNAL(searchPath(int, int)), ui->drawWidget_3, SLOT(searchPath(int, int)));
    connect(this,SIGNAL(stop()),ui->drawWidget,SLOT(stopAllAlgorithms()));
    connect(this,SIGNAL(pointsCountChanged(int)),ui->drawWidget,SLOT(onPointsCountChanged(int)));
    connect(this,SIGNAL(speedChanged(int)),ui->drawWidget,SLOT(onSpeedChanged(int)));
    connect(this,SIGNAL(loadMap()),ui->drawWidget_3,SLOT(loadMap()));
    connect(this,SIGNAL(solve()),ui->drawWidget_4,SLOT(solve()));
    connect(this,SIGNAL(triangulate(QString)),ui->drawWidget_5,SLOT(triangulate(QString)));
    connect(ui->drawWidget_4,SIGNAL(solveDone(int)),this,SLOT(solveDone(int)));
    connect(this,SIGNAL(loadChessBoard()),ui->drawWidget_4,SLOT(loadChessBoard()));
    connect(this, SIGNAL(loadTrainData()), ui->drawWidget_6, SLOT(loadTrainData()));
    connect(this, SIGNAL(loadTestData()), ui->drawWidget_6, SLOT(loadTestData()));
    connect(this, SIGNAL(predictDate()), ui->drawWidget_6, SLOT(predictDate()));
    connect(this, SIGNAL(newThread()), ui->drawWidget_5, SLOT(onNewThreadCreate()));
    connect(ui->drawWidget_2, &DrawWidget::IdwDone, this, &MainWindow::on_IdwDone);
    connect(ui->drawWidget_3, &DrawWidget::searchDone, this, &MainWindow::on_searchDone);
    connect(ui->drawWidget_3, &DrawWidget::loadNames, this, &MainWindow::on_loadNames);
    connect(ui->drawWidget_5, SIGNAL(tinFinishedSig(float)), this, SLOT(tinFinished(float)));
    connect(this, SIGNAL(loadTinPointsCsv(QString)), ui->drawWidget_5, SLOT(loadTinPointsCsv(QString)));
    ui->textEdit->setPlainText("10\n5\n7 9 22.6\n-8 -1 37.0\n-3 -1 38.2\n1 4 33.0\n-3 9 16.5\n6 -4 25.2\n7 5 25.0\n6 6 33.5\n-6 10 36.2\n0 8 33.2");
    ui->textEdit_2->setPlainText("5 3\n3 4\n-4 -1.5\n-5 -0.5\n5 1\n-5 4");
    ui->testButton1->setStyleSheet("color: rgb(255, 255, 255); background-color: rgb(51, 102, 102);");
    ui->generateButton_2->setStyleSheet("color: rgb(255, 255, 255); background-color: rgb(51, 102, 102);");
    ui->testButton2->setStyleSheet("color: rgb(255, 255, 255); background-color: rgb(255, 102, 51);");
    ui->generateButton_3->setStyleSheet("color: rgb(255, 255, 255); background-color: rgb(255, 102, 51);");
    ui->loadMapButton->setStyleSheet("color: rgb(255, 255, 255); background-color: rgb(51, 102, 102);");
    ui->comboBox->setFixedHeight(34);
    ui->comboBox->setInsertPolicy(QComboBox::NoInsert);
    ui->comboBox->setEditable(true);
    ui->comboBox->lineEdit()->setReadOnly(true);
    ui->comboBox->lineEdit()->setText("Start");
    ui->comboBox_2->setFixedHeight(34);
    ui->comboBox_2->setInsertPolicy(QComboBox::NoInsert);
    ui->comboBox_2->setEditable(true);
    ui->comboBox_2->lineEdit()->setReadOnly(true);
    ui->comboBox_2->lineEdit()->setText("End");
    ui->comboBox_3->addItem("All");
    ui->comboBox_3->addItem("500");
    ui->comboBox_3->addItem("5000");
    ui->comboBox_3->addItem("10000");
    ui->comboBox_3->addItem("20000");
    ui->comboBox_3->setFixedHeight(34);
    ui->comboBox_3->setInsertPolicy(QComboBox::NoInsert);
    ui->comboBox_3->setEditable(true);
    ui->comboBox_3->lineEdit()->setReadOnly(true);
    ui->comboBox_3->lineEdit()->setText("the number of points for building");
    //ui->comboBox_3-

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_generateButton_released()
{
    emit generatePoints();
}

void MainWindow::on_generateButton_2_released()
{
    QString initString = ui->textEdit->toPlainText();
    QString inputString = ui->textEdit_2->toPlainText();
    emit generatePointsFromTextEdit(initString, inputString);
}

void MainWindow::on_generateButton_3_released()
{
    QString inputString = ui->textEdit_2->toPlainText();
    emit generatePointsFromGeoJson(inputString);
}

void MainWindow::on_loadTrainDataButton_released()
{
    emit loadTrainData();
}

void MainWindow::on_loadTestDataButton_released()
{
    emit loadTestData();
}

void MainWindow::on_predictButton_released()
{
    emit predictDate();
}

void MainWindow::on_loadMapButton_released()
{
    emit loadMap();
}

void MainWindow::on_loadChessBoardButton_released()
{
    emit loadChessBoard();
}

void MainWindow::on_searchPathButton_released()
{
    int start = ui->comboBox->currentIndex();
    int end = ui->comboBox_2->currentIndex();
    emit searchPath(start, end);
}

void MainWindow::on_IdwDone(const QString& newString)
{
    ui->textEdit_4->setText(newString);
}

void MainWindow::on_searchDone(const QString& newString)
{
    ui->label->setText(newString);
}

void MainWindow::on_loadNames(std::vector<std::string>& names)
{
    for (int i = 0; i < names.size(); i++)
        ui->comboBox->addItem(QString(QString::fromLocal8Bit(names[i].c_str())));
    for (int i = 0; i < names.size(); i++)
        ui->comboBox_2->addItem(QString(QString::fromLocal8Bit(names[i].c_str())));
}

void MainWindow::on_solveButton_released()
{
    emit solve();
}

void MainWindow::on_triangulateButton_released()
{
    //emit newThread();//结束当前线程，避免访问冲突
    ui->label_4->setText("");
    emit triangulate(ui->comboBox_3->currentText());
}

void MainWindow::on_loadTinButton_released()
{
    ui->label_4->setText("");
    emit loadTinPointsCsv(ui->comboBox_3->currentText());
}

void MainWindow::solveDone(int area)
{
    ui->label_3->setText("The maximum continuous area of the same color is "+ QString::number(area) +".");
    qDebug() << "s";
}

void MainWindow::tinFinished(float timeExcute)
{
    QString pointNumTin = ui->comboBox_3->currentText();
    if (pointNumTin == "the number of points for building" || pointNumTin == "All")
        pointNumTin = "all";
    ui->label_4->setText("The TIN network with "+ pointNumTin +" points has been constructed in " + QString::number(timeExcute) + " seconds");
}

void MainWindow::on_pushButton_released()
{
    //演示算法与可视化分离版本的Granham算法
    ui->tabWidget->setCurrentIndex(4);
}

void MainWindow::on_jarvisButton_released()
{
    emit runJarvis();
}

void MainWindow::on_grahamButton_released()
{
    emit runGraham();
}

void MainWindow::on_dacButton_released()
{
    emit runDivideAndConquer();
}

void MainWindow::on_clearButton_released()
{
    emit clearAll();
}

void MainWindow::on_clearButton_2_released()
{
    emit clearAll();
}

void MainWindow::on_stopButton_released()
{
    emit stop();
}

void MainWindow::on_pointsSlider_valueChanged(int value)
{
    emit pointsCountChanged(value);
}

void MainWindow::on_speedSlider_valueChanged(int value)
{
    emit speedChanged(value);
}

void MainWindow::on_testButton1_released()
{
    ui->textEdit_2->setPlainText("5 3\n3 4\n-4 -3\n-5 -2\n5 1\n-5 4");
    ui->label_2->setText("The format of the interpolation points should be consistent with the sample data for the first line being n and k, and the data range should be similar.");
}

void MainWindow::on_testButton2_released()
{
    ui->textEdit_2->setPlainText("5 3\n102.610621469347248 23.11303965511434\n102.62246294950783 23.114685322972026\n102.578806943141876 23.094473229079338\n102.606616667895958 23.113043833660317\n102.594269643380064 23.093934239908865");
    ui->label_2->setText("The format of the interpolation points should be consistent with the sample data for the first line being n and k, and the data range should be similar.");
}
