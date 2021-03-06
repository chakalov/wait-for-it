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
    scope.cpp \
    ast/doublenumberexpr.cpp \
    ast/int32numberexpr.cpp \
    ast/stringexpr.cpp \
    ast/localvariableexpr.cpp \
    ast/expr.cpp \
    ast/globalvariableexpr.cpp \
    ast/variableexpr.cpp \
    ast/identifierexpr.cpp \
    ast/functionargumentexpr.cpp \
    ast/binaryexpr.cpp \
    ast/callexpr.cpp \
    ast/functionprototypeexpr.cpp \
    ast/blockexpr.cpp \
    ast/functionexpr.cpp \
    ast/ifexpr.cpp \
    ast/whileexpr.cpp \
    ast/infiniteexpr.cpp

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
    test11.txt \
    main2.txt \
    testwitherrors1.txt \
    exampleerror2.txt \
    main_rec.txt \
    main_exam.txt \
    main2_wrong_operator.txt

HEADERS += \
    lexer.h \
    token.h \
    parser.h \
    ast.h \
    scope.h \
    ast/expr.h \
    ast/numberexpr.h \
    ast/literalexpr.h \
    ast/doublenumberexpr.h \
    ast/int32numberexpr.h \
    ast/stringexpr.h \
    ast/variableexpr.h \
    ast/localvariableexpr.h \
    ast/globalvariableexpr.h \
    ast/identifierexpr.h \
    ast/functionargumentexpr.h \
    ast/binaryexpr.h \
    ast/callexpr.h \
    ast/functionprototypeexpr.h \
    ast/blockexpr.h \
    ast/functionexpr.h \
    ast/ifexpr.h \
    ast/whileexpr.h \
    ast/infiniteexpr.h

