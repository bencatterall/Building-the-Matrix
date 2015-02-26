DIRS = SimpleJSONServerClient
BUILD-DIRS = $(DIRS:%=build-%)

all: $(BUILD-DIRS)
test:
build-%: %
	$(MAKE) -C $<

