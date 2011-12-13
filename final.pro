QT += core \
    gui \
    opengl
TARGET = final
TEMPLATE = app
INCLUDEPATH += lab \
    lib \
    math \
    support
DEPENDPATH += lab \
    lib \
    math \
    support
HEADERS += lab/glwidget.h \
    lib/targa.h \
    lib/glm.h \
    math/vector.h \
    support/resourceloader.h \
    support/mainwindow.h \
    support/camera.h \
    lib/targa.h \
    geom/Sphere.h \
    geom/Shape.h \
    geom/Planet.h \
    geom/MeshDetail.h \
    lab/scene.h \
    geom/Orbit.h \
    gen/SystemGenerator.h
SOURCES += lab/glwidget.cpp \
    lib/targa.cpp \
    lib/glm.cpp \
    support/resourceloader.cpp \
    support/mainwindow.cpp \
    support/main.cpp \
    support/camera.cpp \
    geom/Sphere.cpp \
    geom/Shape.cpp \
    geom/Planet.cpp \
    lab/scene.cpp \
    geom/Orbit.cpp \
    gen/SystemGenerator.cpp
FORMS += mainwindow.ui \
    support/mainwindow.ui
OTHER_FILES += shaders/refract.vert \
    shaders/refract.frag \
    shaders/reflect.vert \
    shaders/reflect.frag \
    shaders/brightpass.frag \
    shaders/blur.frag \
    shaders/terrain.vert \
    shaders/terrain.frag \
    shaders/star.vert \
    shaders/star.frag \
    shaders/dof.frag
RESOURCES += 
