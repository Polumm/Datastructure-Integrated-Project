#include "drawwidget.h"
#include <QTableWidgetItem>
#include <qlayout.h>



DrawWidget::DrawWidget(QWidget *parent) : QWidget(parent)
{
    qRegisterMetaType<IntVector>("IntVector");
    _scaleX = 0.06;
    _scaleY = 0.06;
    _linePen = QPen(QColor(10, 255, 10), 2, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);
    _textPen = QPen(QColor(255, 10, 10), 1);
    _pointPen = QPen(QColor(10, 10, 255), 3);
    _hullPointPen = QPen(QColor(255, 10, 10), 4);
    _axisPen = QPen(QColor(60,60,60), 1);
    _specialPointPen = QPen(QColor(255,10,10),6);
    _redLinePen = QPen(QColor(200,10,10),1);
    _yellowLinePen = QPen(QColor(230,230,10),1);
    _mergePen = QPen(QColor(10,150,150),1);
    _redPointsSpecial = QPen(QColor(255, 10, 10), 6);
    _jarvis = 0;
    _graham = 0;
    _divideAndConquer = 0;
    _speed = 50;
    _pointsCount = 10;
    _dcenter = QPointF(114.577917, 30.871015);
    resetMarkers();
}

QPoint DrawWidget::toScreenCoords(QPoint &point)
{
    QPoint p = QPoint(point.x(),-point.y());
    p.setX(p.x() * _scaleX + paper_x_);
    p.setY(p.y() * _scaleY + paper_y_);
    QPoint center(width()/2,height()/2);
    return p + center;
}

QLine DrawWidget::toScreenCoords(QLine &line)
{

    QPoint p1 = line.p1();
    p1 = toScreenCoords(p1);
    QPoint p2 = line.p2();
    p2 = toScreenCoords(p2);
    return QLine(p1,p2);
}

void DrawWidget::drawPoint(QPainter& painter, QPoint& point)
{
    //painter.setPen(_pointPen);
    painter.drawPoint(toScreenCoords(point));
}

void DrawWidget::drawVector(QPainter& painter, QVector<QPoint>& points)
{
    for(int i = 0; i < points.size(); i++) {
        drawPoint(painter,points[i]);
    }
}

void DrawWidget::drawSchoolBuildings(QPainter& painter, QVector<QPoint>& points)
{
    int radius = 20;
    QPoint fixPoint(10, 50);
    painter.setPen(QPen(Qt::red, 3.0));
    for (int i = 0; i < points.size(); i++)
    {
        QPoint curPoint(points[i].x() + paper_x_, points[i].y() + paper_y_);
        painter.drawEllipse(curPoint, radius, radius);
        QRadioButton* nBtn = _schoolBuildingButtons[i];
        nBtn->move(scale * curPoint - fixPoint);
        //qDebug() << points[i].x() + paper_x_ << points[i].y() + paper_y_;
        //this.in
    }

    // 绘制边
    QPoint from, to;
    for (int i = 0; i < __schoolBuildingEdges.size(); ++i) {
        for (const auto& neighbour : __schoolBuildingEdges[i]) {
            int j = neighbour.first;
            from.setX(points[i].x() + paper_x_);
            from.setY(points[i].y() + paper_y_);
            to.setX(points[j].x() + paper_x_);
            to.setY(points[j].y() + paper_y_);
            painter.drawLine(from, to);
        }
    }
        //painter.drawPoint(points[i].x()+paper_x_, points[i].y() + paper_y_);
    this->update();
}

void DrawWidget::drawText(QPainter& painter, QPoint& point, QString& text)
{
    QPoint pointShow = point + QPoint(80,80);
    painter.drawText(toScreenCoords(pointShow), text);
}

void DrawWidget::drawTextSet(QPainter& painter, QVector<QPoint>& points, QVector<QString>& texts)
{
    for (int i = 0; i < texts.size(); i++)
    {
        drawText(painter, points[i], texts[i]);
    }
}


void DrawWidget::drawMerge(QPainter &painter)
{
    for(int i=0; i<_mergeHull1.size()-1; i++) {
        int indexFirst = _mergeHull1.at(i);
        int indexSecond = _mergeHull1.at(i+1);
        QPoint first = toScreenCoords(_points[indexFirst]);
        QPoint second = toScreenCoords(_points[indexSecond]);
        painter.setPen(_mergePen);
        painter.drawLine(first,second);
        painter.drawText(first.x()-8,first.y()-8,QString::number(i));
    }
    if(_mergeHull1.size() >= 2){
        painter.setPen(_mergePen);
        painter.drawLine(toScreenCoords(_points[_mergeHull1.last()]),toScreenCoords(_points[_mergeHull1.first()]));
        painter.drawText(toScreenCoords(_points[_mergeHull1.last()]).x()-8,
                         toScreenCoords(_points[_mergeHull1.last()]).y()-8,
                         QString::number(_mergeHull1.size()-1));
    }
    for(int i=0; i<_mergeHull2.size()-1; i++) {
        int indexFirst = _mergeHull2.at(i);
        int indexSecond = _mergeHull2.at(i+1);
        QPoint first = toScreenCoords(_points[indexFirst]);
        QPoint second = toScreenCoords(_points[indexSecond]);
        painter.setPen(_mergePen);
        painter.drawLine(first,second);
        painter.drawText(first.x()-8,first.y()-8,QString::number(i));
    }
    if(_mergeHull2.size() >= 2) {
        painter.setPen(_mergePen);
        painter.drawLine(toScreenCoords(_points[_mergeHull2.last()]),toScreenCoords(_points[_mergeHull2.first()]));
        painter.drawText(toScreenCoords(_points[_mergeHull2.last()]).x()-8,
                         toScreenCoords(_points[_mergeHull2.last()]).y()-8,
                         QString::number(_mergeHull2.size()-1));

    }
}

