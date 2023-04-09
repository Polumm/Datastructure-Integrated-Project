#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QComboBox.h>
#include<qlineedit.h>
#include<QLabel.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void generatePoints();
    void runJarvis();
    void runGraham();
    void runDivideAndConquer();
    void clearAll();
    void stop();
    void pointsCountChanged(int count);
    void speedChanged(int speed);
    void loadTinPointsCsv(QString numTinPoints);
    void generatePointsFromTextEdit(QString InitString, QString inputString);
    void generatePointsFromGeoJson(QString inputString);
    void loadMap();
    void solve();
    void loadChessBoard();
    void searchPath(int start, int end);
    void triangulate(QString numTinPoints);
    void loadTrainData();
    void loadTestData();
    void predictDate();
    void newThread();
private slots:
    void on_generateButton_released();
    void on_pushButton_released();
    void on_jarvisButton_released();
    void on_grahamButton_released();
    void on_dacButton_released();
    void on_clearButton_released();
    void on_clearButton_2_released();
    void on_stopButton_released();
    void on_pointsSlider_valueChanged(int value);
    void on_speedSlider_valueChanged(int value);
    void on_testButton1_released();
    void on_testButton2_released();
    void on_generateButton_2_released();
    void on_generateButton_3_released();
    void on_loadTrainDataButton_released();
    void on_loadTestDataButton_released();
    void on_predictButton_released();
    void on_loadMapButton_released();
    void on_loadChessBoardButton_released();
    void on_searchPathButton_released();
    void on_IdwDone(const QString& newString);
    void on_searchDone(const QString& newString);
    void on_loadNames(std::vector<std::string>& names);
    void on_solveButton_released();
    void on_triangulateButton_released();
    void on_loadTinButton_released();
    void solveDone(int area);
    void tinFinished(float timeExcute);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
