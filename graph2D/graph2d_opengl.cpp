#include "graph2d_opengl.h"

#include <QFileDialog>
#include <QImageWriter>
#include <QMessageBox>


Graph2D_OpenGL::Graph2D_OpenGL()//: m_graph2D(calc_)
{
    //calc = calc_;
    setAttribute(Qt::WA_DeleteOnClose);//qDebug()<<"Delete on close: "<<testAttribute(Qt::WA_DeleteOnClose);

    setMouseTracking(true);
    background_dots = NULL;

    Range       = 50;
    minScale    = 0.005;
    maxScale    = 100;
    scaleDelta  = 1.25;
    scale       = 1;
    backgroundColor.setRgbF(0,0,0,1);
    axisColor.setRgbF(1,1,1,1);
    //axisColor.setRgbF(0.9,0.9,0.9,1);


    t       = 0;
    t_delta = 0.5;
    Timer2D = 0;
}

Graph2D_OpenGL::~Graph2D_OpenGL()
{
    qDebug()<<"destructor, list size"<<m_graph2DList.size();
    m_graph2DList.clear();

    //delete[] dots;
    //delete[] dotsColor;
}

void Graph2D_OpenGL::initializeGL()
{
    initializeOpenGLFunctions();

    initializeAxis2D();
    prepareShaderProgram();
    prepareVertexBuffers();

    //glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClearColor(backgroundColor.redF(),backgroundColor.greenF(), backgroundColor.blueF(), 0.0f);
    //glClearColor(0.1f, 0.4f, 0.1f, 1.0f);


    // Set our own shaderprogram as an active one to GPU. There
    // can be only one active program at the time.
    m_shaderProgram.bind();
}

