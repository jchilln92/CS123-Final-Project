#include "glwidget.h"

#include <iostream>
#include <QFileDialog>
#include <QGLFramebufferObject>
#include <QGLShaderProgram>
#include <QMouseEvent>
#include <QTime>
#include <QTimer>
#include <QThread>
#include <QWheelEvent>
#include "glm.h"

#include "gen/SystemGenerator.h"
#include "geom/Planet.h"
#include "geom/Orbit.h"
#include "lab/scene.h"

using std::cout;
using std::endl;

extern "C"
{
    extern void APIENTRY glActiveTexture(GLenum);
}

static const int MAX_FPS = 120;


/**
  Constructor.  Initialize all member variables here.
 **/
GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent),
    m_drawTimer(this), m_simTimer(this), m_prevTime(0), m_prevFps(0.f), m_fps(0.f),
    m_font("Deja Vu Sans Mono", 8, 4)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    connect(&m_drawTimer, SIGNAL(timeout()), this, SLOT(update()));
    connect(&m_simTimer, SIGNAL(timeout()), this, SLOT(doSimTick()));

    m_scene = new Scene();
    m_isBloomEnabled = true;
    m_isDOFEnabled = true;
}

/**
  Destructor.  Delete any 'new'ed objects here.
 **/
GLWidget::~GLWidget()
{
    foreach (QGLShaderProgram *sp, m_shaderPrograms)
        delete sp;
    foreach (QGLFramebufferObject *fbo, m_framebufferObjects)
        delete fbo;
    glDeleteLists(m_skybox, 1);
    const_cast<QGLContext *>(context())->deleteTexture(m_cubeMap);

    delete m_scene;
}

/**
  Initialize the OpenGL state and start the drawing loop.
 **/
void GLWidget::initializeGL()
{
    // Set up OpenGL
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glDisable(GL_DITHER);

    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Load resources, including creating shader programs and framebuffer objects
    initializeResources();

    // Start the drawing timer
    m_drawTimer.start(1000.0f / MAX_FPS);

    // Start the simulation timer
    m_simTimer.start(1.0f); // 1 tick = ~1ms
    m_prevSimTime = m_clock.elapsed();

    m_dof = 25.0f;
}

void GLWidget::doSimTick() {
    int time = m_clock.elapsed();
    int timeElapsed = time - m_prevSimTime;
    int numTicks = timeElapsed;
    m_prevSimTime = time;
    m_scene->doTicks(numTicks);
}

/**
  Initialize all resources.
  This includes models, textures, call lists, shader programs, and framebuffer objects.
 **/
void GLWidget::initializeResources()
{
    cout << "Using OpenGL Version " << glGetString(GL_VERSION) << endl << endl;
    // Ideally we would now check to make sure all the OGL functions we use are supported
    // by the video card.  But that's a pain to do so we're not going to.
    cout << "--- Loading Resources ---" << endl;

    m_skybox = ResourceLoader::loadSkybox();
    cout << "Loaded skybox..." << endl;

    SystemGenerator gen;
    QList<Planet> pls = gen.generate();
    for (int i = 0; i < pls.size(); i++) {
        m_scene->addBody(pls[i]);
    }
    cout << "Created system..." << endl;

    loadCubeMap();
    cout << "Loaded cube map..." << endl;

    createShaderPrograms();
    cout << "Loaded shader programs..." << endl;

    createFramebufferObjects(width(), height());
    cout << "Loaded framebuffer objects..." << endl;

    cout << " --- Finish Loading Resources ---" << endl;
}

/**
  Load a cube map for the skybox
 **/
void GLWidget::loadCubeMap()
{
    QList<QFile *> fileList;
    fileList.append(new QFile("../CS123-Final-Project/textures/space/posx.jpg"));
    fileList.append(new QFile("../CS123-Final-Project/textures/space/negx.jpg"));
    fileList.append(new QFile("../CS123-Final-Project/textures/space/posy.jpg"));
    fileList.append(new QFile("../CS123-Final-Project/textures/space/negy.jpg"));
    fileList.append(new QFile("../CS123-Final-Project/textures/space/posz.jpg"));
    fileList.append(new QFile("../CS123-Final-Project/textures/space/negz.jpg"));
    m_cubeMap = ResourceLoader::loadCubeMap(fileList);
}

/**
  Create shader programs.
 **/
void GLWidget::createShaderPrograms()
{
    const QGLContext *ctx = context();

    m_shaderPrograms["brightpass"] = ResourceLoader::newFragShaderProgram(ctx, "../CS123-Final-Project/shaders/brightpass.frag");
    m_shaderPrograms["blur"] = ResourceLoader::newFragShaderProgram(ctx, "../CS123-Final-Project/shaders/blur.frag");
    m_shaderPrograms["dof"] = ResourceLoader::newShaderProgram(ctx, "../CS123-Final-Project/shaders/dof.vert",
                                                                    "../CS123-Final-Project/shaders/dof.frag");
}

