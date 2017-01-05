#include "GLPlayBridge.h"
#include "CoreFoundation/CoreFoundation.h"
#include <string>

using namespace std;

GLPlayBridge::GLPlayBridge(QWidget *parent, QGLWidget *shareWidget): QGLWidget(parent, shareWidget)
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
        card[i]=0; hand_card[i]=true;
    }
    for (int i=0; i<4; ++i) {
        table[i]=0;
    }
    hand_poker=13;
    choose=false;
    turn=false;
    now_flower=-1;
    screenScale = devicePixelRatio();
#ifdef QT_OPENGL_ES_2
    program = 0;
#endif
}

GLPlayBridge::~GLPlayBridge()
{
}

QSize GLPlayBridge::minimumSizeHint() const
{
    return QSize(1010, 520);
}

QSize GLPlayBridge::sizeHint() const
{
    return QSize(1010, 520);
}

void GLPlayBridge::setClearColor(const QColor &color)
{
    clearColor = color;
    updateGL();
}

void GLPlayBridge::setCard(int *temp)
{
    for(int i=0;i<13;++i)
        card[i]=temp[i];
}

void GLPlayBridge::setTable(int *temp)
{
    for(int i=0;i<4;++i)
        table[i]=temp[i];
    updateGL();
}

void GLPlayBridge::setTurn(bool _turn)
{
    turn=_turn;
}

void GLPlayBridge::setMyturnid(int _myturnid)
{
    myturnid=_myturnid;
}

void GLPlayBridge::setNowFlower(int _flower)
{
    now_flower=_flower;
}

void GLPlayBridge::again()
{
    hand_poker=13;
    choose=false;
    turn=false;
    now_flower=-1;
    
    for (int i=0; i<13; ++i) {
        hand_card[i]=true;
    }
    //因為在出牌時將點從vertices[]一個一個刪除掉了
    //所以再玩一次時需重新定義
    static const int coords[13][4][2] = {
        { { 180, 165}, { 120, 165}, { 120, 0}, { 180, 0} },
        { { 240, 165}, { 180, 165}, { 180, 0}, { 240, 0} },
        { { 300, 165}, { 240, 165}, { 240, 0}, { 300, 0} },
        { { 360, 165}, { 300, 165}, { 300, 0}, { 360, 0} },
        { { 420, 165}, { 360, 165}, { 360, 0}, { 420, 0} },
        { { 480, 165}, { 420, 165}, { 420, 0}, { 480, 0} },
        { { 540, 165}, { 480, 165}, { 480, 0}, { 540, 0} },
        { { 600, 165}, { 540, 165}, { 540, 0}, { 600, 0} },
        { { 660, 165}, { 600, 165}, { 600, 0}, { 660, 0} },
        { { 720, 165}, { 660, 165}, { 660, 0}, { 720, 0} },
        { { 780, 165}, { 720, 165}, { 720, 0}, { 780, 0} },
        { { 840, 165}, { 780, 165}, { 780, 0}, { 840, 0} },
        { { 900, 165}, { 840, 165}, { 840, 0}, { 900, 0} }
    };
    
    for (int i = 0; i < 13; ++i) {
        for (int j = 0; j < 4; ++j) {
            texCoords.append
            (QVector2D(j == 0 || j == 3, j == 0 || j == 1));
            vertices.append
            (QVector2D(coords[i][j][0]*screenScale, coords[i][j][1]*screenScale));
        }
    }

}

void GLPlayBridge::initializeGL()
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

void GLPlayBridge::paintGL()
{
    qglClearColor(clearColor);
    glClear(GL_COLOR_BUFFER_BIT);

#if !defined(QT_OPENGL_ES_2)

    glLoadIdentity();

	glVertexPointer(3, GL_FLOAT, 0, vertices.constData());
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords.constData());
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
#else

    QMatrix4x4 m;
    m.gluOrtho2D(0, 1010, 0, 520);
    program->setUniformValue("matrix", m);
    program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    program->setAttributeArray
        (PROGRAM_VERTEX_ATTRIBUTE, vertices.constData());
    program->setAttributeArray
        (PROGRAM_TEXCOORD_ATTRIBUTE, texCoords.constData());

