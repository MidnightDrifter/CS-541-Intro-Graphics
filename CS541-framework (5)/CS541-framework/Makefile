########################################################################
# Makefile for Linux

CXXFLAGS = -I. -g -I../glsdk/glm -I../glsdk/boost -I../glsdk/glimg/include -I../glsdk/freeglut/include -I../glsdk/glload/include -I/usr/X11R6/include/GL/ -I/usr/include/GL/
LIBS =  -pthread -L/usr/lib  -L/usr/local/lib -lAntTweakBar -lfreeglut -lX11 -lGLU -lGL -L/usr/X11R6/lib -L../glsdk/glimg/lib/ -L../glsdk/glload/lib/ -L../glsdk/freeglut/lib/ -lglload -lglimg
target = framework.exe

src1 = framework.cpp models.cpp scene.cpp shader.cpp texture.cpp fbo.cpp transform.cpp
src2 = rply.c
headers = scene.h shader.h texture.h fbo.h models.h rply.h AntTweakBar.h transform.h
extras = framework.vcxproj Makefile AntTweakBar.dll AntTweakBar.lib images
models = ~/assets/mesh/bunny.ply ~/assets/mesh/dragon.ply
shaders = lighting.frag lighting.vert

pkgFiles = $(src1) $(src2) $(shaders) $(headers) $(extras)

objects = $(patsubst %.cpp,%.o,$(src1)) $(patsubst %.c,%.o,$(src2)) 

$(target): $(objects)
	@echo Link $(target)
	g++ -g  -o $@  $(objects) $(LIBS)

%.o: %.cpp
	@echo Compile $<
	@$(CXX) -c -std=c++11 $(CXXFLAGS) $< -o $@

%.o: %.c
	@echo Compile $<
	@$(CC) -c $(CXXFLAGS) $< -o $@

run: $(target)
	./$(target)

zip562: $(pkgFiles)
	rm -rf ../CS562-framework ../CS562-framework.zip
	mkdir ../CS562-framework
	cp -r $(pkgFiles) ../CS562-framework
	mkdir ../CS562-framework/models
	cp $(models) ../CS562-framework/models
	cp -r ../glsdk  ../CS562-framework
	cd ..;  zip -r CS562-framework.zip CS562-framework; rm -rf CS562-framework

zip541: $(pkgFiles)
	rm -rf ../CS541-framework ../CS541-framework.zip
	mkdir ../CS541-framework
	cp -r $(pkgFiles) ../CS541-framework
	mkdir ../CS541-framework/models
	cp $(models) ../CS541-framework/models
	cp -r ../glsdk ../CS541-framework
	cd ..;  zip -r CS541-framework.zip CS541-framework; rm -rf CS541-framework

zip300: $(pkgFiles)
	rm -rf ../CS300-framework ../CS300-framework.zip
	mkdir ../CS300-framework
	cp -r $(pkgFiles) ../CS300-framework
	mkdir ../CS300-framework/models
	cp $(models) ../CS300-framework/models
	cp -r ../glsdk ../CS300-framework
	cd ..;  zip -r CS300-framework.zip CS300-framework; rm -rf CS300-framework

clean:
	rm -f *.o *~ framework dependencies

ws:
	unix2dos $(src1) $(src2) $(shaders) $(headers) $(extras)
	@echo
	@echo ========= TABS:
	@grep -P '\t' $(src1) $(src2) $(shaders) $(headers) $(extras)

dependencies: 
	g++ -MM $(CXXFLAGS)  $(src1) > dependencies

include dependencies