void DrawWidget::setLineRed(QPoint a, QPoint b)
{
    _redLine = QLine(a,b);
    this->repaint();
}

void DrawWidget::setLineYellow(QPoint a, QPoint b)
{
    _yellowLine = QLine(a,b);
    this->repaint();
}

void DrawWidget::setPoint(QPoint a)
{
    _specialPoint = a;
    this->repaint();
}

void DrawWidget::setHull(IntVector convexHull)
{
    _convexHull = convexHull;
    this->repaint();
}

void DrawWidget::setMerge(IntVector hull1, IntVector hull2)
{
    _mergeHull1 = hull1;
    _mergeHull2 = hull2;
    this->repaint();
}

void DrawWidget::onAlgorithmFinished()
{
    resetMarkers();
}

void DrawWidget::drawHull(QPainter &painter)
{
    if(_convexHull.size() > 0) {
        if(_convexHull.size() == 1) {
            int indexFirst = _convexHull.at(0);
            QPoint first = toScreenCoords(_points[indexFirst]);
            painter.setPen(_hullPointPen);
            painter.drawPoint(first);
            painter.setPen(_textPen);
            painter.drawText(first.x()+8,first.y()+8,QString::number(1));
        }
        for(int i=0; i<_convexHull.size()-1; i++) {
            int indexFirst = _convexHull.at(i);
            int indexSecond = _convexHull.at(i+1);
            QPoint first = toScreenCoords(_points[indexFirst]);
            QPoint second = toScreenCoords(_points[indexSecond]);
            painter.setPen(_linePen);
            painter.drawLine(first,second);
            painter.setPen(_hullPointPen);
            painter.drawPoint(first);
            painter.setPen(_textPen);
            painter.drawText(first.x()+8,first.y()+8,QString::number(i+1));
        }

        painter.setPen(_linePen);
        painter.drawLine(toScreenCoords(_points[_convexHull.last()]),toScreenCoords(_points[_convexHull.first()]));
        painter.setPen(_textPen);
        painter.drawText(toScreenCoords(_points[_convexHull.last()]).x()+8,
                         toScreenCoords(_points[_convexHull.last()]).y()+8,QString::number(_convexHull.size()));
    }
}

void DrawWidget::resizeEvent(QResizeEvent *event)
{
    resetMarkers();
    _scaleX = width()/ windowBoardX;
    _scaleY = height()/ windowBoardY;
    QWidget::resizeEvent(event);
}

void DrawWidget::stopAllAlgorithms()
{
    clearHull();
    resetMarkers();
    if(_jarvis!=0) {
        _jarvis->terminate();
        _jarvis->wait();
        delete _jarvis;
        _jarvis = 0;
    }
    if(_graham!=0) {
        _graham->terminate();
        _graham->wait();
        delete _graham;
        _graham = 0;
    }
    if(_divideAndConquer!=0) {
        _divideAndConquer->terminate();
        _divideAndConquer->wait();
        delete _divideAndConquer;
        _divideAndConquer = 0;
    }
}

void DrawWidget::onPointsCountChanged(int value)
{
    _pointsCount = value;
}

void DrawWidget::onSpeedChanged(int speed)
{
    _speed = speed;
    emit speedChanged(_speed);
}

void DrawWidget::reshape(QVector<QPointF> & tmp_points, QVector<QPoint>& winPoints, QRectF& BBox)
{
    double ymax, ratioy, xmax, ratiox, ratio;
    findBoundingBox(tmp_points, BBox);
    ratiox = windowBoardX / BBox.width();
    ratioy = windowBoardY / BBox.height();
    QPointF _center = BBox.center();
    (ratioy > ratiox) ? ratio = ratiox : ratio = ratioy;
    for (int i = 0; i < tmp_points.size(); ++i)
        winPoints.push_back(QPoint(tmp_points[i].rx() * ratio - (_center.x() * ratio), tmp_points[i].ry() * ratio - (_center.y() * ratio)));
}

void DrawWidget::loadMap()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Map file"), "", "*.map");
    QDir dir = QDir::current();
    QString relPath = dir.relativeFilePath(fileName);
    QFileInfo fileInfo(dir.absoluteFilePath(fileName));
    QDir dir2 = fileInfo.dir();
    mapLoc = dir2.path();
    try
    {
        if (relPath.isEmpty())
            return;
        clearAll();
        std::ifstream input_file(relPath.toStdString());
        int num_vertices, num_edges;
        std::string mapName;
        input_file >> mapName >> num_vertices >> num_edges;
        mapLoc = mapLoc + "/" + QString(QString::fromLocal8Bit(mapName.c_str()));

        _schoolGraph.setVerticeNum(num_vertices);
        //初始化顶点名
        for (int i = 0; i < num_vertices; i++) {
            int id; std::string name;
            input_file >> id >> name;
            _schoolGraph.AddVertexName(id, name);
        }
        //初始化顶点坐标
        for (int i = 0; i < num_vertices; i++) {
            int id; std::string pos;
            input_file >> id; std::getline(input_file, pos);
            _schoolGraph.AddVertexPos(id, pos);
        }
        //添加边对
        for (int i = 0; i < num_edges; i++) {
            int from, to, weight;
            input_file >> from >> to >> weight;
            _schoolGraph.AddEdge(from, to, weight);
        }
        std::vector<std::string> stringVector = _schoolGraph.GetVertexPos();
        __schoolBuildingEdges = _schoolGraph.GetEdges();
        for (const std::string& str : stringVector)
        {
            std::istringstream iss(str);
            int x, y;
            if (iss >> x >> y)
                _schoolBuildings.push_back(QPoint(x, y));
        }
        for (int i = 0; i < num_vertices; i++)
        {
            QRadioButton* nBtn = new QRadioButton(this);
            _schoolBuildingButtons.push_back(nBtn);
            QString btn_text = QString("%1").arg(i + 1);
            nBtn->setGeometry(_schoolBuildings[i].x() + paper_x_, _schoolBuildings[i].y() + paper_y_, 100, 100);
            nBtn->setText(btn_text);
            //nBtn->setStyleSheet("QRadioButton::indicator::unchecked { image: url(:/mainwindow/res/no_dot.png);  } }");
            nBtn->setStyleSheet("QRadioButton { font-size: 12pt; }");
            nBtn->show();
            connect(nBtn, &QRadioButton::clicked, this, &DrawWidget::buttonClicked);
        }
        _schoolBuildingNames = _schoolGraph.GetVertex_names();
        emit loadNames(_schoolBuildingNames);
        try {
            if (!pixmap.load(mapLoc))
            {
                QFile file(mapLoc);
                if (!file.open(QIODevice::ReadOnly))
                {
                    QMessageBox::warning(this, QString("Warning"), QString("Can not open for using Chinese path!%1").arg(mapLoc));
                    return;
                }
                pixmap.loadFromData(file.readAll());
            }

        }
        catch (std::exception& e)
        {
            qDebug() << "Standard exception: " << e.what() << endl;
        }
    }
    catch (std::exception& e)
    {
        qDebug() << "Standard exception: " << e.what() << endl;
    } 
}

