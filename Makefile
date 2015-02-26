DIRS = SimpleJSONServerClient
BUILD-DIRS = $(DIRS:%=build-%)

all: $(BUILD-DIRS)
build-%: %
	$(MAKE) -C $<

