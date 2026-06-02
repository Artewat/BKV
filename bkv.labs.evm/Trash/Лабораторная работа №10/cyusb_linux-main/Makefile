help:
	@echo	'make all	build the library and gui'
	@echo	'make lib	build the library'
	@echo	'make gui	build the cyusb gui'
	@echo	'make clean	remove the library'
	@echo	'make install	install everything (must be called as root)'
	@echo	'make uninstall	uninstall everything (must be called as root)'
	@echo	'make deb	build a debian package'

.PHONY: all
all: lib gui cli

.PHONY: lib
lib:
	$(MAKE) -C lib

.PHONY: gui
gui:
	cd gui_src && qmake && make

.PHONY: cli
cli:
	$(MAKE) -C src

.PHONY: clean
clean:
	rm -f lib/libcyusb.so lib/libcyusb.so.1
	rm -f bin/cyusb
	$(MAKE) -C gui_src clean
	$(MAKE) -C src clean

.PHONY: install
install:
	@if [ `whoami` != 'root' ]; then echo "You have to be root to run this script"; exit 1; fi
	-rm -f /usr/lib/libcyusb.so* /usr/local/lib/libcyusb.so*
	install -m644 lib/libcyusb.so.1 /usr/local/lib
	cd /usr/local/lib; ln -sf libcyusb.so.1 libcyusb.so
	install configs/cy_renumerate.sh /usr/local/bin
	install bin/cyusb /usr/local/bin/
	install src/download_fx2 /usr/local/bin
	install -m644 configs/cyusb.conf /etc/
	install -m644 configs/88-cyusb.rules /etc/udev/rules.d/
	echo "# Cypress USB Suite: Root directory" > /etc/profile.d/cyusb
	echo "export CYUSB_ROOT=`pwd`" >> /etc/profile.d/cyusb

.PHONY: uninstall
uninstall:
	@if [ `whoami` != 'root' ]; then echo "You have to be root to run this script"; exit 1; fi
	-rm -f /etc/cyusb.conf
	-rm -f /etc/udev/rules.d/88-cyusb.rules
	-rm -f /usr/lib/libcyusb.so* /usr/local/lib/libcyusb.so*
	-rm -f /usr/local/bin/cyusb
	-rm -f /usr/local/bin/download_fx2
	-rm -f /usr/local/bin/cy_renumerate.sh
	-rm -f /etc/profile.d/cyusb


.PHONY: deb
deb: all
	fakeroot checkinstall --pkgname cyusb --pkgversion 1.0.5 --pkggroup electronics --default --install=no --backup=no --deldoc=yes

.PHONY: CHANGELOG
CHANGELOG:
	git log --pretty='format:%as: %s [%h]' > CHANGELOG
