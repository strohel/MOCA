SUBDIRS = ukol-1

.PHONY: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@