void DrawWidget::solve()
{
    ChessBoard* dfsSolver = new ChessBoard(chessboard, color_count);
    int maxCount = dfsSolver->traverseChessboard();
    maxArea = dfsSolver->getMaxArea();
    for (int i = 0; i < maxArea.size(); i++)
    {
        for (int j = 0; j < maxArea[0].size(); j++)
        {
            qDebug() << maxArea[i][j] << " ";
        }
    }
    emit solveDone(maxCount);
}

void DrawWidget::triangulate(QString numTinPoints)
{
    if (onrun) {
        thread->terminate();
        delete thread;
        onrun = 0;
    }
    DelaTins.clear();
    thread = new MyThread(&myHull, &DelaTins, this);
    connect(thread, SIGNAL(resultReady(float)), this, SLOT(tinFinished(float)));
    thread->start();
    onrun++;
}

void DrawWidget::loadChessBoard()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Chessboard file"), "", "*.txt");
    QDir dir = QDir::current();
    QString relPath = dir.relativeFilePath(fileName);
    try
    {
        if (relPath.isEmpty())
            return;
        clearAll();
        std::ifstream file(relPath.toStdString());
        int rows, columns;
        // 读取行数和列数
        file >> rows >> columns;
        // 读取二维字符数组
        chessboard.resize(rows);
        for (int i = 0; i < rows; i++)
        {
            chessboard[i].resize(columns);
            for (int j = 0; j < columns; j++)
            {
                file >> chessboard[i][j];
                color_count[chessboard[i][j]]++;
            }
        }

        colorMap = { {'W', Qt::white}, {'B', Qt::blue},{'G', Qt::green}, {'R', Qt::red} };
        cellWidth = width() / columns;
        cellHeight = height() / rows;
    }
    catch (std::exception& e)
    {
        qDebug() << "Standard exception: " << e.what() << endl;
    }
}

