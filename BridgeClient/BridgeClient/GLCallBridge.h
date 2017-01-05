#ifndef GLCALLBRIDGE_H
#define GLCALLBRIDGE_H

#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QtOpenGL>
#include <GLUT/GLUT.h>
//#include <GL\glut.h>

class QGLShaderProgram;

class GLCallBridge : public QGLWidget
{
    Q_OBJECT

public:
    GLCallBridge(QWidget *parent = 0, QGLWidget *shareWidget = 0);
    ~GLCallBridge();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
   
    void setClearColor(const QColor &color);
    void setCard(int *);

signals:
    void clicked();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
	
private:
    void makeObject();
    int card[13],screenScale;
    char *resourcesPath;
    QColor clearColor;
    QPoint lastPos;
    GLuint textures[53];
    QVector<QVector3D> vertices;
    QVector<QVector2D> texCoords;
#ifdef QT_OPENGL_ES_2
    QGLShaderProgram *program;
#endif
};

#endif
