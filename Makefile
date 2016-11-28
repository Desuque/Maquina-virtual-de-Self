all:
	sudo apt-get install libgtkmm-3.0-dev
	cd interfaz && $(MAKE)
	cd vm && $(MAKE)

PREFIX = /usr/local

.PHONY: install
install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp interfaz/client_self $(DESTDIR)$(PREFIX)/bin/self_client
	cp interfaz/dGlade.glade $(DESTDIR)$(PREFIX)/bin/dGlade.glade
	cp vm/server $(DESTDIR)$(PREFIX)/bin/self_server

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/self_client
	rm -f $(DESTDIR)$(PREFIX)/bin/self_server
	rm -f $(DESTDIR)$(PREFIX)/bin/dGlade
