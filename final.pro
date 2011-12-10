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
    noise/NoiseGenerator.h \
    noise/PerlinNoise.h \
    geom/Sphere.h \
    geom/Shape.h \
    geom/Planet.h \
    geom/MeshDetail.h \
    lab/scene.h \
    geom/Orbit.h
SOURCES += lab/glwidget.cpp \
    lib/targa.cpp \
    lib/glm.cpp \
    support/resourceloader.cpp \
    support/mainwindow.cpp \
    support/main.cpp \
    support/camera.cpp \
    noise/NoiseGenerator.cpp \
    noise/PerlinNoise.cpp \
    geom/Sphere.cpp \
    geom/Shape.cpp \
    geom/Planet.cpp \
    lab/scene.cpp \
    geom/Orbit.cpp
FORMS += mainwindow.ui \
    support/mainwindow.ui
OTHER_FILES += shaders/refract.vert \
    shaders/refract.frag \
    shaders/reflect.vert \
    shaders/reflect.frag \
    shaders/brightpass.frag \
    shaders/blur.frag \
    shaders/terrain.vert \
    shaders/terrain.frag
RESOURCES += 
