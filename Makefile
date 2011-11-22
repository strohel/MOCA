SUBDIRS = ukol-1 ukol-2 ukol-3

.PHONY: all $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@