void Graph2D_OpenGL::prepareShaderProgram()
{
    if (m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/shaders/vertexShaderES2.vsh"))
        qDebug() << "Sucess compiling and adding vertex shader";
    else
        qDebug() << "Error compiling and adding vertex shader: " << m_shaderProgram.log();


    if (m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders/shaders/fragmentShaderES2.fsh"))
        qDebug() << "Sucess compiling and adding fragment shader";
    else
        qDebug() << "Error compiling and adding fragment shader: " << m_shaderProgram.log();


    // Link our program. It's now ready for use.
    if (m_shaderProgram.link())
        qDebug() << "Sucess linking shader program";
    else
        qDebug() << "Error linking shader program: " << m_shaderProgram.log();
}

void Graph2D_OpenGL::prepareVertexBuffers()
{

    if (m_vertexPositionBuffer.create()) qDebug() << "Success creating vertex position buffer";
    m_vertexPositionBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    if (m_vertexPositionBuffer.bind()) qDebug() << "Success biding vertex position buffer";
    m_vertexPositionBuffer.allocate(axis2D, ( 4 )* 3 * sizeof(float));



    //if (m_shaderProgram.bind()) qDebug() << "Success biding shader program";

    m_vertexPositionBuffer.bind();
    m_shaderProgram.enableAttributeArray("vertexPosition");
  //  m_shaderProgram.setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3);

    m_shaderProgram.disableAttributeArray("vertexColor");
    m_shaderProgram.setUniformValue("vertexColor",axisColor);

}


void Graph2D_OpenGL::initializeAxis2D()
{

    //initializeAxis2DBackGroundDots();

    axis2D[0].setX(Range);  axis2D[0].setY(0.0f);   axis2D[0].setZ(0.0f);
    axis2D[1].setX(-Range); axis2D[1].setY(0.0f);   axis2D[1].setZ(0.0f);
    axis2D[2].setX(0.0f);   axis2D[2].setY(Range);  axis2D[2].setZ(0.0f);
    axis2D[3].setX(0.0f);   axis2D[3].setY(-Range); axis2D[3].setZ(0.0f);


}

void Graph2D_OpenGL::initializeAxis2DBackGroundDots()
{

    int xmin = -Range;
    int xmax = Range;
    int ymin = -Range;
    int ymax = Range;


    int dots_dist = 2;
    number_of_dots_2 = ( (xmax - xmin) / dots_dist) * ( (ymax - ymin) / dots_dist );

    background_dots = new QVector3D[number_of_dots_2];


    int index = 0;
    for (int x = xmin; x <= xmax; x++)
    {
        for (int y = ymin; y <= ymax; y++)
        {
            background_dots[index].setX(x);
            background_dots[index].setY(y);
            background_dots[index].setZ(0);
            index++;
        }
    }

  //  if (m_vertexBufferGraph2D.bind()) qDebug() << "setBufferData() - Success biding vertex position graph 2D buffer";

    /*if (m_vertexBufferGraph2D.bind()) qDebug() << "setBufferData() - Success biding vertex position graph 2D buffer";
    else qDebug()<<"setBufferData() - something wrong";
    m_vertexBufferGraph2D.allocate(vertexPosition, size * 3 * sizeof(float));*/

    delete[] background_dots;
}


void Graph2D_OpenGL::resizeGL(int width, int height)
{
    w=width;
    h=height;
    glViewport(0,0, width, height);
}

void Graph2D_OpenGL::showEvent(QShowEvent *event)
{
    qDebug()<<"showing";
    Q_UNUSED(event);
    prepareGraphs();
    time.start();
}

void Graph2D_OpenGL::closeEvent(QCloseEvent *event)
{
    //qDebug()<<"closing graphopengl";
    releaseGraphs();
}


void Graph2D_OpenGL::prepareGraphs()
{

    for (int i = 0; i < m_graph2DList.size(); i++)
    {
     //   m_graph2DList[i].setColor(1,0,0);
        m_graph2DList[i].prepareBuffers();
        m_graph2DList[i].setBufferData(m_shaderProgram);
    }

    update();
}


void Graph2D_OpenGL::releaseGraphs()
{   
    for (int i = 0; i < m_graph2DList.size(); i++)
    {
        m_graph2DList[i].releaseBuffers();
    }
}


void Graph2D_OpenGL::paintGL()
{
    int tt = time.elapsed();
    //qDebug("Time elapsed: %d ms", tt);
    if (tt != 0)
        qDebug("Time elapsed: %d ms - Fps: %lf ", tt, double(1000/tt));
    time.restart();


    glClearColor(backgroundColor.redF(), backgroundColor.greenF(), backgroundColor.blueF(), backgroundColor.alphaF());
    // Clear the background and depth-buffer for this frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    projection.setToIdentity();

    if (width() <= height())
        projection.ortho(-(Range),Range,-Range*h/w,Range*h/w,-(Range*2),Range*2);
    else
        projection.ortho(-(Range*w/h),Range*w/h,-Range,Range,-(Range*2),Range*2);


    orientation.setToIdentity();
    orientation.translate( translate_xy.x(), translate_xy.y(), 0.0f );
    orientation.scale( scale );

    m_shaderProgram.setUniformValue( "proj", projection );// Set projection to the shader
    m_shaderProgram.setUniformValue( "matrix", orientation );// Set orientation matrix to the shaderprogram


    drawAxis2D();

    for (int i = 0; i < m_graph2DList.size(); i++)
    {
        m_graph2DList[i].draw(m_shaderProgram);
     //   qDebug()<<m_graph2DList.size();
    }


}

void Graph2D_OpenGL::drawAxis2D()
{


    m_vertexPositionBuffer.bind();
//    if (m_vertexPositionBuffer.bind()) qDebug() << "Success biding vertex position buffer";
   // m_shaderProgram.enableAttributeArray("vertexPosition");
    m_shaderProgram.setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3);

   // m_shaderProgram.disableAttributeArray("vertexColor");

    m_shaderProgram.setUniformValue("vertexColor",axisColor);


    glDrawArrays(GL_LINES,0 ,4);


}

bool Graph2D_OpenGL::areTimeGraphs()
{
    for (int i = 0; i < m_graph2DList.size() ; i++)
    {
        if (m_graph2DList[i].isTimeGraph() && (!m_graph2DList[i].isParametricGraph()))
            return true;
    }
    return false;
}

void Graph2D_OpenGL::startStopTimer2D()
{
    if (Timer2D == 0 && areTimeGraphs())
    {
        Timer2D = startTimer(0);
        return;
    }

    if (Timer2D != 0)
    {
        killTimer(Timer2D);
        Timer2D = 0;
        t       = 0;
    }
}


void Graph2D_OpenGL::stopTimer2D()
{
    if (Timer2D != 0)
    {
        killTimer(Timer2D);
        Timer2D = 0;
        t       = 0;
    }
}


void Graph2D_OpenGL::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == Timer2D)
    {
        t += t_delta;
        for(int i = 0; i < m_graph2DList.size(); i++)
        {
            m_graph2DList[i].UpdateGraphTime(t, m_shaderProgram);
        }

        qDebug()<<"updating t: "<<t;
        update();
        return;
    }

    QOpenGLWidget::timerEvent(event);
}