void DrawWidget::generatePointsFromTextEdit(QString InitString, QString inputString)
{
    clearAll();
    paper_x_ = 0; paper_y_ = 0; scale = 1;
    _pointPen = QPen(QColor(10, 10, 255), 6, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
    QTextStream stream(&InitString);
    QVector<QPointF> tmp_points;
    std::vector<std::vector<double>> pointForBuild;
    int n, k;
    stream >> n >> k;

    for (int i = 0; i < n; ++i) {
        double x, y, value;
        stream >> x >> y >> value;
        std::vector<double> tmpForBuild = { x, y, value };
        tmp_points.push_back(QPointF(x, y));
        pointForBuild.push_back(tmpForBuild);
    }
    //reshape(tmp_points, _points, BBox);
    //resetMarkers();
    //this->repaint();

    IDW* myIDWtree = new IDW(pointForBuild); 
    std::vector<double> valueCal;
    QTextStream streamForCalUnknown(&inputString); // 4000*3000
    QVector<QPointF> pointsCal;
    std::vector<std::vector<double>> tmpPointsForCalUnknown;
    streamForCalUnknown >> n >> k;
    for (int i = 0; i < n; ++i) {
        double x, y;
        streamForCalUnknown >> x >> y;
        std::vector<double> tmp = {x, y};
        pointsCal.push_back(QPoint(x,y));
        tmpPointsForCalUnknown.push_back(tmp);
    }
    myIDWtree->calIDW(tmpPointsForCalUnknown, valueCal,k);
    QString outText;
    for (int i = 0; i < tmpPointsForCalUnknown.size(); i++)
    {
        QString tmpString = "(" + QString::number(tmpPointsForCalUnknown[i][0]) + "," + QString::number(tmpPointsForCalUnknown[i][1]) + "," + QString::number(valueCal[i]) + ")";
        QString tmpStrin2 = "" + QString::number(tmpPointsForCalUnknown[i][0]) + " " + QString::number(tmpPointsForCalUnknown[i][1]) + " " + QString::number(valueCal[i]) + "\n";
        _textsSpecial.push_back(tmpString);
        outText += tmpStrin2;
    }
    QVector<QPointF> test = tmp_points + pointsCal;
    reshape(test, _pointsSpecial, BBox);
    reshape(test, _points, BBox);
    QVector<QPoint> backup = _pointsSpecial;
    _pointsSpecial.clear();
    for (int i = tmp_points.size(); i < test.size(); i++)
    {
        _pointsSpecial.push_back(backup[i]);
    }
    resetMarkers();
    this->update();
    IdwDone(outText);

    QVector<Point> pointsAll;
    for (const auto& pointF : _points)
        pointsAll.append(Point(pointF.x(), pointF.y()));
    myIdwHull.generateHull(pointsAll);
    int hullPtCount = 1;

    for (const auto& pointF : myIdwHull.getHull())
    {
        _pointsSpecialForHull.push_back(QPoint(pointF.x, pointF.y));
        _textsSpecialForHull.push_back(QString::number(hullPtCount++));
    }
    this->repaint();
    //若需可视化编号
    /*
QVector<Point> pointsAll;
for (const auto& pointF : _points)
    pointsAll.append(Point(pointF.x(), pointF.y()));
myIdwHull.generateHull(pointsAll);

std::vector<QPoint> pointsAll2v;
for (const auto& pointF : myIdwHull.getHull())
    pointsAll2v.push_back(QPoint(pointF.x, pointF.y));

int hullPtCount = 1;
_pointsSpecialForHull.clear();
for (const auto& pointF : myIdwHull.sort_points(pointsAll2v))
    _pointsSpecialForHull.push_back(QPoint(pointF.x(), pointF.y()));
for (const auto& pointF : _pointsSpecialForHull)
    _textsSpecialForHull.push_back(QString::number(hullPtCount++));

*/
}

void DrawWidget::generatePointsFromGeoJson(QString inputString)
{
    clearAll();
    paper_x_ = 0; paper_y_ = 0; scale = 1;
    //获取GeoJson的相对路径
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open GeoJson file"), "", "*.Json");
    QDir dir = QDir::current();
    QString relPath = dir.relativeFilePath(fileName);
    try
    {
        if (relPath.isEmpty())
            return;
        clearAll();
        _pointPen = QPen(QColor(10, 10, 255), 1, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
        paper_x_ = 0; paper_y_ = 0; scale = 1;
        std::ifstream t(relPath.toStdString());
        std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

        
        double ymax, ratioy, xmax, ratiox, ratio;
        double x, y;
        // 构造json解析对象
        rapidjson::Document d;
        d.Parse(str.c_str());
        QVector<QPointF> tmp_points;

        int numLineStrings = d["features"].Size();
        std::vector<int> numPointsCounts;
        std::vector<std::vector<double>> pointForBuild;
        //读取所有LineString
        for (int i = 0; i < numLineStrings; i++)
        {
            int pointCounts = 0;
            // 获取properties和geometry
            const Value& properties = d["features"][i]["properties"];
            const Value& geometry = d["features"][i]["geometry"];
            const Value& coords = geometry["coordinates"];

            // 从properties中解析ID和ELEV高程
            int id = properties["ID"].GetInt();
            double elev = properties["ELEV"].GetDouble();

            // 解析LineString上的所有点
            for (pointCounts = 0; pointCounts < coords.Size(); pointCounts++) {
                std::vector<double> coord;
                x = coords[pointCounts][0].GetDouble();
                y = coords[pointCounts][1].GetDouble();
                tmp_points.push_back(QPointF(x, y));
                pointForBuild.push_back(coord = { x, y, elev });
            }
            numPointsCounts.push_back(pointCounts);
        }
        findBoundingBox(tmp_points, BBox);
        ratiox = windowBoardX / BBox.width();
        ratioy = windowBoardY / BBox.height();
        QPointF _center = BBox.center();
        (ratioy > ratiox) ? ratio = ratiox : ratio = ratioy;
        int pointCounts = 0;
        for (int i = 0; i < numLineStrings; i++)
        {
            for (int j = 0; j < numPointsCounts[i]; j++)
            {
                _convexHull.push_back(pointCounts);
                QPoint tmp = QPoint(tmp_points[pointCounts].rx() * ratio - (_center.x() * ratio), tmp_points[pointCounts].ry() * ratio - (_center.y() * ratio));
                _points.push_back(tmp);
                pointCounts++;
                // qDebug() << _points[i];
            }
            this->repaint();
            _convexHull.clear();
        }
        resetMarkers();
        this->repaint();
        qDebug() << _center.x() - width() / 2.0 << _center.y() - height() / 2.0 << width();
        t.close();

        IDW* myIDWtree = new IDW(pointForBuild);
        std::vector<double> valueCal;
        QTextStream streamForCalUnknown(&inputString);
        QVector<QPointF> pointsCal;
        std::vector<std::vector<double>> tmpPointsForCalUnknown;
        int n, k;
        streamForCalUnknown >> n >> k;
        for (int i = 0; i < n; ++i) {
            double x, y;
            streamForCalUnknown >> x >> y;
            std::vector<double> tmp = { x, y };
            pointsCal.push_back(QPointF(x, y));
            tmpPointsForCalUnknown.push_back(tmp);
        }

        myIDWtree->calIDW(tmpPointsForCalUnknown, valueCal, 5);
        QString outText;
        for (int i = 0; i < tmpPointsForCalUnknown.size(); i++)
        {
            QString tmpString = "(" + QString::number(tmpPointsForCalUnknown[i][0]) + "," + QString::number(tmpPointsForCalUnknown[i][1]) + "," + QString::number(valueCal[i]) + ")";
            QString tmpStrin2 = "" + QString::number(tmpPointsForCalUnknown[i][0]) + " " + QString::number(tmpPointsForCalUnknown[i][1]) + " " + QString::number(valueCal[i]) + "\n";
            _textsSpecial.push_back(tmpString);
            outText += tmpStrin2;
        }
        QVector<QPointF> test = tmp_points + pointsCal;
        reshape(test, _pointsSpecial, BBox);
        QVector<QPoint> backup = _pointsSpecial;
        _pointsSpecial.clear();
        for (int i = tmp_points.size(); i < test.size(); i++)
        {
            _pointsSpecial.push_back(backup[i]);
        }
        resetMarkers();
        this->update();
        IdwDone(outText);
    }
    catch(std::exception& e)
    {
        qDebug() << "Standard exception: " << e.what() << endl;
    }

}

void DrawWidget::findBoundingBox(const QVector<QPointF>& points, QRectF& BoundingBox)
{
    double xMin = points[0].x();
    double yMin = points[0].y();
    double xMax = points[0].x();
    double yMax = points[0].y();
    for (const QPointF& p : points) {
        xMin = std::min(xMin, p.x());
        yMin = std::min(yMin, p.y());
        xMax = std::max(xMax, p.x());
        yMax = std::max(yMax, p.y());
    }
    BoundingBox.setLeft(xMin);
    BoundingBox.setTop(yMin);
    BoundingBox.setWidth(xMax - xMin);
    BoundingBox.setHeight(yMax - yMin);
}

void DrawWidget::runJarvis()
{
    qDebug() << "Running Jarvis algorithm";
    _convexHull.clear();
    stopAllAlgorithms();
    if(_points.size()>0) {
        _jarvis = new Jarvis();
        _jarvis->setInputData(_speed,_points);
        connect(_jarvis,SIGNAL(setLineRed(QPoint,QPoint)),this,SLOT(setLineRed(QPoint,QPoint)));
        connect(_jarvis,SIGNAL(setPoint(QPoint)),this,SLOT(setPoint(QPoint)));
        connect(_jarvis,SIGNAL(setHull(IntVector)),this,SLOT(setHull(IntVector)));
        connect(_jarvis,SIGNAL(finished()),this,SLOT(onAlgorithmFinished()));
        connect(this, SIGNAL(speedChanged(int)),_jarvis, SLOT(setSpeed(int)) );
        _jarvis->start();
    }
    this->repaint();
}

void DrawWidget::runGraham()
{
    qDebug() << "Running Graham algorithm";
    _convexHull.clear();
    stopAllAlgorithms();
    if(_points.size()>0) {
        _graham = new Graham();
        _graham->setInputData(_speed,_points);
        connect(_graham,SIGNAL(setLineRed(QPoint,QPoint)),this,SLOT(setLineRed(QPoint,QPoint)));
        connect(_graham,SIGNAL(setPoint(QPoint)),this,SLOT(setPoint(QPoint)));
        connect(_graham,SIGNAL(setHull(IntVector)),this,SLOT(setHull(IntVector)));
        connect(_graham,SIGNAL(finished()),this,SLOT(onAlgorithmFinished()));
        connect(this, SIGNAL(speedChanged(int)),_graham, SLOT(setSpeed(int)) );
        _graham->start();
    }
    this->repaint();
}

void DrawWidget::runDivideAndConquer()
{
    qDebug() << "Running Divide And Conquer algorithm";
    _convexHull.clear();
    stopAllAlgorithms();
    if(_points.size()>0) {
        _divideAndConquer = new DivideAndConquer();
        _divideAndConquer->setInputData(_speed,_points);
        connect(_divideAndConquer,SIGNAL(setLineRed(QPoint,QPoint)),this,SLOT(setLineRed(QPoint,QPoint)));
        connect(_divideAndConquer,SIGNAL(setLineYellow(QPoint,QPoint)),this,SLOT(setLineYellow(QPoint,QPoint)));
        connect(_divideAndConquer,SIGNAL(setPoint(QPoint)),this,SLOT(setPoint(QPoint)));
        connect(_divideAndConquer,SIGNAL(setHull(IntVector)),this,SLOT(setHull(IntVector)));
        connect(_divideAndConquer,SIGNAL(setMerge(IntVector,IntVector)),this,SLOT(setMerge(IntVector,IntVector)));
        connect(_divideAndConquer,SIGNAL(resetMarkers()),this,SLOT(resetMarkers()));
        connect(_divideAndConquer,SIGNAL(clearHull()),this,SLOT(clearHull()));
        connect(_divideAndConquer,SIGNAL(finished()),this,SLOT(onAlgorithmFinished()));
        connect(this, SIGNAL(speedChanged(int)),_divideAndConquer, SLOT(setSpeed(int)) );
        _divideAndConquer->start();
    }
    this->repaint();
}

void DrawWidget::generatePoints()
{
    clearAll();
    paper_x_ = 0; paper_y_ = 0; scale = 1;
    bool useNormalX = rand()%2;
    bool useNormalY = rand()%2;

    std::default_random_engine generator;
    generator.seed(time(NULL));
    std::normal_distribution<double> distribution(0,1.0);
    for(int i = 0; i < 3 * _pointsCount; i++) {
        double x;
        double y;
        if(useNormalX) {
            x = distribution(generator) * 1000;
        } else {
            x = rand() % 7000 - 3500;
        }
        if(useNormalY) {
            y = distribution(generator) * 1000;
        } else {
            y = rand() % 5000 - 2500;
        }
        _points.push_back(QPoint(x ,y));
    }
    resetMarkers();
    this->repaint();
}

void DrawWidget::resetMarkers()
{
    _redLine = QLine(QPoint(-100000,-100000),QPoint(-100000,-100000));
    _yellowLine = QLine(QPoint(-100000,-100000),QPoint(-100000,-100000));
    _specialPoint = QPoint(-100000,-100000);
    _mergeHull1.clear();
    _mergeHull2.clear();
    this->repaint();
}

void DrawWidget::clearHull()
{
    _convexHull.clear();
    this->repaint();
}

void DrawWidget::clearAll()
{
    _points.clear();
    _pointsSpecial.clear();
    _convexHull.clear();
    _textsSpecial.clear();
    _pointsSpecial.clear();
    _textsSpecialForHull.clear();
    _pointsSpecialForHull.clear();
    myIdwHull.clearHull();
    resetMarkers();
    stopAllAlgorithms();
}

void DrawWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.scale(scale,scale);
    int w = width();
    int h = height();
    painter.fillRect(0,0,w,h,Qt::white);

    QPen pen(Qt::black);
    QPen pen2(Qt::black);
    pen.setWidth(0.1);
    pen2.setWidth(2);

    painter.setPen(pen);

    for (int i = 0; i < myHull.getTins().size(); i++)
    {
        Triangle tri = myHull.getTins()[i];
        QPointF tripts[3];
        for (int j = 0; j < 3; j++)
        {
            tripts[j] = QPointF(paper_x_, paper_y_ - 900) - QPointF(tri.p[j].x * ratioT - (_dcenter.x() * ratioT), tri.p[j].y * ratioT - (_dcenter.y() * ratioT));
        }
        painter.drawPolygon(tripts, 3);
    }
    for (int i = 0; i < m_point.size(); i++) 
        painter.drawPoint(QPointF(paper_x_, paper_y_ - 900) - QPointF(m_point[i].x * ratioT - (_dcenter.x() * ratioT), m_point[i].y * ratioT - (_dcenter.y() * ratioT)));
    for (int i = 0; i < myHull.getHull().size() - 1; i++)
    {
        QPointF from = QPointF(paper_x_, paper_y_ - 900) - QPointF(myHull.getHull()[i].x * ratioT - (_dcenter.x() * ratioT), myHull.getHull()[i].y * ratioT - (_dcenter.y() * ratioT));
        QPointF to = QPointF(paper_x_, paper_y_ - 900) - QPointF(myHull.getHull()[i+1].x * ratioT - (_dcenter.x() * ratioT), myHull.getHull()[i+1].y * ratioT - (_dcenter.y() * ratioT));
        painter.drawLine(from, to);
        if (i == myHull.getHull().size() - 2)
            painter.drawLine(QPointF(paper_x_, paper_y_ - 900) - QPointF(myHull.getHull()[0].x * ratioT - (_dcenter.x() * ratioT), myHull.getHull()[0].y * ratioT - (_dcenter.y() * ratioT)), QPointF(paper_x_, paper_y_ - 900) - QPointF(myHull.getHull()[myHull.getHull().size() - 1].x * ratioT - (_dcenter.x() * ratioT), myHull.getHull()[myHull.getHull().size() - 1].y * ratioT - (_dcenter.y() * ratioT)));
    }

    drawHullFromQVP(myIdwHull.getHull(), painter);

    if (mapLoc != "" && mapLoc != "TestData/CampusMap")
    {
        QRect target(rect().x(), rect().y(), 2 * width(), 2 * height());
        painter.drawPixmap(target, pixmap, QRect(-paper_x_, -paper_y_, 2 * width(), 2 * height()));
    }
    else if(chessboard.empty())
    {
        painter.setPen(_axisPen);
        painter.drawLine(w / 2 + paper_x_, h + paper_y_, w / 2 + paper_x_, 0 + paper_y_);
        painter.drawLine(w + paper_x_, h / 2 + paper_y_, 0 + paper_x_, h / 2 + paper_y_);
    }

    for (int i = 0; i < chessboard.size(); i++)
        for (int j = 0; j < chessboard[0].size(); j++)
            painter.fillRect(j * cellWidth, i * cellHeight, cellWidth, cellHeight, colorMap[chessboard[i][j]]);
    for (int i = 0; i < chessboard.size(); i++)
        for (int j = 0; j < chessboard[0].size(); j++)
        {
            painter.fillRect(j * cellWidth, i * cellHeight, cellWidth, cellHeight, colorMap[chessboard[i][j]]);
            if (!maxArea.empty() && maxArea[i][j] == (char)-1)
            {
                painter.setPen(QPen(QBrush(QColor(0, 0, 0)), 7));
                painter.drawRect(QRect(j * cellWidth, i * cellHeight, cellWidth, cellHeight));
                QBrush brush(Qt::black, Qt::DiagCrossPattern);
                painter.fillRect(QRect(j * cellWidth, i * cellHeight, cellWidth, cellHeight), brush);
            }
        }

    painter.setPen(Qt::white);
    if(!chessboard.empty())
        for (int i = 0; i <= chessboard[0].size(); i++)
            painter.drawLine(i * cellWidth, 0, i * cellWidth, height());
    for (int i = 0; i <= chessboard.size(); i++)
        painter.drawLine(0, i * cellHeight, width(), i * cellHeight);
    

    drawSchoolBuildings(painter, _schoolBuildings);
    if (startPoint != endPoint && isStartPoint % 2 == 0)
        drawShortestPath(painter);

    painter.setFont(QFont("Arial", 8));
    
    painter.setPen(_pointPen);
    drawVector(painter,_points);

    drawHull(painter);

    painter.setPen(_redPointsSpecial);
    drawVector(painter, _pointsSpecial);
    painter.setPen(_textPen);
    drawTextSet(painter, _pointsSpecial, _textsSpecial);
    
    drawVector(painter, _pointsSpecial);
    painter.setPen(_textPen);
    drawTextSet(painter, _pointsSpecialForHull, _textsSpecialForHull);

    painter.setPen(_specialPointPen);
    painter.drawPoint(toScreenCoords(_specialPoint));

    painter.setPen(_redLinePen);
    painter.drawLine(toScreenCoords(_redLine));

    painter.setPen(_yellowLinePen);
    painter.drawLine(toScreenCoords(_yellowLine));
    drawMerge(painter);
}

void DrawWidget::wheelEvent(QWheelEvent* event)
{
    scale += (event->delta() / 1200.0); //or use any other step for zooming 
    update();
}

void DrawWidget::mousePressEvent(QMouseEvent* event)
{
    //mousePressEvent记录拖动过程中初次点击的坐标
    if (event->buttons() & Qt::RightButton) {

        is_mouse_left_btn_down_ = true;

    }
    else if (event->buttons() & Qt::LeftButton) {
        //鼠标按下的坐标，相当于拖动过程的起点
        mouse_down_pos_ = event->pos();
        //减当前变化量paper_x_,paper_y_是为了在多次拖动的过程中累计坐标变化量，可以实现连续地拖动
        start_press_x = mouse_down_pos_.x() - paper_x_;
        start_press_y = mouse_down_pos_.y() - paper_y_;
        is_mouse_left_btn_down_ = true;
    }

}

void DrawWidget::mouseMoveEvent(QMouseEvent* event)
{
    //mouseMoveEvent记录拖动过程释放鼠标的最终坐标
    if (is_mouse_left_btn_down_ == true) {
        //计算相对于本次拖动初始位置的偏差
        paper_x_ = event->pos().x() - start_press_x;
        paper_y_ = event->pos().y() - start_press_y;
        update();
    }
}

void DrawWidget::mouseReleaseEvent(QMouseEvent* event)
{
    is_mouse_left_btn_down_ = false;
    is_mouse_right_btn_down_ = false;
}

void DrawWidget::buttonClicked()
{
    QRadioButton* Btn = qobject_cast<QRadioButton*>(sender());
    int curPoint = Btn->text().toInt() - 1;
    (isStartPoint++ % 2) ? endPoint = curPoint : startPoint = curPoint;
    qDebug() << "s" << startPoint << "e" << endPoint;
    Btn->setAutoExclusive(false);
    Btn->setChecked(false);
    Btn->setAutoExclusive(true);
    if (startPoint != endPoint)
        findShortestPath();
    this->repaint();
}

void DrawWidget::findShortestPath()
{
    _schoolPath = _schoolGraph.Dijkstra(startPoint,endPoint);
    QString pathText = "We suggest taking the optimal path from ";
    for (int i = 0; i < _schoolPath.size(); i++)
    {
        if (i != 0 && i != _schoolPath.size() - 1)
            pathText = pathText + " to "; //"(" + QString::number(_schoolGraph.GetEdgeWeight(_schoolPath[i - 1], _schoolPath[i])) + ")" + 
        if (i == _schoolPath.size() - 1)
            pathText += " and then to ";
        pathText += QString(QString::fromLocal8Bit(_schoolGraph.GetVertexName(_schoolPath[i]).c_str()));
    }
    qDebug() << pathText;
    emit searchDone(pathText);
}

void DrawWidget::drawShortestPath(QPainter& painter)
{
    int radius = 20;
    painter.setPen(QPen(Qt::yellow, 3.5));
    for (int i = 0; i < _schoolPath.size(); i++)
    {
        QPoint curPoint(_schoolBuildings[_schoolPath[i]].x() + paper_x_, _schoolBuildings[_schoolPath[i]].y() + paper_y_);
        painter.drawEllipse(curPoint, radius, radius);
    }

    // 绘制边
    QPoint from, to;
    for (int i = 0; i < _schoolPath.size() - 1; ++i) {
        from.setX(_schoolBuildings[_schoolPath[i]].x() + paper_x_);
        from.setY(_schoolBuildings[_schoolPath[i]].y() + paper_y_);
        to.setX(_schoolBuildings[_schoolPath[i + 1]].x() + paper_x_);
        to.setY(_schoolBuildings[_schoolPath[i + 1]].y() + paper_y_);
        painter.drawLine(from, to);
    }
    this->update();
}

void DrawWidget::searchPath(int start, int end)
{
    startPoint = start;
    endPoint = end;
    if (startPoint != endPoint)
        findShortestPath();
    this->repaint();
}

void DrawWidget::handleResults(QList<Triangle>* triangulation)
{
    //_delaunay = triangulation;
    this->repaint();
}

void DrawWidget::loadTestData()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open the DT_data_test"), "", "*.csv");
    QDir dir = QDir::current();
    QString relPath = dir.relativeFilePath(fileName);
    try
    {
        if (relPath.isEmpty())
            return;
        std::vector<std::vector<std::string>>strs = read_csv(relPath.toStdString());
        std::vector<std::vector<int>> featureVectors(strs.size()-1, std::vector<int>(strs[0].size()));
        std::vector<std::string> names = strs[0];
        strs.erase(strs.begin());
        for (int j = 0; j < strs[0].size(); j++) {
            int nextInt = 0;
            for (int i = 0; i < strs.size(); i++) {
                const std::string& str = strs[i][j];
                featureVectors[i][j] = mdict[j][str];
            }
        }
        testC4D = getFirstNColumns(featureVectors, featureVectors[0].size() - 1);

        table.setParent(this);
        table.setGeometry(0, 0, width(), height());
        table.setWindowTitle("Test data");
        // 设置列数
        table.setColumnCount(names.size());
        QStringList labels;
        for (int i = 0; i < names.size(); i++)
        {
            labels.push_back(QString::fromStdString(names[i]));
        }
        table.setHorizontalHeaderLabels(labels);

        table.setRowCount(strs.size());

        // 填充数据
        for (int i = 0; i < strs.size(); i++) {
            for (int j = 0; j < names.size(); j++) {
                QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(strs[i][j]));
                table.setItem(i, j, item);
            }
        }
        table.show();
    }
    catch (std::exception& e)
    {
        qDebug() << "Standard exception: " << e.what() << endl;
    }
}

