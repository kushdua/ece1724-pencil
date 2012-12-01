TARGET = QImageComparator

INCPATH = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtOpenGL -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Iliblinux -I/usr/X11R6/include
LIBS = -L/usr/lib -L/usr/X11R6/lib -Lliblinux -lming -lpng -lQtXml -lQtOpenGL -lQtGui -lQtCore -lGLU -lGL

$(TARGET): main.o
	g++ $(INCPATH) $(LIBS) main.o -o $(TARGET)
	
main.o: main.cpp
	g++ $(INCPATH) $(LIBS) -c main.cpp

all: $(TARGET)

clean:
	rm *.o $(TARGET)