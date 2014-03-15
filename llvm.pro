TEMPLATE = app
TARGET = wait_for_it
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += "/usr/include/llvm-3.5/"
INCLUDEPATH += "/usr/include/llvm-c-3.5/"

SOURCES += main.cpp \
    lexer.cpp \
    parser.cpp \
    ast.cpp \
    table.cpp

OTHER_FILES += \
    CMakeLists.txt \
    test1.txt \
    test2.txt \
    test3.txt

HEADERS += \
    lexer.h \
    token.h \
    parser.h \
    ast.h \
    table.h

