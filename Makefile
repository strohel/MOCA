SUBDIRS = ukol-1 ukol-2

.PHONY: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@