void Graph2D_OpenGL::setBackGroundColor(const QColor &color_)
{
    backgroundColor = color_;
    glClearColor(backgroundColor.redF(),backgroundColor.greenF(), backgroundColor.blueF(), 1.0f);

    update();
    //paintGL();
}


void Graph2D_OpenGL::SaveImageAs()
{
    QString filename;
    //QString format = "jpg";
    //QString fileformat;
    //fileformat = sprintf("Image (*.%s)",format)
    filename = QFileDialog::getSaveFileName( this,
                                            QObject::tr("Save File"),
                                            getenv( "HOME" ), QObject::tr(" Image (*.png)") );
                                            //getenv( "HOME" ), fileformat );

    if( !filename.endsWith( ".png" ) )
    {
        filename.append( ".png" );
    }

    QPixmap pixmap( size() );
    render( &pixmap );
    QImage image = grabFramebuffer();
    QImageWriter imageWriter( filename, "png" );

    //imageWriter.setQuality(100);

    if( imageWriter.canWrite() )
    {
        imageWriter.write(image);
    }
    else
    {
        QMessageBox::warning( this, QObject::tr("Save Image"), QObject::tr("Error saving image.") );
    }
}

void Graph2D_OpenGL::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);
    qDebug("Hiding window");

    killTimer(Timer2D);
    Timer2D = 0;

}

void Graph2D_OpenGL::mousePressEvent(QMouseEvent *event)
{
#if !defined(Q_WS_S60)
{
    lastPos = MouseCoordinates_ToViewport(event->pos());
}
#endif
}

void Graph2D_OpenGL::mouseDoubleClickEvent(QMouseEvent * event)
{
    Q_UNUSED(event);
    scale   = 1;
    translate_xy.setX(0);
    translate_xy.setY(0);
    lastPos.setX(0);
    lastPos.setY(0);
    update();
}


void Graph2D_OpenGL::mouseMoveEvent(QMouseEvent *event)
{
    mousePicking(event->pos());

#if !defined(Q_WS_S60)
{
    if (event->buttons() == Qt::LeftButton || event->buttons() == Qt::RightButton)
    {
        translate_xy += (MouseCoordinates_ToViewport(event->pos()) - lastPos) * scale;
        update();
    }
    lastPos = MouseCoordinates_ToViewport(event->pos());
}
#endif
}


void Graph2D_OpenGL::wheelEvent(QWheelEvent * event)
{

    if (event->delta()<0)
        if (scale > minScale)
        {
            scale/=scaleDelta;
            translate_xy += (MouseCoordinates_ToViewport(event->pos()) - lastPos)* scale;
        }

    if (event->delta()>0)
        if (scale < maxScale)
        {
            scale*=scaleDelta;
            translate_xy += (MouseCoordinates_ToViewport(event->pos()) - lastPos)* scale;
        }

   // qDebug()<<scale;
    update();
    event->accept();
}