#endif
    //畫table~~~
    glBindTexture(GL_TEXTURE_2D, textures[table[0]]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,0.0f);
    glVertex2i(480*screenScale, 188*screenScale);
    glTexCoord2f(1.0f,0.0f);
    glVertex2i(540*screenScale, 188*screenScale);
    glTexCoord2f(1.0f,1.0f);
    glVertex2i(540*screenScale, 353*screenScale);
    glTexCoord2f(0.0f,1.0f);
    glVertex2i(480*screenScale, 353*screenScale);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textures[table[1]]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,0.0f);
    glVertex2i(419*screenScale, 270*screenScale);
    glTexCoord2f(1.0f,0.0f);
    glVertex2i(479*screenScale, 270*screenScale);
    glTexCoord2f(1.0f,1.0f);
    glVertex2i(479*screenScale, 435*screenScale);
    glTexCoord2f(0.0f,1.0f);
    glVertex2i(419*screenScale, 435*screenScale);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textures[table[2]]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,0.0f);
    glVertex2i(480*screenScale, 354*screenScale);
    glTexCoord2f(1.0f,0.0f);
    glVertex2i(540*screenScale, 354*screenScale);
    glTexCoord2f(1.0f,1.0f);
    glVertex2i(540*screenScale, 519*screenScale);
    glTexCoord2f(0.0f,1.0f);
    glVertex2i(480*screenScale, 519*screenScale);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textures[table[3]]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,0.0f);
    glVertex2i(541*screenScale, 270*screenScale);
    glTexCoord2f(1.0f,0.0f);
    glVertex2i(601*screenScale, 270*screenScale);
    glTexCoord2f(1.0f,1.0f);
    glVertex2i(601*screenScale, 435*screenScale);
    glTexCoord2f(0.0f,1.0f);
    glVertex2i(541*screenScale, 435*screenScale);
    glEnd();
    
    //設定上下家為player ? ASCII 49='1'
    std::string my_text,my_text_number(1,49+myturnid%4);
    std::string text1,text1_number(1,49+(myturnid+1+4)%4);
    std::string text2,text2_number(1,49+(myturnid-1+4)%4);
    my_text = "Player "+my_text_number+" :";
    text1 = "Player "+text1_number;
    text2 = "Player "+text2_number;
    drawText(my_text.data(),my_text.size(),80,200);
    drawText(text1.data(), text1.size(), 315, 510);
    drawText(text2.data(), text2.size(), 435, 510);

    //畫!
    for (int i = 0,j=0; i < hand_poker; ++i,++j) {
        while(hand_card[j]==false)  ++j;
        glBindTexture(GL_TEXTURE_2D, textures[card[j]]);
        glDrawArrays(GL_QUADS, i * 4, 4);
    }
}

void GLPlayBridge::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    #if !defined(QT_OPENGL_ES_2)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    #ifndef QT_OPENGL_ES
    gluOrtho2D(0, width, 0, height);
    #endif
    glMatrixMode(GL_MODELVIEW);
    #endif  
   
}

void GLPlayBridge::mousePressEvent(QMouseEvent *event)
{
    //將滑鼠按下的點存到lastPos
    lastPos = event->pos();
    
    int assess_x=lastPos.x()-120;

    //height = 520   so change star left-bottom (lastPos.x(),height-lastPos.y())
    
    //有出牌權才動作
    if(turn==true){
    //60為牌的寬度  因為已經-120 所以直接除60可得知滑鼠點到哪張牌
        
    // 13張牌之間&&高度介於牌的高度&&被指到的那張牌確定有在手上
        if((assess_x/60<13&&assess_x>0)&&
           ((520-lastPos.y())<165&&(520-lastPos.y())>0)&&hand_card[assess_x/60]==true&&hand_poker>0) {
            
            int k; //紀錄"指到的牌前面有沒有已打的牌"有的話要要減掉
            
            if(hand_poker<13) {
                int temp=0;
                //"指到的牌"之前 assess_x/60
                for(int j=0;j<assess_x/60;++j)
                {
                    //已打的牌(不在手上 false) temp記錄有幾張
                    if(hand_card[j]==false)
                        temp++;
                }
                //因為存在vertices為四點一張牌 所以乘四
                //因為釋放點坐標 所以中間空了多少點要用減的
                k=0+4*(assess_x/60-temp);
            }
            else
                //這好像沒關聯  不過我懶的測試了 就放者 XD
                k=0+4*(assess_x/60);
            
            //k介於vertices有值的範圍 避免發生沒有預期到的事當掉
            if(k<vertices.size()&&k>=0)
                for(int i=0;i<4;++i,++k)
                {
                    QVector3D temp = vertices.at(k);
                    //若已經向上了(提示選取了那張牌) 則不動作直接跳出
                    if(temp.y()==185) break;
                    
                    //沒有則四個點都推高20單位
                    temp.setY((temp.y())+20);
                    
                    //放回vertices
                    vertices.replace(k,temp);
                    
                    //注意有++k
                }
            //已選擇了一張牌所以true
            choose=true;
        }
    }
    //更新畫面
    updateGL();
}

