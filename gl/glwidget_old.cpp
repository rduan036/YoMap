#include "glwidget.h"

#include <QtOpenGL>

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <glut.h>
#endif

#include <QDebug>

#include "db/database.h"
#include "algorithms/pathalgorithms.h"
#include "stable.h"

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    canvasWidth = 0;
    canvasHeight = 0;
    timer = new QTimer(this);
    camera = new GLCamera();

    isStop = false;

//    d = new Database();
//    d->build("/Users/emreozanalkan/Desktop/SoftwareEngineering/osm/LeCreusotWaysFF.osm");

    d = new Database("D:\\Bicycle\\LeCreusotWaysFF.osm");
}

void GLWidget::initializeGL()
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glClearColor(0, 0, 0, 1.0);
    glColor3f(1.0, 1.0, 1.0);

    glEnable(GL_DEPTH_TEST);
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    connect(timer, SIGNAL(timeout()), this, SLOT(updateLC()));
    timer->start(10);
    this->setFocus();
}

void GLWidget::updateLC()
{
    // Do my update stuff

    //map->update();

    updateGL();
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(camera->x, camera->y, camera->z,
              camera->look_x, camera->look_y, camera->look_z,
              0.0, 1.0, 0.0);

    glPushMatrix();

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(0.5f, 0.5f);
    glEnd();

    //glScalef(1.001f, 1.000f, 1.0f);

    glColor3f(1.0f, 1.0f, 1.0f);

    //glLineWidth(5.0f);

    glLineWidth(1.0f);

    map<unsigned long int,Way*>* allWays = d->getAllWays();
    map<unsigned long int, Way*>::iterator wayIt;
    for(wayIt = allWays->begin(); wayIt != allWays->end(); ++wayIt)
    {
        glBegin(GL_LINE_STRIP);
        //((Way*)(*wayIt).second)->getNodesBegin()
        for (vector<Node*>::iterator nodeIt = ((Way*)(*wayIt).second)->getNodesBegin(); nodeIt != ((Way*)(*wayIt).second)->getNodesEnd(); nodeIt++){
            //cout<<(*nodeIt)->getId()<<endl;
            boost_xy_point& nodeGeoPos = (*nodeIt)->getGeoPosition();
            //qDebug() << "Node Pos: " << nodeGeoPos.x() << " " << nodeGeoPos.y();
            //glVertex2f(nodeGeoPos.x() * 1000000, nodeGeoPos.y() * 1000000);
            //glVertex2f(nodeGeoPos.x() * 100, nodeGeoPos.y() * 100);
            glVertex3f(nodeGeoPos.x() * 100, nodeGeoPos.y() * 100, 0.0f);
            //qDebug() << "Node Pos: " << nodeGeoPos.x() * 100<< " " << nodeGeoPos.y() * 100;
            //glVertex2d(nodeGeoPos.x(), nodeGeoPos.y());
            // lat="46.7918039" lon="4.4277047"
        }
        glEnd();
    }

//        vector<WaySegment*> segments_in_area;
//        //d->searchWaySegmentsInArea(boost_xy_point(46.78,4.402), boost_xy_point(46.79,4.403),segments_in_area);
//        d->searchWaySegmentsInArea(boost_xy_point(4.400,46.76), boost_xy_point(4.403,46.79),segments_in_area);

//        //qDebug() << "Size of the segment: " << segments_in_area.size();



//        glColor3f(1.0f, 0.0f, 0.0f);

//        for(int i = 0; i < segments_in_area.size(); i++)
//        {
//            WaySegment* waySegment = segments_in_area[i];
//            boost_xy_point& nodeGeoPosA = waySegment->getPointA()->getGeoPosition();
//            boost_xy_point& nodeGeoPosB = waySegment->getPointB()->getGeoPosition();
//            glBegin(GL_LINES);
//                glVertex3f(nodeGeoPosA.x() * 100, nodeGeoPosA.y() * 100, 1.0f);
//                glVertex3f(nodeGeoPosB.x() * 100, nodeGeoPosB.y() * 100, 1.0f);
//            glEnd();
//        }

    glColor3f(1.0f, 0.0f, 0.0f);
    glLineWidth(5.0f);

    vector<WaySegment*> path;
    float cost,time;
    boost_xy_point start_point(4.4097667,46.7901184);
    boost_xy_point end_point(4.4249021,46.7990036);
    ns_permisions::transport_type tt = ns_permisions::foot;

    bool found = PathAlgorithms::findShortestPath(d, start_point, end_point, tt, path, cost, time);
    //bool found = PathAlgorithms::findShortestPath(&d, &start_point, &end_point, tt, path, cost, time);

    if(found)
    {
        for(int i = 0; i < path.size(); i++)
        {
            WaySegment* waySegment = path[i];
            boost_xy_point& nodeGeoPosA = waySegment->getPointA()->getGeoPosition();
            boost_xy_point& nodeGeoPosB = waySegment->getPointB()->getGeoPosition();
            glBegin(GL_LINES);
            //qDebug() << "A: " <<nodeGeoPosA.x() << "-" << nodeGeoPosA.y();
            //qDebug() << "B: " <<nodeGeoPosB.x() << "-" << nodeGeoPosB.y();
            glVertex3f(nodeGeoPosA.x() * 100, nodeGeoPosA.y() * 100, 1.0f);
            glVertex3f(nodeGeoPosB.x() * 100, nodeGeoPosB.y() * 100, 1.0f);
            glEnd();
        }
    }

    glPopMatrix();
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    camera->canvasWidth = w;
    camera->canvasHeight = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float ratio = float(w) / float(h);

    glOrtho(camera->planeLeft,
            camera->planeRight,
            camera->planeBottom,
            camera->planeTop,
            camera->planeNear,
            camera->planeFar);

    //gluPerspective(60.0, ratio, 1.0, 100.0);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_W:
        camera->up();
        break;
    case Qt::Key_A:
        camera->left();
        break;
    case Qt::Key_S:
        camera->down();
        break;
    case Qt::Key_D:
        camera->right();
        break;
    case Qt::Key_Q:
        camera->zoomIn();
        break;
    case Qt::Key_E:
        camera->zoomOut();
        break;
    case Qt::Key_Up:
        camera->up();
        break;
    case Qt::Key_Down:
        camera->down();
        break;
    case Qt::Key_Left:
        camera->left();
        break;
    case Qt::Key_Right:
        camera->right();
        break;
    case Qt::Key_Plus:
        camera->zoomIn();
        break;
    case Qt::Key_Minus:
        camera->zoomOut();
        break;
    case Qt::Key_Escape:
        this->parentWidget()->parentWidget()->close();
        break;
    default:
        break;
    }

    event->accept();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    if(event->orientation() == Qt::Vertical)
    {
        if(event->delta() > 0)
            camera->zoomIn();
        else
            camera->zoomOut();
    }
    else
    {
        if(event->delta() > 0)
            camera->right();
        else
            camera->left();
    }

    event->accept();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    event->accept();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug() << "mouseMoveEvent"<<endl;
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

