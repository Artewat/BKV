libcyusb.so.1: libcyusb.cpp
	g++ -fPIC -o libcyusb.o -c libcyusb.cpp
	g++ -shared -Wl,-soname,libcyusb.so -o libcyusb.so.1 libcyusb.o -l usb-1.0 -l rt
	ln -sf libcyusb.so.1 libcyusb.so
	rm -f libcyusb.o

.PHONY: clean
clean:
	rm -f libcyusb.so libcyusb.so.1