void GLPlayBridge::mouseReleaseEvent(QMouseEvent *event)
{
    //height = 520   so change star left-bottom (lastPos.x(),height-lastPos.y())

    // 因為已經-120 所以直接除60可得知滑鼠點到哪張牌
    int assess_x=event->pos().x()-120;
    bool have=false; //記錄有無犯規
    int k;//紀錄"指到的牌前面有沒有已打的牌"有的話要要減掉
    if(turn==true){
    
    if(hand_poker<13) {
        int temp=0;
        //"指到的牌"之前 assess_x/60
        for(int j=0;j<assess_x/60;++j)
        {
             //已打的牌(不在手上 false) temp記錄有幾張  
            if(hand_card[j]==false)
                temp++;
        }
        //因為存在vertices為四點一張牌 所以乘四
        //因為釋放點坐標 所以中間空了多少點要用減的
        k=0+4*((lastPos.x()-120)/60-temp);
    }
    else
        //這好像也沒關聯  不過我懶的測試了 就放者 XD
        k=0+4*((lastPos.x()-120)/60);
    
        //若now_flower!=-1代表已經設定那一輪是什麼花色先打了
        if(now_flower!=-1)
        for(int i=0;i<13;++i)
        {
            //because K = 13 26 39 52  so we have to -1
            if((card[i]-1)/13==now_flower&&hand_card[i])
            {
                //have=true 表示client手上有同樣花色的牌
                have=true;
                break;
            }
        }
 
        //Release的點(x)小於13&&Release與Press的點是指向同一張牌&&Release的點(y)為推高20單位&&
        //Press的點(y)為原本高度&&指到的那張牌真的存在(hand_card)&&沒有同樣花色的牌(!=now_flower)
        if(((assess_x/60<13&&assess_x/60==((lastPos.x()-120)/60))&&
            ((520-event->pos().y())>20&&(520-event->pos().y())<185)&&520-lastPos.y()<165&&520-lastPos.y()>0)&&hand_card[assess_x/60]==true&&event->x()>120&&!have)
        {
            //k介於vertices有值的範圍 避免發生沒有預期到的事當掉
            if(k<vertices.size()&&k>=0)
                for (int i=0; i<4; ++i) {
                    //從vertices移除掉
                    vertices.remove(k);
                }
            //打掉的那張牌 設為false
            hand_card[(lastPos.x()-120)/60]=false;
            //手牌減一
            hand_poker--;
            //將自己的出牌權設為false
            turn = false;
            //將出牌的資訊傳回MainWindow
            emit leadPoker(card[(lastPos.x()-120)/60]);
        }
        //有同樣花色的牌(==now_flower)&&"指到的那張"的花色==now_flower&&Release的點(y)為推高20單位&&
        //Press的點(y)為原本高度&&指到的那張牌真的存在(hand_card)&&Release的點(x)小於13&&
        //Release與Press的點是指向同一張牌
        else if(have&&(card[(lastPos.x()-120)/60]-1)/13==now_flower&&(520-event->pos().y())>20&&(520-event->pos().y())<185&&520-lastPos.y()<165&&520-lastPos.y()>0&&hand_card[assess_x/60]==true&&assess_x/60<13&&assess_x/60==((lastPos.x()-120)/60))
        {   //k介於vertices有值的範圍 避免發生沒有預期到的事當掉
            if(k<vertices.size()&&k>=0)
                for (int i=0; i<4; ++i) {
                    //從vertices移除掉
                    vertices.remove(k);
                }
            //打掉的那張牌 設為false
            hand_card[(lastPos.x()-120)/60]=false;
            //手牌減一
            hand_poker--;
            //將自己的出牌權設為false
            turn = false;
            //將出牌的資訊傳回MainWindow
            emit leadPoker(card[(lastPos.x()-120)/60]);
        }
        //Press的牌存在&&已選定牌&&手上有牌
        else if(hand_card[(lastPos.x()-120)/60]==true&&choose==true&&hand_poker>0)
        {   //k介於vertices有值的範圍 避免發生沒有預期到的事當掉
            if(k<vertices.size()&&k>=0)
                for(int i=0;i<4;++i,++k)
                {
                    QVector3D temp = vertices.at(k);
                    //若為原本高度(未出牌) 則不動作直接跳出
                    if (temp.y()==165) break;
                    
                    //沒有則四個點都推高20單位
                    temp.setY((temp.y())-20);
                    //放回vertices
                    vertices.replace(k,temp);
                    
                }
            //並設定為未選定牌狀態
            choose=false;
        }
    }
    updateGL();
}