//    qDebug() << "lastPos.x: "<< lastPos.x() << " lastPost.y: " << lastPos.y() <<endl;
//    qDebug() << "event->x: "<< event->x() << " event->y: " << event->y() << endl;
//    qDebug() << "dx: "<< dx << " dy: " << dy << endl;
//    qDebug() << "===========================================" << endl;

    if (event->buttons() & Qt::LeftButton) {
        camera->move(dx, dy);
        //setXRotation(xRot + 8 * dy);
        //setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        //setXRotation(xRot + 8 * dy);
        //setZRotation(zRot + 8 * dx);
    }

    lastPos = event->pos();
}

//void GLWidget::mouseReleaseEvent(QMouseEvent *event)
//{
//    if(event->button() == Qt::RightButton)
//    {
//        lastRightClickPos = event->pos();
//        QMenu menu;

//        QAction* actionStart = new QAction("Set as Start Point", this);
//        menu.addAction(actionStart);

//        menu.addSeparator();

//        QAction* actionEnd = new QAction("Set as End Point", this);
//        menu.addAction(actionEnd);

//        connect(actionStart, SIGNAL(triggered()), this, SLOT(setStartPoint()));
//        connect(actionEnd, SIGNAL(triggered()), this, SLOT(setEndPoint()));

//        menu.exec(mapToGlobal(event->pos()));
//    }

//    QGLWidget::mouseReleaseEvent(event);
//}

//void GLWidget::setStartPoint()
//{
//    qDebug() << "set end point called";
//    qDebug() << lastRightClickPos;

//}

//void GLWidget::setEndPoint()
//{
//    qDebug() << "set end point called";
//    qDebug() << lastRightClickPos;
//}

void GLWidget::startGL()
{
    isStop = false;
    timer->start(10);
}

void GLWidget::stopGL()
{
    isStop = true;
    timer->stop();
}

QPointF GLWidget::getGeoPosition(QPoint point)
{
    return QPointF(4.4097667, 46.7901184);
}

void GLWidget::centerMap()
{

}