void DrawWidget::tinFinished(float timeExcute)
{
    emit tinFinishedSig(timeExcute);
}

void DrawWidget::onNewThreadCreate()
{
    emit killThread();
    DelaTins.clear();
}

void DrawWidget::loadTrainData()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open train DT_data"), "", "*.csv");
    QDir dir = QDir::current();
    QString relPath = dir.relativeFilePath(fileName);
    try
    {
        //读取数据
        if (relPath.isEmpty())
            return;
        strs = read_csv(relPath.toStdString());
        //特征向量为vector<int>型，数据集为特征向量构成的矩阵
        std::vector<std::vector<int>> featureVectors(strs.size()-1, std::vector<int>(strs[0].size()));
        //记录表头然后除表头
        names = strs[0];
        strs.erase(strs.begin());
        //构建特征向量
        for (int j = 0; j < strs[0].size(); j++) {
            //建立每个特征的类别与特征向量取值之间的映射
            std::map<std::string, int> strToInt;
            std::map<int, std::string> intToStr;
            int nextInt = 0;
            for (int i = 0; i < strs.size(); i++) {
                const std::string& str = strs[i][j];
                if (strToInt.count(str) == 0) {
                    strToInt[str] = nextInt;
                    intToStr[nextInt++] = str;
                }
                featureVectors[i][j] = strToInt[str];
            }
            mdict.push_back(strToInt);
            reverse_dict.push_back(intToStr);
        }
        Tree.setDimension(names.size()-1);
        //分离特征和标签
        std::vector<std::vector<int>> input = getFirstNColumns(featureVectors, featureVectors[0].size() - 1);
        //建立C4.5D决策树
        for (int i = 0; i < input.size(); i++)
        {
            Tree.PushData(&input[i][0], featureVectors[i][featureVectors[0].size() - 1]);
        }
        Tree.Build();

        table.setParent(this);
        table.setGeometry(0, 0, width(), height());
        table.setWindowTitle("Train data");
        // 设置列数
        table.setColumnCount(names.size());
        QStringList labels;
        for (int i = 0; i < names.size(); i++)
        {
            labels.push_back(QString::fromStdString(names[i]));
        }
        table.setHorizontalHeaderLabels(labels);

        table.setRowCount(strs.size());

        // 填充数据
        for (int i = 0; i < strs.size(); i++) {
            for (int j = 0; j < names.size(); j++) {
                QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(strs[i][j]));
                table.setItem(i, j, item);
            }
        }
        table.show();
    }
    catch (std::exception& e)
    {
        qDebug() << "Standard exception: " << e.what() << endl;
    }
}

