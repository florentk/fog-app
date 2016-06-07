XERCES_LIBS = -l$(LIB_XERCES)

bin_PROGRAMS = itsfogapp

COMMON_LIBS = libitsfogapp.a \
./utils/options/liboptions.a \
./utils/common/libcommon.a \
./utils/xml/libxml.a \
./utils/iodevices/libiodevices.a \
./foreign/tcpip/libtcpip.a \
./utils/log/liblog.a \
$(XERCES_LIBS)

noinst_LIBRARIES = libitsfogapp.a

libitsfogapp_a_SOURCES = server.h server.cpp \
app-commands-subscriptions-constants.h \
application-logic.h application-logic.cpp

itsfogapp_SOURCES = fog_app_main.cpp 

itsfogapp_LDFLAGS = $(XERCES_LDFLAGS)

itsfogapp_LDADD = $(COMMON_LIBS)

SUBDIRS = utils foreign

EXTRA_DIST = config.h
