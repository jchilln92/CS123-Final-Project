QT += core gui opengl

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
    geom/MeshDetail.h \
    geom/Planet.h \
    math/CS123Vector.h \
    math/CS123Matrix.h \
    math/CS123Algebra.h \
    lib/CS123Common.h

SOURCES += \
    lab/glwidget.cpp \
    lib/targa.cpp \
    lib/glm.cpp \
    support/resourceloader.cpp \
    support/mainwindow.cpp \
    support/main.cpp \
    support/camera.cpp \
    geom/Sphere.cpp \
    geom/Shape.cpp \
    geom/Planet.cpp \
    math/CS123Vector.inl \
    math/CS123Matrix.inl \
    math/CS123Matrix.cpp

FORMS += mainwindow.ui \
    support/mainwindow.ui

OTHER_FILES += \
    shaders/refract.vert \
    shaders/refract.frag \
    shaders/reflect.vert \
    shaders/reflect.frag \
    shaders/brightpass.frag \
    shaders/blur.frag

RESOURCES +=