void DrawWidget::predictDate()
{
    //for (int i = 0; i < testC4D.size(); i++)
    //    qDebug() << QString::fromStdString(reverse_dict[reverse_dict.size() - 1][Tree.Match(&testC4D[i][0])]);
    table.setWindowTitle("Predict");
    // 设置列数
    table.setColumnCount(names.size());
    QStringList labels;
    for (int i = 0; i < names.size(); i++)
    {
        labels.push_back(QString::fromStdString(names[i]));
    }
    table.setHorizontalHeaderLabels(labels);

    // 填充数据
    int j = testC4D[0].size() + 1;
    for (int i = 0; i < testC4D.size(); i++) 
    {
        QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(reverse_dict[reverse_dict.size() - 1][Tree.Match(&testC4D[i][0])]));
        item->setBackground(Qt::yellow);
        table.setItem(i, j, item);
    }
    table.show();
}

void DrawWidget::loadTinPointsCsv(QString numTinPoints)
{
    if (onrun) {
        thread->terminate();
        delete thread;
        onrun = 0;
    }
    if (relTinPath=="")
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open the TIN_data.csv"), "", "*.csv");
        QDir dir = QDir::current();
        relTinPath = dir.relativeFilePath(fileName);
    }
    try
    {
        m_point.clear();
        DelaTins.clear();
        if (relTinPath.isEmpty())
            return;
        std::ifstream file(relTinPath.toStdString());
        std::string line;
        std::getline(file, line); // 跳过第一行
        std::vector<delaunay::Point<double>> points;
        int point_count = 0;
        if (numTinPoints!="All" && numTinPoints != "the number of points for building")
        {
            while (std::getline(file, line) && point_count != numTinPoints.toInt())
            {
                std::stringstream line_stream(line);
                std::string cell;
                std::vector<std::string> values;
                while (std::getline(line_stream, cell, ','))
                {
                    values.push_back(cell);
                }
                double x = std::stod(values[2]);
                double y = std::stod(values[3]);
                m_point.push_back(Point(x, y));
                ++point_count;
            }
        }
        else
        {
            while (std::getline(file, line))
            {
                std::stringstream line_stream(line);
                std::string cell;
                std::vector<std::string> values;
                while (std::getline(line_stream, cell, ','))
                {
                    values.push_back(cell);
                }
                double x = std::stod(values[2]);
                double y = std::stod(values[3]);
                m_point.push_back(Point(x, y));
                ++point_count;
            }
        }
        myHull.generateHull(m_point);
    }
    catch (std::exception& e)
    {
        qDebug() << "Standard exception: " << e.what() << endl;
    }
}

