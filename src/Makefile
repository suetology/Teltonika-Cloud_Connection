CPPFLAGS = -I../deps/tuya_iot_sdk/include
LDFLAGS = -L../deps/tuya_iot_sdk/lib -Wl,-R../deps/tuya_iot_sdk/lib -llink_core -lmiddleware_implementation -lplatform_port -lutils_modules
DEPS = $(wildcard *.h)
SOURCE = $(wildcard *.c)
OBJ = $(SOURCE:.c=.o)

BIN:= cloud_connector

.PHONY: all clean 

all: $(BIN)

%.o: %.c $(DEPS)
	$(CC) -c $^ $(CPPFLAGS)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean: 
	$(RM) $(OBJ) $(BIN) *.gch
