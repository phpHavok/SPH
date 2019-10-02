SRC:=src
BIN:=bin
TARGET:=$(BIN)/sph.out
PPN?=1
CC:=mpicc
LDFLAGS:=-L$(SDKSTAGE)/opt/vc/lib/ -lGLESv2 -lGLEW -lEGL -lopenmaxil -lbcm_host -lvcos -lvchiq_arm -lpthread -lrt -L../libs/ilclient -L../libs/vgfont -lfreetype
INCLUDES:=-I$(SDKSTAGE)/opt/vc/include/ -I$(SDKSTAGE)/opt/vc/include/interface/vcos/pthreads -I$(SDKSTAGE)/opt/vc/include/interface/vmcs_host/linux -I$(SRC)/ -I../libs/ilclient -I../libs/vgfont -I/usr/include/freetype2
CCFLAGS:=-DRASPI -mfloat-abi=hard -mfpu=vfp -O3 -lm -ffast-math -g
SRC_COMMON:=ogl_utils.c egl_utils.c dividers_gl.c liquid_gl.c exit_menu_gl.c image_gl.c cursor_gl.c rectangle_gl.c lodepng.c background_gl.c font_gl.c particles_gl.c mover_gl.c controls.c renderer.c geometry.c hash.c communication.c fluid.c
SRC_BLINK:=blink1_light.c
SRC_LIGHT:=rgb_light.c
SRC_COMMON:=$(addprefix $(SRC)/,$(SRC_COMMON))
SRC_BLINK:=$(addprefix $(SRC)/,$(SRC_BLINK))
SRC_LIGHT:=$(addprefix $(SRC)/,$(SRC_LIGHT))

blink: $(SRC_COMMON) $(SRC_BLINK)| $(BIN)
	$(CC) $(CCFLAGS) $(INCLUDES) -DBLINK1 -DUSE_HIDAPI $^ $(LDFLAGS) -lblink1 -o $(TARGET)

vanilla: $(SRC_COMMON) | $(BIN)
	$(CC) $(CCFLAGS) $(INCLUDES) $^ $(LDFLAGS) -o $(TARGET)

light: $(SRC_COMMON) $(SRC_LIGHT) | $(BIN)
	$(CC) $(CCFLAGS) $(INCLUDES) -DLIGHT $^ $(LDFLAGS) -o $(TARGET)

leap: $(SRC_COMMON) $(SRC_BLINK)| $(BIN)
	$(CC) $(CCFLAGS) $(INCLUDES) -DBLINK1 -DUSE_HIDAPI -DLEAP_MOTION_ENABLED1 $^ $(LDFLAGS) -lblink1 -lcurl -o $(TARGET)


$(BIN):
	mkdir $@

clean:
	rm -rf $(BIN)
	rm -f *.o
	rm -f $(SRC)/*.o

run:
	./run.sh $(PPN)

.PHONY: default light blink leap clean run
