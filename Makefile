.PHONY: all src

all: src

src:
	$(MAKE) -C ./src

clean:
	$(MAKE) -C ./src clean