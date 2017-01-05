#ifndef GLPLAYBRIDGE_H
#define GLPLAYBRIDGE_H

#include <QtOpenGL/QtOpenGL>
#include <GLUT/GLUT.h>

class QGLShaderProgram;

class GLPlayBridge : public QGLWidget
{
    Q_OBJECT

public:
    GLPlayBridge(QWidget *parent = 0, QGLWidget *shareWidget = 0);
    ~GLPlayBridge();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
   
    void setClearColor(const QColor &color);
    void setCard(int *);
    void setTable(int *);
    void setTurn(bool );
    void setMyturnid(int);
    void setNowFlower(int);
    void again();
    void drawText(const char *text, int length, int x, int y);
signals:
	void leadPoker(int);
    protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
	
private:
    
    void makeObject();
    int card[13],table[4],myturnid,screenScale;
    int hand_poker,now_flower;
    bool hand_card[13];
    bool choose,turn;
    char *resourcesPath;
    QColor clearColor;
    QPoint lastPos;
    GLuint textures[54];
    QVector<QVector3D> vertices;
    QVector<QVector2D> texCoords;
    
#ifdef QT_OPENGL_ES_2
    QGLShaderProgram *program;
#endif
};

#endif
