CP=cp -rf
RM=rm -f
RMDIR=rm -rf
MKDIR=mkdir -p
NAME=communication
LIBNAME=lib$(NAME)
VER=0.3-0
TARGET=usr
PREFIX=/$(TARGET)
HEADERS=../Connection.h ../Rest.h ../Config.h
PKG_DIR=Communication
PACKAGE=$(LIBNAME)-$(VER).deb

all: package

package:
	$(MKDIR) $(PKG_DIR)/$(TARGET)/lib
	$(MKDIR) $(PKG_DIR)/$(TARGET)/include/$(PKG_DIR)
	$(CP) ../$(LIBNAME).so* $(PKG_DIR)/$(TARGET)/lib
	$(CP) $(HEADERS) $(PKG_DIR)/$(TARGET)/include/$(PKG_DIR)
	dpkg-deb --build $(PKG_DIR)
	mv $(PKG_DIR).deb $(PACKAGE)

clean:
	$(RM) $(PACKAGE)
	$(RMDIR) $(PKG_DIR)/$(TARGET)
