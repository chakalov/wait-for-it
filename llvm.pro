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
    scope.cpp

OTHER_FILES += \
    CMakeLists.txt \
    test1.txt \
    test2.txt \
    test3.txt \
    main.txt \
    test4.txt \
    test5.txt \
    test6.txt \
    test7.txt \
    test8.txt \
    test9.txt \
    test10.txt \
    test11.txt

HEADERS += \
    lexer.h \
    token.h \
    parser.h \
    ast.h \
    scope.h

