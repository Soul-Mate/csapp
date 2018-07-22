.PHTONY: all

DIRS = shell

all: 
	for i in $(DIRS); do\
		(cd $$i echo "make $$d" && $(MAKE)) || exit 1; \
	done