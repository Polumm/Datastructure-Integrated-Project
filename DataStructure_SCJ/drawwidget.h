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
    void drawText(QPainter& painter, QPoint& point, QString& text);//�����ı�
    void drawTextSet(QPainter& painter, QVector<QPoint>& points, QVector<QString>& texts);
    void drawVector(QPainter & painter, QVector<QPoint> & _points);//���Ƶ㼯
    void drawMerge(QPainter & painter);//����mergeHull
    void setLineRed(QPoint a, QPoint b);
    void setLineYellow(QPoint a, QPoint b);
    void setPoint(QPoint a);
    void setHull(IntVector convexHull);
    void setMerge(IntVector hull1,IntVector hull2);
    void onAlgorithmFinished();
    void drawHull(QPainter & painter);//����͹��
    void resizeEvent(QResizeEvent * event);
    void stopAllAlgorithms();
    void onPointsCountChanged(int value);
    void onSpeedChanged(int speed);
    void generatePointsFromTextEdit(QString InitString, QString inputString);//���ı�����IDW��ֵ
    void generatePointsFromGeoJson(QString inputString);//��GeoJson�ļ�����IDW��ֵ
    void findBoundingBox(const QVector<QPointF>& points, QRectF& BoundingBox);//��ȡBoundingBox
    void reshape(QVector<QPointF>& tmpPoints, QVector<QPoint>& winPoints, QRectF& BBox);//����Ӧ������ȫ����ʾ
    void loadMap();//����У԰��ͼ
    void solve();//���ѵݹ�������̸�
    void triangulate(QString numTinPoints);//����Delaunay�����ʷֵ����߳�
    void loadChessBoard();//�������̸�
    void drawSchoolBuildings(QPainter& painter, QVector<QPoint>& points);//����ѧУ�ĳ���
    void buttonClicked();
    void findShortestPath();//Ѱ��ѧУ����������·��(����Dijkstra�㷨)
    void drawShortestPath(QPainter& painter);//��ɫ�����������·��
    void searchPath(int start, int end);//������������㡢�յ�
    void handleResults(QList<Triangle>* triangulation);//���߳̽���������ػ溯��
    void loadTrainData();//���ؾ�������ѵ����
    void loadTestData();//���ؾ������Ĳ��Լ�
    void predictDate();//������ִ��Ԥ��
    void loadTinPointsCsv(QString numTinPoints);//���ع����������������ɢ������
    void onNewThreadCreate();//�����½��̵߳�Ԥ�������
    std::vector<std::vector<std::string>> read_csv(std::string file_path);//��ȡcsv�ļ������ڹ���������
    std::vector<std::vector<int>> getFirstNColumns(std::vector<std::vector<int>>& originalData, int n);//���ڷ��������ѵ�����������кͱ�ǩ��
    void tinFinished(float);//��ʾ�������������
    void drawHullFromQVP(QList<Point> hull, QPainter& painter);//��һ��͹�����ƺ���
};


#endif DRAWWIDGET_H