/**
  Allocate framebuffer objects.
  @param width: the viewport width
  @param height: the viewport height
 **/
void GLWidget::createFramebufferObjects(int width, int height)
{
    // Allocate the main framebuffer object for rendering the scene to
    // This needs a depth attachment
    m_framebufferObjects["no_bind"] = new QGLFramebufferObject(width, height, QGLFramebufferObject::Depth,
                                                             GL_TEXTURE_2D, GL_RGB16F_ARB);
    m_framebufferObjects["no_bind"]->format().setSamples(16);
    // Allocate the secondary framebuffer obejcts for rendering textures to (post process effects)
    // These do not require depth attachments
    m_framebufferObjects["pass1"] = new QGLFramebufferObject(width, height, QGLFramebufferObject::NoAttachment,
                                                             GL_TEXTURE_2D, GL_RGB16F_ARB);
    m_framebufferObjects["pass1_2"] = new QGLFramebufferObject(width, height, QGLFramebufferObject::Depth,
                                                             GL_TEXTURE_2D, GL_RGB16F_ARB);
    // TODO: Create another framebuffer here.  Look up two lines to see how to do this... =.=
    m_framebufferObjects["dof_applied"] = new QGLFramebufferObject(width, height, QGLFramebufferObject::NoAttachment,
                                                             GL_TEXTURE_2D, GL_RGB16F_ARB);
    m_framebufferObjects["bp_applied"] = new QGLFramebufferObject(width, height, QGLFramebufferObject::NoAttachment,
                                                             GL_TEXTURE_2D, GL_RGB16F_ARB);
}

