#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H
#include <QVector>
#include <QPoint>
#include <QWidget>
#include <QPen>
#include <QPainter>
#include <random>
#include<QFileDialog.h>
#include"algorithms/jarvis.h"
#include"algorithms/graham.h"
#include"algorithms/divideandconquer.h"
#include"IDW.h"

#include <fstream>
#include <QTableWidgetItem>
#include <QGraphicsView>
#include<qgraphicswidget.h>
#include <fstream>
#include <sstream>
#include <QRadioButton>
#include<QMessageBox>
#include<QDebug>
#include<exception> 
#include<qpixmap.h>
#include<qimage.h>
#include<qpushbutton.h>

#include "JSON/rapidjson/document.h"
#include "JSON/rapidjson/istreamwrapper.h"
#include "JSON/rapidjson/rapidjson.h"

//#include"sort.h"
#include"Graph.h"
#include"ChessBoard.h"
#include"C4D5.h"
#include"delaunay.cpp"
#include"convexhull.h"
#include"MyThread.h"

using namespace rapidjson;

class DrawWidget : public QWidget
{
    Q_OBJECT
        ~DrawWidget() { if (onrun) { thread->terminate(); delete thread; } };
    QVector<QPoint> _points;
    QVector<QPoint> _pointsSpecial;
    QVector<QPoint> _pointsSpecialForHull;
    QVector<QPoint> _pointsAll;
    QVector<QPoint> _schoolBuildings;
    QVector<QRadioButton*> _schoolBuildingButtons;
    std::vector<std::vector<std::pair<int, int>>> __schoolBuildingEdges;
    std::vector<int> _schoolPath;
    Graph _schoolGraph;
    QPixmap pixmap;
    std::vector<std::string> _schoolBuildingNames;
    QVector<int> _convexHull;
    double _scaleX;
    double _scaleY;

    QPen _linePen;
    QPen _textPen;
    QPen _pointPen;
    QPen _hullPointPen;
    QPen _axisPen;
    QPen _specialPointPen;
    QPen _redLinePen;
    QPen _yellowLinePen;
    QPen _mergePen;
    QBrush _brush;

    QLine _redLine;
    QLine _yellowLine;
    QPen _redPointsSpecial;

    QPoint _specialPoint;

    QVector<int> _mergeHull1;
    QVector<int> _mergeHull2;

    Jarvis * _jarvis;
    Graham * _graham;
    DivideAndConquer * _divideAndConquer;
    QVector<QString> _textsSpecial;
    QVector<QString> _textsSpecialForHull;

    int _speed;
    int _pointsCount;
public:
    explicit DrawWidget(QWidget *parent = 0);

    QPoint toScreenCoords(QPoint & point);
    QLine toScreenCoords(QLine & line);

    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
private:
    QString mapLoc = "";
    qreal scale = 1;
    int start_press_x;
    int start_press_y;
    bool is_mouse_left_btn_down_;
    bool is_mouse_right_btn_down_;
    QPoint mouse_down_pos_;
    QRectF BBox;
    int paper_x_ = 0;
    int paper_y_ = 0;
    int icount = 0;
    int isStartPoint = 0;
    int startPoint = 0;
    int endPoint = 0;
    int cellWidth = 0;
    int cellHeight = 0;
    std::map<char, QColor> colorMap;
    std::vector<std::vector<char>> chessboard;
    std::vector<std::vector<char>> maxArea;
    std::map<char, int> color_count;
    delaunay::Delaunay<double> _delaunay;
    std::vector<std::map<std::string, int>> mdict;
    std::vector<std::map<int, std::string>> reverse_dict;
    std::vector<std::string> names;
    std::vector<std::vector<std::string>> strs;
    C4D5 Tree;
    QTableWidget table;
    double ratioT = 3000;
    float windowBoardX = 8000.0;
    float windowBoardY = 6000.0;
    QPointF _dcenter;
    QPointF dp;
    QVector<Point> m_point;
    ConvexHull myHull;
    ConvexHull myIdwHull;
    QList<Triangle> DelaTins;
    QList<Triangle> hullTins;
    std::vector<std::vector<int>> testC4D;
    QString relTinPath;
    MyThread* thread;
    int onrun = 0;
signals:
    void speedChanged(int speed);
    void solveDone(int area);
    void IdwDone(const QString& newString);
    void searchDone(const QString& newString);
    void loadNames(std::vector<std::string>&);
    void onDraw();
    void killThread();
    void tinFinishedSig(float);
public slots:
    void runJarvis();
    void runGraham();
    void runDivideAndConquer();
    void generatePoints();
    void resetMarkers();
    void clearHull();
    void clearAll();
    void paintEvent(QPaintEvent * event);
    void drawPoint(QPainter& painter, QPoint& point);
    void drawText(QPainter& painter, QPoint& point, QString& text);//绘制文本
    void drawTextSet(QPainter& painter, QVector<QPoint>& points, QVector<QString>& texts);
    void drawVector(QPainter & painter, QVector<QPoint> & _points);//绘制点集
    void drawMerge(QPainter & painter);//绘制mergeHull
    void setLineRed(QPoint a, QPoint b);
    void setLineYellow(QPoint a, QPoint b);
    void setPoint(QPoint a);
    void setHull(IntVector convexHull);
    void setMerge(IntVector hull1,IntVector hull2);
    void onAlgorithmFinished();
    void drawHull(QPainter & painter);//绘制凸包
    void resizeEvent(QResizeEvent * event);
    void stopAllAlgorithms();
    void onPointsCountChanged(int value);
    void onSpeedChanged(int speed);
    void generatePointsFromTextEdit(QString InitString, QString inputString);//从文本进行IDW插值
    void generatePointsFromGeoJson(QString inputString);//从GeoJson文件进行IDW插值
    void findBoundingBox(const QVector<QPointF>& points, QRectF& BoundingBox);//获取BoundingBox
    void reshape(QVector<QPointF>& tmpPoints, QVector<QPoint>& winPoints, QRectF& BBox);//自适应缩放至全屏显示
    void loadMap();//加载校园地图
    void solve();//深搜递归求解棋盘格
    void triangulate(QString numTinPoints);//开启Delaunay三角剖分的子线程
    void loadChessBoard();//加载棋盘格
    void drawSchoolBuildings(QPainter& painter, QVector<QPoint>& points);//绘制学校的场所
    void buttonClicked();
    void findShortestPath();//寻找学校场所间的最短路径(调用Dijkstra算法)
    void drawShortestPath(QPainter& painter);//黄色高亮绘制最短路径
    void searchPath(int start, int end);//仅用于设置起点、终点
    void handleResults(QList<Triangle>* triangulation);//子线程结束后调用重绘函数
    void loadTrainData();//加载决策树的训练集
    void loadTestData();//加载决策树的测试集
    void predictDate();//决策树执行预测
    void loadTinPointsCsv(QString numTinPoints);//加载构建三角网所需的离散点数据
    void onNewThreadCreate();//触发新建线程的预处理操作
    std::vector<std::vector<std::string>> read_csv(std::string file_path);//读取csv文件，用于构建决策树
    std::vector<std::vector<int>> getFirstNColumns(std::vector<std::vector<int>>& originalData, int n);//用于分离决策树训练集的特征列和标签列
    void tinFinished(float);//提示完成三角网构建
    void drawHullFromQVP(QList<Point> hull, QPainter& painter);//另一种凸包绘制函数
};


#endif DRAWWIDGET_H