bool Graph2D_OpenGL::event(QEvent *event)
{
  //  qDebug()<<"Event";
    ///////////////////////////////////////////////////
    //Zoom touch
    switch (event->type()) {
    case QEvent::TouchBegin:
    {
        qDebug()<<"TouchBegin";
        QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
        QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();

        if (touchPoints.count() == 1)
        {
            lastPos = MouseCoordinates_ToViewport(touchPoints.first().pos());
            event->accept();
            return true;
        }
        break;
    }
    case QEvent::TouchUpdate:
    {
        qDebug()<<"TouchUpdate";
        QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
        QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();

        if (touchPoints.count() == 1)
        {
            translate_xy += (MouseCoordinates_ToViewport(touchPoints.first().pos()) - lastPos) * scale;
            lastPos = MouseCoordinates_ToViewport(touchPoints.first().pos());
            update();
            event->accept();
            return true;
        }

        if (touchPoints.count() == 2)
        {
            // determine scale factor
            const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
            const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
            qreal currentScaleFactor =
                    QLineF(touchPoint0.pos(), touchPoint1.pos()).length()
                    / QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length();
           // if (touchEvent->touchPointStates() & Qt::TouchPointMoved /*Qt::TouchPointReleased*/)
            {
                // if one of the fingers is released, remember the current scale
                // factor so that adding another finger later will continue zooming
                // by adding new scale factor to the existing remembered value.

                scale *= currentScaleFactor;
                currentScaleFactor = 1;
                if (scale < minScale)
                    scale=minScale;
                if (scale > maxScale)
                    scale=maxScale;

                /*if (currentScaleFactor<1)
                    if (scale > 0.01)
                        scale/=1.1;

                if (currentScaleFactor>1)
                    if (scale < 20)
                        scale*=1.1;*/

                update();
                event->accept();
                return true;
            }
            return true;
        }
        break;
    }
    case QEvent::TouchEnd:
    {
        qDebug()<<"TouchEnd";
        break;
    }
    default:
        break;
    }
    ///////////////////////////////////////////////////

    return QOpenGLWidget::event(event);
}

QPointF Graph2D_OpenGL::MouseCoordinates_ToViewport(const QPointF &p)
{
    int ww=width();
    int hh=height();
    double xx;
    double yy;

    if (ww <= hh)
    {
        xx = 2*Range/ww*p.x()-Range ;
        yy = -2*Range/ww*p.y()+Range*hh/ww;
    }
    else
    {
        xx = 2*Range/hh*p.x()-Range*ww/hh;
        yy = -2*Range/hh*p.y()+Range;
    }

    //return QPointF(xx,yy);
    return QPointF((xx - translate_xy.x()) / scale, (yy - translate_xy.y()) / scale);
}

QVector3D Graph2D_OpenGL::mousePicking(const QPointF &p)
{

    bool bInverted;
    int w = width();
    int h = height();

    QMatrix4x4 mvp = projection * orientation;
    QMatrix4x4 inverted_mvp = mvp.inverted(&bInverted);
    QMatrix4x4 inverse_proj = projection.inverted(&bInverted);
    QMatrix4x4 inverse_view = orientation.inverted(&bInverted);


    QVector4D mouseXYZW(p.x(), p.y(), 1.0f, 1.0f);
    QVector4D ray4D = inverted_mvp * mouseXYZW;

    QVector3D ray3D = QVector3D(ray4D.x(), ray4D.y(), ray4D.z());

    qDebug()<<"mousePicking x, y"<<p.x()<<" ; "<<p.y();
    qDebug()<<"ray3d x, y"<<ray3D.x()<<" ; "<<ray3D.y();

  /*

    gl_Position = proj*matrix * vertexPosition;

    projection.setToIdentity();

    if (width() <= height())
        projection.ortho(-(Range),Range,-Range*h/w,Range*h/w,-(Range*2),Range*2);
    else
        projection.ortho(-(Range*w/h),Range*w/h,-Range,Range,-(Range*2),Range*2);


    orientation.setToIdentity();
    orientation.translate( translate_xy.x(), translate_xy.y(), 0.0f );
    orientation.scale( scale );

    m_shaderProgram.setUniformValue( "proj", projection );// Set projection to the shader
    m_shaderProgram.setUniformValue( "matrix", orientation );// Set orientation matrix to the shaderprogram
*/

    /*
    qDebug()<<"mousePicking x, y"<<p.x()<<" ; "<<p.y();
    float x = (2.0 * p.x()) / width() - 1.0;
    float y = 1.0 - (2.0 * p.y()) / height();

    qDebug()<<"normalised x, y"<<x<<" ; "<<y;

    return QVector3D();
    */
}


void Graph2D_OpenGL::addGraph2D(const Graph2D &graph2d_)
{
    //m_graph2D = graph2d_;
    m_graph2DList.append(graph2d_);

}