/**
  Called to switch to an orthogonal OpenGL camera.
  Useful for rending a textured quad across the whole screen.

  @param width: the viewport width
  @param height: the viewport height
**/
void GLWidget::applyOrthogonalCamera(float width, float height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0.f, -1.f, 1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/**
  Called to switch to a perspective OpenGL camera.

  @param width: the viewport width
  @param height: the viewport height
**/
void GLWidget::applyPerspectiveCamera(float width, float height)
{
    // set up projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(55, (float)width / (float)height, 0.01, 1000);

    // set up modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    m_camera.multMatrix();
}

/**
  Draws the scene to a buffer which is rendered to the screen when this function exits.
 **/
void GLWidget::paintGL()
{
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update the fps
    int time = m_clock.elapsed();
    m_fps = 1000.f / (time - m_prevTime);
    m_prevTime = time;
    int width = this->width();
    int height = this->height();

    // draw scene
    //applyPerspectiveCamera(width, height);
    //renderScene();
    //paintText();

    //---------------------------------
    // Old Framebuffer code, might be useful later

    // Render the scene to a framebuffer
    m_framebufferObjects["no_bind"]->bind();
    applyPerspectiveCamera(width, height);
    renderScene(false);
    m_framebufferObjects["no_bind"]->release();

    m_framebufferObjects["pass1_2"]->bind();
    applyPerspectiveCamera(width, height);
    renderScene(true);
    m_framebufferObjects["pass1_2"]->release();

    // Copy the rendered scene into framebuffer 1
    m_framebufferObjects["no_bind"]->blitFramebuffer(m_framebufferObjects["pass1"],
                                                   QRect(0, 0, width, height), m_framebufferObjects["no_bind"],
                                                   QRect(0, 0, width, height), GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glDisable(GL_LIGHTING);

    applyOrthogonalCamera(width, height);
    m_framebufferObjects["dof_applied"]->bind();
    glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_shaderPrograms["dof"]->bind();
    m_shaderPrograms["dof"]->setUniformValue("width", (GLfloat)width);
    m_shaderPrograms["dof"]->setUniformValue("height", (GLfloat)height);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_framebufferObjects["pass1"]->texture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_framebufferObjects["pass1_2"]->texture());
    m_shaderPrograms["dof"]->setUniformValue("tex", 0);
    m_shaderPrograms["dof"]->setUniformValue("depthTex", 1);
    renderTexturedQuad(width, height, true);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    m_shaderPrograms["dof"]->release();
    m_framebufferObjects["dof_applied"]->release();

    if (m_isDOFEnabled) {
        glBindTexture(GL_TEXTURE_2D, m_framebufferObjects["dof_applied"]->texture());
    } else {
        glBindTexture(GL_TEXTURE_2D, m_framebufferObjects["pass1"]->texture());
    }

    renderTexturedQuad(width, height, true);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (m_isBloomEnabled) {
        m_framebufferObjects["bp_applied"]->bind();
        m_shaderPrograms["brightpass"]->bind();
        glBindTexture(GL_TEXTURE_2D, m_framebufferObjects["dof_applied"]->texture());
        renderTexturedQuad(width, height, true);
        m_shaderPrograms["brightpass"]->release();
        glBindTexture(GL_TEXTURE_2D, 0);
        m_framebufferObjects["bp_applied"]->release();

        float scales[] = {4.f,8.f,16.f,32.f,64.f,128.f,256.f};
        for (int i = 0; i < 7; ++i)
        {
            // Render the blurred brightpass filter result to fbo 1
            renderBlur(width / scales[i], height / scales[i]);

            // Bind the image from fbo to a texture
            glBindTexture(GL_TEXTURE_2D, m_framebufferObjects["pass1"]->texture());
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            // Enable alpha blending and render the texture to the screen this with your image editor, or programmatically. For the latter approach, you need a gradient function that returns a colour given a number between 0 and 1. This function is then called for every element in you Perlin noise array to obtain a colour, which you can store in a separate array, from which an image can be created.
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);
            glTranslatef(0.f, (scales[i] - 1) * -height, 0.f);
            renderTexturedQuad(width * scales[i], height * scales[i], false);
            glDisable(GL_BLEND);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    paintText();
    glEnable(GL_LIGHTING);
}

/**
  Renders the scene.  May be called multiple times by paintGL() if necessary.
**/
void GLWidget::renderScene(bool depthPass) {
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Enable cube maps and draw the skybox
    glEnable(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);

    // Disable the lighting, draw the skybox, and enable the lighting
    glDisable(GL_LIGHTING);
    glCallList(m_skybox);
    glEnable(GL_LIGHTING);

    glEnable(GL_CULL_FACE);

    glPolygonMode(GL_FRONT, GL_FILL);

    // draw the scene
    m_scene->render(depthPass, m_dof);

    // Disable culling, depth testing and cube maps
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_CUBE_MAP,0);
    glDisable(GL_TEXTURE_CUBE_MAP);
}

/**
  Run a gaussian blur on the texture stored in fbo 2 and
  put the result in fbo 1.  The blur should have a radius of 2.

  @param width: the viewport width
  @param height: the viewport height
**/
void GLWidget::renderBlur(int width, int height)
{
    int radius = 2;
    int dim = radius * 2 + 1;
    GLfloat kernel[dim * dim];
    GLfloat offsets[dim * dim * 2];
    createBlurKernel(radius, width, height, &kernel[0], &offsets[0]);

    m_framebufferObjects["pass1"]->bind();
    m_shaderPrograms["blur"]->bind();
    m_shaderPrograms["blur"]->setUniformValue("arraySize", dim * dim);
    m_shaderPrograms["blur"]->setUniformValueArray("offsets", offsets, dim * dim, 2);
    m_shaderPrograms["blur"]->setUniformValueArray("kernel", kernel, dim * dim * 2, 1);
    glBindTexture(GL_TEXTURE_2D, m_framebufferObjects["bp_applied"]->texture());
    renderTexturedQuad(width, height, false);
    m_shaderPrograms["blur"]->release();
    glBindTexture(GL_TEXTURE_2D, 0);
    m_framebufferObjects["pass1"]->release();
}

/**
  Called when the mouse is dragged.  Rotates the camera based on mouse movement.
**/
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    Vector2 pos(event->x(), event->y());
    m_camera.mouseMove(pos - m_prevMousePos, event->buttons());
    m_prevMousePos = pos;
}

/**
  Record a mouse press position.
 **/
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_prevMousePos.x = event->x();
    m_prevMousePos.y = event->y();
}

/**
  Called when the mouse wheel is turned.  Zooms the camera in and out.
**/
void GLWidget::wheelEvent(QWheelEvent *event)
{
    if (event->orientation() == Qt::Vertical) {
        m_camera.mouseWheel(event->delta());
        m_scene->updateBodyDetails(&m_camera); // recalculate level of detail for planets
    }
}

/**
  Called when the GLWidget is resized.
 **/
void GLWidget::resizeGL(int width, int height)
{
    // Resize the viewport
    glViewport(0, 0, width, height);

    // Reallocate the framebuffers with the new window dimensions
    foreach (QGLFramebufferObject *fbo, m_framebufferObjects)
    {
        const QString &key = m_framebufferObjects.key(fbo);
        QGLFramebufferObjectFormat format = fbo->format();
        delete fbo;
        m_framebufferObjects[key] = new QGLFramebufferObject(width, height, format);
    }
}

