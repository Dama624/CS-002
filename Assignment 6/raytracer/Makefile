CC = g++
DOCSGEN = doxygen
CFLAGS = -Wall -ansi -pedantic -ggdb -std=c++11 `sdl-config --cflags`  `xml2-config --cflags`
LIBS = -lm `sdl-config --libs` -lSDL_gfx  `xml2-config --libs` -lpthread
SRCDIR = src
OBJDIR = obj
BINDIR = bin
DOCSDIR = docs
OBJS = $(OBJDIR)/Material.o $(OBJDIR)/PointLight.o $(OBJDIR)/Ray.o \
	$(OBJDIR)/RaytracerApp.o $(OBJDIR)/RaytracerMultithreaded.o \
	$(OBJDIR)/RaytracerSinglethreaded.o $(OBJDIR)/Shader.o \
	$(OBJDIR)/Sphere.o $(OBJDIR)/Viewport.o $(OBJDIR)/World.o \
	$(OBJDIR)/XMLSceneParser.o

.PHONY: docs clean memcheck callgrind

raytracer: $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(BINDIR)/raytracer

$(OBJDIR)/Material.o: $(SRCDIR)/Material.cpp $(SRCDIR)/Material.h \
    $(SRCDIR)/structs.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/Material.cpp -o $(OBJDIR)/Material.o

$(OBJDIR)/Plane.o: $(SRCDIR)/Plane.cpp $(SRCDIR)/Plane.h \
    $(SRCDIR)/Entity.h $(SRCDIR)/Ray.h $(SRCDIR)/Material.h \
    $(SRCDIR)/structs.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/Plane.cpp -o $(OBJDIR)/Plane.o

$(OBJDIR)/PointLight.o: $(SRCDIR)/PointLight.cpp \
    $(SRCDIR)/PointLight.h $(SRCDIR)/Light.h $(SRCDIR)/structs.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/PointLight.cpp -o $(OBJDIR)/PointLight.o

$(OBJDIR)/Ray.o: $(SRCDIR)/Ray.cpp $(SRCDIR)/Ray.h \
    $(SRCDIR)/structs.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/Ray.cpp -o $(OBJDIR)/Ray.o

$(OBJDIR)/RaytracerApp.o: $(SRCDIR)/RaytracerApp.cpp \
    $(SRCDIR)/RaytracerApp.h $(SRCDIR)/RaytracerBase.h \
    $(SRCDIR)/Shader.h $(SRCDIR)/World.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/RaytracerApp.cpp -o $(OBJDIR)/RaytracerApp.o

$(OBJDIR)/RaytracerMultithreaded.o: $(SRCDIR)/RaytracerMultithreaded.cpp \
    $(SRCDIR)/RaytracerMultithreaded.h $(SRCDIR)/RaytracerBase.h \
    $(SRCDIR)/World.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/RaytracerMultithreaded.cpp -o $(OBJDIR)/RaytracerMultithreaded.o

$(OBJDIR)/RaytracerSinglethreaded.o: $(SRCDIR)/RaytracerSinglethreaded.cpp \
    $(SRCDIR)/RaytracerSinglethreaded.h $(SRCDIR)/RaytracerBase.h \
    $(SRCDIR)/Shader.h $(SRCDIR)/World.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/RaytracerSinglethreaded.cpp -o $(OBJDIR)/RaytracerSinglethreaded.o

$(OBJDIR)/Shader.o: $(SRCDIR)/Shader.cpp $(SRCDIR)/Shader.h \
    $(SRCDIR)/Ray.h $(SRCDIR)/structs.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/Shader.cpp -o $(OBJDIR)/Shader.o

$(OBJDIR)/Sphere.o: $(SRCDIR)/Sphere.cpp $(SRCDIR)/Sphere.h \
    $(SRCDIR)/Entity.h $(SRCDIR)/Ray.h $(SRCDIR)/structs.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/Sphere.cpp -o $(OBJDIR)/Sphere.o

$(OBJDIR)/Viewport.o: $(SRCDIR)/Viewport.cpp $(SRCDIR)/Viewport.h \
    $(SRCDIR)/structs.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/Viewport.cpp -o $(OBJDIR)/Viewport.o

$(OBJDIR)/World.o: $(SRCDIR)/World.cpp $(SRCDIR)/World.h \
    $(SRCDIR)/Ray.h $(SRCDIR)/Entity.h $(SRCDIR)/Light.h \
    $(SRCDIR)/Viewport.h $(SRCDIR)/structs.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/World.cpp -o $(OBJDIR)/World.o

$(OBJDIR)/XMLSceneParser.o: $(SRCDIR)/XMLSceneParser.cpp \
    $(SRCDIR)/XMLSceneParser.h $(SRCDIR)/World.h $(SRCDIR)/Entity.h \
	$(SRCDIR)/Sphere.h $(SRCDIR)/Light.h $(SRCDIR)/PointLight.h \
	$(SRCDIR)/structs.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/XMLSceneParser.cpp -o $(OBJDIR)/XMLSceneParser.o

clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/raytracer
	rm -rf $(DOCSDIR)/*

docs:
	$(DOCSGEN)

memcheck: $(OBJS)
	$(CC) $(CFLAGS) -c -DVALGRIND $(SRCDIR)/RaytracerApp.cpp -o $(OBJDIR)/RaytracerApp.o
	$(CC) $(OBJS) $(LIBS) -o $(BINDIR)/raytracer
	valgrind --tool=memcheck --leak-check=full $(BINDIR)/raytracer -m 2>&1 | tee memcheck.full

callgrind: $(OBJS)
	$(CC) $(CFLAGS) -c -DVALGRIND $(SRCDIR)/RaytracerApp.cpp -o $(OBJDIR)/RaytracerApp.o
	$(CC) $(OBJS) $(LIBS) -o $(BINDIR)/raytracer
	valgrind --tool=callgrind $(BINDIR)/raytracer -m
