#include <cmath>
#include "glcamera.h"

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <glut.h>
#endif

#include <QDebug>

GLCamera::GLCamera()
{
    moveSpeed = 10.0;

    x = -1.0;
    y = 0.4;
    z = 1.0;

    look_x = -1.0;
    look_y = 0.4;
    look_z = 0.0;

    zoomLevel = 1.0;
    zoomSpeed = 0.4;

    // lat="46.791803,9" lon="4.427704,7"
    planeLeft = 437.0;
    planeRight = 449.0;

    planeBottom = 4674.0;
    planeTop = 4686.0;

    planeNear = 0.0;
    planeFar = 10.0;

    canvasWidth = 0;
    canvasHeight = 0;
}

void GLCamera::left()
{
    x -= (moveSpeed * (1 / std::abs(zoomLevel)));
    look_x -= (moveSpeed * (1 / std::abs(zoomLevel)));
}

void GLCamera::right()
{
    x += (moveSpeed * (1 / std::abs(zoomLevel)));;
    look_x += (moveSpeed * (1 / std::abs(zoomLevel)));;
}

void GLCamera::up()
{
    y += (moveSpeed * (1 / std::abs(zoomLevel)));;
    look_y += (moveSpeed * (1 / std::abs(zoomLevel)));;
}

void GLCamera::down()
{
    y -= (moveSpeed * (1 / std::abs(zoomLevel)));;
    look_y -= (moveSpeed * (1 / std::abs(zoomLevel)));;
}

void GLCamera::move(int dx, int dy)
{
    GLdouble gldx = planeRight - planeLeft;
    GLdouble gldy = planeTop - planeBottom;
    GLdouble aspect = GLdouble(canvasWidth) / GLdouble(canvasHeight);

    GLdouble xMax = planeRight;
    GLdouble yMax = planeTop;

    GLdouble dX = gldy * GLdouble(canvasWidth) / GLdouble(canvasHeight);
    GLdouble dY = gldx * GLdouble(canvasHeight) / GLdouble(canvasWidth);

    if(aspect >= 1.0)
    {
        xMax = planeLeft + dX;
        yMax = planeBottom + dY;
    }

    double glViewWidth = xMax - planeLeft;
    //double glViewHeight = yMax - planeBottom;

    if(fabs(x) < 4.0)
    {
        x -= ((double(dx) / double(canvasWidth)) * glViewWidth);
        look_x -= ((double(dx) / double(canvasWidth)) * glViewWidth);
    }
    else
    {
        if(std::signbit(x))
            x = -3.99999;
        else
            x = 3.99999;

        look_x = x;
    }

    if(fabs(y) < 3.0)
    {
        y += ((double(dy) / double(canvasWidth)) * gldy);
        look_y += ((double(dy) / double(canvasWidth)) * gldy);
    }
    else
    {
        if(std::signbit(y))
            y = -2.99999;
        else
            y = 2.99999;

        look_y = y;
    }
}

void GLCamera::zoomIn()
{
    if(zoomLevel == 15)
        return;

    if(planeRight - planeLeft <= 0.1 || planeTop - planeBottom <= 0.1)
        return;

    zoomLevel += 1;

    planeLeft += zoomSpeed;
    planeRight -= zoomSpeed;
    planeBottom += zoomSpeed;
    planeTop -= zoomSpeed;

    setViewingVolume();
}

void GLCamera::zoomOut()
{
    if(zoomLevel == 1)
        return;

    zoomLevel -= 1;

    planeLeft -= zoomSpeed;
    planeRight += zoomSpeed;
    planeBottom -= zoomSpeed;
    planeTop += zoomSpeed;

    setViewingVolume();
}

void GLCamera::setViewingVolume()
{
    glViewport(0, 0, canvasWidth, canvasHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    this->setProjection();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void GLCamera::setProjection()
{
    GLdouble dx = planeRight - planeLeft;
    GLdouble dy = planeTop - planeBottom;
    GLdouble aspect = GLdouble(canvasWidth) / GLdouble(canvasHeight);

    //    GLdouble dx = xMax - xOrg;
    //	GLdouble dy = yMax - yOrg;
    //	double aspect_ratio;
    //	if(fabs(dx) > 0.001 || fabs(dy) > 0.001)//Controls how far you can zoom in
    //	{
    //		if(dx >= dy)
    //		{

    //			GLdouble dY = dx * m_height / m_width;
    //			GLdouble yMax = yOrg  + dY;
    //			::glOrtho( xOrg, xMax, yOrg, yMax, -zMax, zMax);
    //		}
    //		else
    //		{
    //			GLdouble dX = dy * m_width / m_height;
    //			GLdouble xMax = xOrg + dX;
    //			::glOrtho( xOrg, xMax, yOrg, yMax, -zMax, zMax);
    //        }



    GLdouble xMax = planeRight;
    GLdouble yMax = planeTop;

    GLdouble dX = dy * GLdouble(canvasWidth) / GLdouble(canvasHeight);
    GLdouble dY = dx * GLdouble(canvasHeight) / GLdouble(canvasWidth);

    if(aspect >= 1.0)
    {
        xMax = planeLeft + dX;
        yMax = planeBottom + dY;
    }

    glOrtho(planeLeft, xMax, planeBottom, yMax, planeNear, planeFar);

//    if(aspect >= 1.0)
//    {
//        GLdouble dY = dx * GLdouble(canvasHeight) / GLdouble(canvasWidth);
//        GLdouble yMax = planeBottom + dY;
////        GLdouble dX = dy * GLdouble(canvasWidth) / GLdouble(canvasHeight);
////        GLdouble xMax = planeLeft + dX;
//        glOrtho(planeLeft, planeRight, planeBottom, yMax, planeNear, planeFar);
//    }
//    else
//    {
//        GLdouble dX = dy * GLdouble(canvasWidth) / GLdouble(canvasHeight);
//        GLdouble xMax = planeLeft + dX;
//        GLdouble dY = dx * GLdouble(canvasHeight) / GLdouble(canvasWidth);
//        GLdouble yMax = planeBottom + dY;
//        glOrtho(planeLeft, xMax, planeBottom, yMax, planeNear, planeFar);
//    }
}

void GLCamera::center()
{
    x = -1.0;
    y = 0.4;
    z = 1.0;

    look_x = -1.0;
    look_y = 0.4;
    look_z = 0.0;
//    x = 0.0;
//    y = 0.0;
//    z = 1.0;

//    look_x = 0.0;
//    look_y = 0.0;
//    look_z = 0.0;
}

