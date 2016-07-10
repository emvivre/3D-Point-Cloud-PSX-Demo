# ----------------------------
# PlayStation 1 Psy-Q MAKEFILE
# ----------------------------
CXXFLAGS=-Ilib/ -O3
all:
    del *.cpe
    del *.obj
    ccpsx $(CXXFLAGS) -c lib/mm.cpp -o mm.obj
    ccpsx $(CXXFLAGS) -c sphere.cpp -o sphere.obj
    ccpsx $(CXXFLAGS) -c ptcloud.cpp -o ptcloud.obj
    ccpsx $(CXXFLAGS) -c bishop.c -o bishop.obj
    ccpsx $(CXXFLAGS) -c cube.cpp -o cube.obj
    ccpsx $(CXXFLAGS) -c intro_msg.cpp -o intro_msg.obj
	ccpsx $(CXXFLAGS) -c credits.cpp -o credits.obj
	ccpsx $(CXXFLAGS) -c lib/text.cpp -o text.obj
	ccpsx $(CXXFLAGS) -c credit_part0.c -o c0.obj
	ccpsx $(CXXFLAGS) -c credit_part1.c -o c1.obj
	ccpsx $(CXXFLAGS) -c main.cpp -o main.obj
	ccpsx $(CXXFLAGS) -c lib/font.cpp -o font.obj 
	ccpsx $(CXXFLAGS) -c lib/trigo.c -o trigo.obj	
	psxlink
    waitfile main.cpe	
    cpe2x /ce main.cpe	
	psxrun main.exe
	
	