std::vector<std::vector<std::string>> DrawWidget::read_csv(std::string file_path){
    std::ifstream file(file_path);
    std::vector<std::vector<std::string>> data;
    std::string line;
    while (getline(file, line)) {
        std::stringstream line_stream(line);
        std::string cell;
        std::vector<std::string> cells;
        while (getline(line_stream, cell, ',')) {
            cells.push_back(cell);
        }
        data.push_back(cells);
    }
    return data;
}

std::vector<std::vector<int>> DrawWidget::getFirstNColumns(std::vector<std::vector<int>>& originalData, int n)
{
    std::vector<std::vector<int>> newData;
    for (auto& row : originalData)
    {
        std::vector<int> newRow(row.begin(), row.begin() + n);
        newData.push_back(newRow);
    }
    return newData;
}

void DrawWidget::drawHullFromQVP(QList<Point> hull, QPainter& painter)
{
    if (!hull.isEmpty())
    {
        QPolygon polygon;
        for (const auto& point : hull) {
            QPoint p = QPoint(point.x, - point.y);
            p.setX(p.x() * _scaleX + paper_x_);
            p.setY(p.y() * _scaleY + paper_y_);
            QPoint center(width() / 2, height() / 2);
            polygon.append(p + center);
        }
        painter.drawPolygon(polygon);
    }
}