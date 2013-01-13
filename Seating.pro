FORMS += \
    seatdisplay.ui \
    booking.ui \
    waiting.ui \
    adminpanel.ui \
    login.ui \
    disabilityEditor.ui

HEADERS += \
    seatdisplay.h

SOURCES += \
    main.cpp \
    seatdisplay.cpp

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    .conf
QT += sql
#CONFIG += release
INCLUDEPATH+=C:\\PROGRA~1\\MySQL\\MYSQLS~1.5\\include
LIBS+=C:\\PROGRA~1\\MySQL\\MYSQLS~1.5\\lib\\libmysql.lib
