//1175  95
#include "GLCallBridge.h"
#include "CoreFoundation/CoreFoundation.h"

GLCallBridge::GLCallBridge(QWidget *parent, QGLWidget *shareWidget): QGLWidget(parent, shareWidget)
{
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    resourcesPath = (char *)malloc(sizeof(char)*PATH_MAX);
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)resourcesPath, PATH_MAX)) // Error: expected unqualified-id before 'if'
    {
        // error!
    }
    CFRelease(resourcesURL);
    
    clearColor = Qt::green;
    for (int i=0; i<13; ++i) {
        card[i]=0;
    }
    screenScale = devicePixelRatio();
#ifdef QT_OPENGL_ES_2
    program = 0;
#endif
}

GLCallBridge::~GLCallBridge()
{
}

QSize GLCallBridge::minimumSizeHint() const
{
    return QSize(1175, 95);
}

QSize GLCallBridge::sizeHint() const
{
    return QSize(1175, 95);
}

void GLCallBridge::setClearColor(const QColor &color)
{
    clearColor = color;
    updateGL();
}
void GLCallBridge::setCard(int *temp)
{
    for(int i=0;i<13;++i)
        card[i]=temp[i];

	updateGL();
}

void GLCallBridge::initializeGL()
{
    makeObject();

    glEnable(GL_CULL_FACE);
#ifndef QT_OPENGL_ES_2
    glEnable(GL_TEXTURE_2D);
#endif

#ifdef QT_OPENGL_ES_2

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1

    QGLShader *vshader = new QGLShader(QGLShader::Vertex, this);
    const char *vsrc =
        "attribute highp vec4 vertex;\n"
        "attribute mediump vec4 texCoord;\n"
        "varying mediump vec4 texc;\n"
        "uniform mediump mat4 matrix;\n"
        "void main(void)\n"
        "{\n"
        "    gl_Position = matrix * vertex;\n"
        "    texc = texCoord;\n"
        "}\n";
    vshader->compileSourceCode(vsrc);

    QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);
    const char *fsrc =
        "uniform sampler2D texture;\n"
        "varying mediump vec4 texc;\n"
        "void main(void)\n"
        "{\n"
        "    gl_FragColor = texture2D(texture, texc.st);\n"
        "}\n";
    fshader->compileSourceCode(fsrc);

    program = new QGLShaderProgram(this);
    program->addShader(vshader);
    program->addShader(fshader);
    program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    program->link();

    program->bind();
    program->setUniformValue("texture", 0);
    
#endif
}

void GLCallBridge::paintGL()
{
    qglClearColor(clearColor);
    glClear(GL_COLOR_BUFFER_BIT );

#if !defined(QT_OPENGL_ES_2)
    glLoadIdentity();

    glVertexPointer(3, GL_FLOAT, 0, vertices.constData());
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords.constData());
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

#else
    QMatrix4x4 m;
    m.gluOrtho2D(0, 1175, 0, 95);
    program->setUniformValue("matrix", m);
    program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    program->setAttributeArray
        (PROGRAM_VERTEX_ATTRIBUTE, vertices.constData());
    program->setAttributeArray
        (PROGRAM_TEXCOORD_ATTRIBUTE, texCoords.constData());
#endif
    //draw~~~
    for (int i = 0; i < 13; ++i) {
        glBindTexture(GL_TEXTURE_2D, textures[card[i]]);
        glDrawArrays(GL_QUADS, i * 4, 4);
    }
}

void GLCallBridge::resizeGL(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLCallBridge::makeObject()
{
    static const int coords[13][4][2] = {
        { { 325, 80}, { 275, 80}, { 275, 0}, { 325, 0} },
        { { 375, 80}, { 325, 80}, { 325, 0}, { 375, 0} },
        { { 425, 80}, { 375, 80}, { 375, 0}, { 425, 0} },
        { { 475, 80}, { 425, 80}, { 425, 0}, { 475, 0} },
        { { 525, 80}, { 475, 80}, { 475, 0}, { 525, 0} },
        { { 575, 80}, { 525, 80}, { 525, 0}, { 575, 0} },
        { { 625, 80}, { 575, 80}, { 575, 0}, { 625, 0} },
        { { 675, 80}, { 625, 80}, { 625, 0}, { 675, 0} },
        { { 725, 80}, { 675, 80}, { 675, 0}, { 725, 0} },
        { { 775, 80}, { 725, 80}, { 725, 0}, { 775, 0} },
        { { 825, 80}, { 775, 80}, { 775, 0}, { 825, 0} },
        { { 875, 80}, { 825, 80}, { 825, 0}, { 875, 0} },
        { { 925, 80}, { 875, 80}, { 875, 0}, { 925, 0} }
    };
   for (int j=0; j < 53; ++j) {
       textures[j] = bindTexture(QPixmap(QString("%1/%2.bmp").arg(resourcesPath,QString::number(j))), GL_TEXTURE_2D);
    }

    for (int i = 0; i < 13; ++i) {
        for (int j = 0; j < 4; ++j) {
            texCoords.append
                (QVector2D(j == 0 || j == 3, j == 0 || j == 1));
            vertices.append
                (QVector2D(coords[i][j][0]*screenScale, coords[i][j][1]*screenScale));
        }
    }
}
