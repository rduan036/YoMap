#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QWheelEvent>

#include "glcamera.h"

#include "db/database.h"

#include "gl/glpoipoint.h"

#include "db/Relations/path.h"

#include "algorithms/pathalgorithms.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT
private:
    GLCamera* camera;
    QTimer *timer;

    QPoint lastPos;

    int canvasWidth;
    int canvasHeight;

    bool isStop;
    bool isGLInitialized;

    map<unsigned long int,Way*>* allWays;
    map<unsigned long int,Building *>* allBuildings;

    vector<WaySegment*> path;
    Path newPath;

    set<Path*,ComparePaths> radiusSearch;
    float searchRadius;

    QPointF *startPoint;
    QPointF *endPoint;

    vector<GLPOIPoint*> glPOIPoints;

    void drawPOIPoints();
    void scalePOIPoints(double);
    void drawPath();
    void drawRadiusSearch();

    void drawStartPoint();
    void drawEndPoint();

    void drawWays();
    void drawBuildings();

    void drawMapBorder();
    boost_xy_point mapMinBound;
    boost_xy_point mapMaxBound;

    bool shouldDrawBuildings;

    bool shouldDrawAllWays;
    bool shouldDrawWays1;
    bool shouldDrawWays2;
    bool shouldDrawWays3;
    bool shouldDrawWays4;
    bool shouldDrawWays5;
    bool shouldDrawWays6;
    bool shouldDrawWays7;
    bool shouldDrawWays8;
    bool shouldDrawWays9;

    bool shouldDrawPOI;

    bool shouldDrawMapBorder;

public:
    explicit GLWidget(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void setMapBounds(boost_xy_point, boost_xy_point);

    void keyPressEvent(QKeyEvent*);

    void wheelEvent(QWheelEvent*);

    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent *);

    void startGL();
    void stopGL();
    QPointF getGeoPosition(QPoint);
    POIPoint* getIfPOI(QPointF);

    void setMap(map<unsigned long int,Way*>*);
    void setBuildings(map<unsigned long int,Building *>*);

    void setPath(vector<WaySegment*>);
    void setPath(Path);
    void deletePath();

    void setRadiusSearch(set<Path*,ComparePaths>, float);
    void deleteRadiusSearch();

    void setPOIs(vector<POIPoint*>);
    void deletePOIs();


    void drawStartPoint(QPointF*);
    void drawEndPoint(QPointF*);
    void deleteStartPoint();
    void deleteEndPoint();

    void setSearchRadius(float);

    QString pickOpenGLColor(int);

signals:
    void poiClicked(POIPoint*, QMouseEvent*);

public slots:
    void updateLC();
    void searchRadiusChanged(float);
    void centerMap();

};

#endif // GLWIDGET_H
