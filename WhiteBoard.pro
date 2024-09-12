QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets pdf pdfwidgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ItemLayer/BaseGraphicsItem.cpp \
    ItemLayer/ControlGroupObserver.cpp \
    ItemLayer/ControlPointGroup.cpp \
    ItemLayer/ControlPointItem.cpp \
    SceneLayer/BackgroundItem.cpp \
    SceneLayer/BasicCommand.cpp \
    SceneLayer/WhiteBoardScene.cpp \
    ViewLayer/WhiteBoardView.cpp \
    main.cpp \
    WhiteBoard.cpp

HEADERS += \
    ItemLayer/BaseGraphicsItem.h \
    ItemLayer/ControlGroupObserver.h \
    ItemLayer/ControlPointGroup.h \
    ItemLayer/ControlPointItem.h \
    ItemLayer/ItemShaper.h \
    SceneLayer/BackgroundItem.h \
    SceneLayer/BasicCommand.h \
    SceneLayer/WhiteBoardScene.h \
    ViewLayer/WhiteBoardView.h \
    WhiteBoard.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc \
    res.qrc

DISTFILES += \
    BackgroundImages/1.jpg \
    BackgroundImages/2.png \
    BackgroundImages/3.png \
    README.md \
    我的白板项目规划/Item层次结构设计.md \
    我的白板项目规划/Item层次结构设计/Item中的抽象概念.md \
    我的白板项目规划/Item层次结构设计/assets/image-20240903151455-02next7.png \
    我的白板项目规划/Item层次结构设计/assets/image-20240903152136-n0wan2c.png \
    我的白板项目规划/Item层次结构设计/assets/image-20240903154118-37169lv.png \
    我的白板项目规划/Item层次结构设计/assets/image-20240903154133-l1lbdgr.png \
    我的白板项目规划/Item层次结构设计/assets/image-20240903155028-frinnu4.png \
    我的白板项目规划/Item层次结构设计/assets/image-20240903155059-k13ttol.png \
    我的白板项目规划/Item层次结构设计/assets/image-20240903155832-j5fr53p.png \
    我的白板项目规划/Item层次结构设计/assets/image-20240903160433-30wdvvx.png \
    我的白板项目规划/Item层次结构设计/assets/image-20240903161647-gxmzsgt.png \
    我的白板项目规划/Item层次结构设计/assets/image-20240904191425-19yhhbz.png \
    我的白板项目规划/Item层次结构设计/assets/image-20240904193021-2c4ws9i.png \
    我的白板项目规划/Qt画图板.md \
    我的白板项目规划/Scene层次结构设计.md \
    我的白板项目规划/View层次结构设计.md \
    我的白板项目规划/assets/image-20240828135405-4hoghi2.png \
    我的白板项目规划/assets/image-20240828141029-h8hregj.png \
    我的白板项目规划/assets/image-20240901112508-mxc7eb4.png \
    我的白板项目规划/assets/image-20240905220900-ojdecsd.png \
    我的白板项目规划/assets/image-20240905230403-od1puha.png \
    我的白板项目规划/assets/image-20240907112245-c7zwi82.png \
    我的白板项目规划/assets/image-20240907141115-vgv2auz.png \
    我的白板项目规划/assets/image-20240907151359-qrzu356.png \
    我的白板项目规划/assets/image-20240907151505-mcbbpd6.png \
    我的白板项目规划/白板项目界面总体设计.md \
    我的白板项目规划/白板项目需求分析.md \
    我的白板项目规划/设计阶段对设计的验证和测试.md
