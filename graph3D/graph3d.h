#ifndef GRAPH3D_H
#define GRAPH3D_H

#include <QList>
#include <QString>
//#include <QGLWidget>
#include <QColor>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include "calculator/calculator.h"

class Graph3D
{
public:
    Graph3D();
    ~Graph3D();


    QString getGraph3DExpression()const{return m_graph3DExpression;}
    QString getIntervalxMin()const{return m_xminExpression;}
    QString getIntervalxMax()const{return m_xmaxExpression;}
    QString getIntervalyMin()const{return m_yminExpression;}
    QString getIntervalyMax()const{return m_ymaxExpression;}
    QString getDelta()const{return m_deltaExpression;}
    QString getVariable1()const{return m_variable_X;}
    QString getVariable2()const{return m_variable_Y;}


    // set a graph from a math expression
    void setGraph3DExpression(const QString &expression_);

    void setXInterval(const double &min_, const double &max_);
    void setXInterval(const QString &minExpression_, const QString &maxExpression_);
    void setYInterval(const double &min_, const double &max_);
    void setYInterval(const QString &minExpression_, const QString &maxExpression_);

    void setMinX(const double &minX_);
    void setMinX(const QString &minXExpression_);

    void setMaxX(const double &maxX_);
    void setMaxX(const QString &maxXExpression_);

    void setMinY(const double &minY_);
    void setMinY(const QString &minYExpression_);

    void setMaxY(const double &maxY_);
    void setMaxY(const QString &maxYExpression_);

    void setDelta(const double delta_);
    void setDelta(const QString deltaExpression_);

    void setVariable1(const QString &variable1_);
    void setVariable2(const QString &variable2_);

    // setupGraph() must be called after setMin...(), setMax...(), setDelta() and setGraph3DExpression()
    // it calculates and setup up the array's xx and yy
    bool setupGraph();
    bool setupGraph2();

    Graph3D &operator=(const Graph3D & );


    bool isTimeGraph(){return timeGraph3D;} // check if is a graph with a variable t (time)
    //in "m_graph3DExpression"
    // timeGraphs can be animated

private:

    QList<double> xx;
    QList<double> yy;
    QList<double> zz;

    bool timeGraph3D;
    QString m_graph3DExpression;

    QString m_xminExpression;
    double m_xmin;

    QString m_xmaxExpression;
    double m_xmax;

    QString m_yminExpression;
    double m_ymin;

    QString m_ymaxExpression;
    double m_ymax;

    QString m_deltaExpression;
    double m_delta;

    QString m_variable_X;
    QString m_variable_Y;

    Calculator calc;

    double t; // time
    bool bPolarGraph;

    // drawing ---------------------------------------------
    QOpenGLBuffer m_vertexBufferGraph3D;
    QOpenGLBuffer m_colorBufferGraph3D;
    QOpenGLBuffer m_colorBackBufferGraph3D;

    QOpenGLBuffer m_elementsBufferGraph3D;

    QVector3D *vertexPosition;
    QVector3D *vertexColor;
    QVector3D *vertexBackColor;

    GLint *vertexElements;


    QList<int> elements; //array elements
    int bufferSize;


    int xCount;
    int yCount;
    QColor colorA;
    QColor colorB;
    QColor colorC;
    QColor colorD;

public:
    void setColorA(const QColor &color);
    void setColorB(const QColor &color);
    void setColorC(const QColor &color);
    void setColorD(const QColor &color);

    QColor getColorA();
    QColor getColorB();
    QColor getColorC();
    QColor getColorD();

   // Graph3D &operator=(const Graph3D & );  // Right side is the argument.

public:

    void prepareBuffers();
    void prepareBuffers2();
    void releaseBuffers();
    void releaseBuffers2();
    void setBufferData(QOpenGLShaderProgram &m_shaderProgram);
    void setBufferData2(QOpenGLShaderProgram &m_shaderProgram);
    void draw(QOpenGLShaderProgram &m_shaderProgram);
    void draw2(QOpenGLShaderProgram &m_shaderProgram);

    void draw_aux(QOpenGLShaderProgram &m_shaderProgram);

    void UpdateGraphTime(double t_, QOpenGLShaderProgram &m_shaderProgram);
    bool graph_has_variable_t();
/*
    QVector3D *graph3D_fxy;
    QVector3D *graph3D_color_upper_face;
    QVector3D *graph3D_color_lower_face;
    //unsigned short *indices;

    bool bMemoryError;
    int k_x;
    int k_y;
*/


};


#endif // GRAPH3D_H