void GLPlayBridge::makeObject()
{
    static const int coords[13][4][2] = {
        { { 180, 165}, { 120, 165}, { 120, 0}, { 180, 0} },
        { { 240, 165}, { 180, 165}, { 180, 0}, { 240, 0} },
        { { 300, 165}, { 240, 165}, { 240, 0}, { 300, 0} },
        { { 360, 165}, { 300, 165}, { 300, 0}, { 360, 0} },
        { { 420, 165}, { 360, 165}, { 360, 0}, { 420, 0} },
        { { 480, 165}, { 420, 165}, { 420, 0}, { 480, 0} },
        { { 540, 165}, { 480, 165}, { 480, 0}, { 540, 0} },
        { { 600, 165}, { 540, 165}, { 540, 0}, { 600, 0} },
        { { 660, 165}, { 600, 165}, { 600, 0}, { 660, 0} },
        { { 720, 165}, { 660, 165}, { 660, 0}, { 720, 0} },
        { { 780, 165}, { 720, 165}, { 720, 0}, { 780, 0} },
        { { 840, 165}, { 780, 165}, { 780, 0}, { 840, 0} },
        { { 900, 165}, { 840, 165}, { 840, 0}, { 900, 0} }
    };

    for (int j=0; j < 54; ++j) {
        textures[j] = bindTexture
        (QPixmap(QString("%1/%2.bmp").arg(resourcesPath,QString::number(j))), GL_TEXTURE_2D);
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

void GLPlayBridge::drawText(const char *text, int length, int x, int y){
    glMatrixMode(GL_PROJECTION); // change the current matrix to PROJECTION
    double *matrix = new double[16]; // allocate 16-byte in memory
    glGetDoublev(GL_PROJECTION_MATRIX, matrix); // get the values from PROJECTION matrix to local variable
    glLoadIdentity(); // reset PROJECTION matrix to identity matrix
    glOrtho(0, 800, 0, 600, -5, 5); // orthographic perspective
    glMatrixMode(GL_MODELVIEW); // change current matrix to MODELVIDE matrix again
    glLoadIdentity(); // reset it to identity matrix
    glPushMatrix(); // push current state of MODELVIEW matrix to stack
    glLoadIdentity(); // reset it again. (may not be required, but it my convention
    glRasterPos2i(x, y); // raster position in 2D
    for(int i=0; i<length; i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)text[i]); // generation of characters in our text with TIMES_ROMAN 24
    }
    glPopMatrix(); // get MODELVIEW matrix value from stack
    glMatrixMode(GL_PROJECTION); // change current matrix mode to PROJECTION
    glLoadMatrixd(matrix); // reset
    glMatrixMode(GL_MODELVIEW); // change current matrix mode to MODELVIEW
}
