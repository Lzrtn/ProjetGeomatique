copydata.commands = $(COPY_DIR) $$PWD/Docker $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

TEMPLATE = subdirs

SUBDIRS += test_db \
        googletest