/**
  Draws a textured quad. The texture most be bound and unbound
  before and after calling this method - this method assumes that the texture
  has been bound before hand.

  @param w: the width of the quad to draw
  @param h: the height of the quad to draw
  @param flip: flip the texture vertically
**/
void GLWidget::renderTexturedQuad(int width, int height, bool flip) {
    // Clamp value to edge of texture when texture index is out of bounds
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Draw the  quad
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, flip ? 1.0f : 0.0f);
    glVertex2f(0.0f, 0.0f);
    glTexCoord2f(1.0f, flip ? 1.0f : 0.0f);
    glVertex2f(width, 0.0f);
    glTexCoord2f(1.0f, flip ? 0.0f : 1.0f);
    glVertex2f(width, height);
    glTexCoord2f(0.0f, flip ? 0.0f : 1.0f);
    glVertex2f(0.0f, height);
    glEnd();
}

/**
  Creates a gaussian blur kernel with the specified radius.  The kernel values
  and offsets are stored.

  @param radius: The radius of the kernel to create.
  @param width: The width of the image.
  @param height: The height of the image.
  @param kernel: The array to write the kernel values to.
  @param offsets: The array to write the offset values to.
**/
void GLWidget::createBlurKernel(int radius, int width, int height,
                                                    GLfloat* kernel, GLfloat* offsets)
{
    int size = radius * 2 + 1;
    float sigma = radius / 3.0f;
    float twoSigmaSigma = 2.0f * sigma * sigma;
    float rootSigma = sqrt(twoSigmaSigma * M_PI);
    float total = 0.0f;
    float xOff = 1.0f / width, yOff = 1.0f / height;
    int offsetIndex = 0;
    for (int y = -radius, idx = 0; y <= radius; ++y)
    {
        for (int x = -radius; x <= radius; ++x,++idx)
        {
            float d = x * x + y * y;
            kernel[idx] = exp(-d / twoSigmaSigma) / rootSigma;
            total += kernel[idx];
            offsets[offsetIndex++] = x * xOff;
            offsets[offsetIndex++] = y * yOff;
        }
    }
    for (int i = 0; i < size * size; ++i)
    {
        kernel[i] /= total;
    }
}

/**
  Handles any key press from the keyboard
 **/
void GLWidget::keyPressEvent(QKeyEvent *event)
{
    SystemGenerator gen;
    QList<Planet> pls;

    switch(event->key())
    {
        case Qt::Key_B:
            m_isBloomEnabled = !m_isBloomEnabled;
            break;
        case Qt::Key_D:
            m_isDOFEnabled = !m_isDOFEnabled;
            break;
        case Qt::Key_P:
            if (m_simTimer.isActive()) {
                m_simTimer.stop();
            } else {
                m_prevSimTime = m_clock.elapsed();
                m_simTimer.start(1.0f);
            }
            break;
        case Qt::Key_R:
            m_scene->clear();
            pls = gen.generate();
            for (int i = 0; i < pls.size(); i++) {
                m_scene->addBody(pls[i]);
            }
            break;
        case Qt::Key_J:
            m_dof *= 0.98;
            break;
        case Qt::Key_K:
            m_dof *= 1.02;
            break;
        case Qt::Key_S:
            QImage qi = grabFrameBuffer(false);
            QString filter;
            QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("PNG Image (*.png)"), &filter);
            qi.save(QFileInfo(fileName).absoluteDir().absolutePath() + "/" + QFileInfo(fileName).baseName() + ".png", "PNG", 100);
            break;
    }
}

/**
  Draws text for the FPS and screenshot prompt
 **/
void GLWidget::paintText()
{
    glColor3f(1.f, 1.f, 1.f);

    // Combine the previous and current framerate
    if (m_fps >= 0 && m_fps < 1000)
    {
       m_prevFps *= 0.95f;
       m_prevFps += m_fps * 0.05f;
    }

    // QGLWidget's renderText takes xy coordinates, a string, and a font
    renderText(10, 20, "FPS: " + QString::number((int) (m_prevFps)), m_font);
    renderText(10, 35, "S: Save screenshot", m_font);
    renderText(10, 50, "P: Toggle animation", m_font);
    renderText(10, 65, "R: Regenerate system", m_font);
    renderText(10, 90, "Options:", m_font);

    QString bloomString = "B: Toggle Bloom";
    bloomString += m_isBloomEnabled ? " (On)" : " (Off)";
    QString dofString = "D: Toggle DOF";
    dofString += m_isDOFEnabled ? " (On)" : " (Off)";

    renderText(10, 105, bloomString, m_font);
    renderText(10, 120, dofString, m_font);

    renderText(10, 145, "Current focus depth: " + QString::number(m_dof), m_font);
    renderText(10, 160, "J: Lower focus depth", m_font);
    renderText(10, 175, "K: Raise focus depth", m_font);

    glColor3f(1.f, 1.f, 1.f);
}
