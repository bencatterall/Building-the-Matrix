DIRS = SimpleJSONServerClient Building-the-Matrix
BUILD-DIRS = $(DIRS:%=build-%)

all: $(BUILD-DIRS)
test:
build-%: %
	$(MAKE) -C $<

