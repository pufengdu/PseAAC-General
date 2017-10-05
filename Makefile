.PHONY: all clean pseb bmv bmc install
all: pseb bmv bmc

pseb:
	@cd pseb && $(MAKE) $@
bmv:
	@cd bmv && $(MAKE) $@
bmc:
	@cd bmc && $(MAKE) $@
clean:
	@cd pseb && $(MAKE) $@
	@cd bmv && $(MAKE) $@
	@cd bmc && $(MAKE) $@
	@if [ -d ./bin ] ; then \
		rm -rf ./bin ; \
	fi

install: all
	@if [ ! -d ./bin ] ; then \
		mkdir "./bin" ; \
	fi
	@if [ -e ./pseb/pseb ] ; then \
		cp ./pseb/pseb ./bin ; \
	else \
		echo "PSEB Build Error!" ; \
	fi
	@if [ -e ./bmc/bmc ] ; then \
		cp ./bmc/bmc ./bin ; \
	else \
		echo "Binary Module Compiler Error!" ; \
	fi
	@if [ -e ./bmv/bmv ] ; then \
		cp ./bmv/bmv ./bin ; \
	else \
		echo "Binary Module Viewer Error!" ; \
	fi

