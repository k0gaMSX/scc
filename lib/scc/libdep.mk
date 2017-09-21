LIB-OBJ = $(LIBDIR)/debug.o \
          $(LIBDIR)/die.o \
          $(LIBDIR)/newitem.o \
          $(LIBDIR)/xcalloc.o \
          $(LIBDIR)/xmalloc.o \
          $(LIBDIR)/xrealloc.o \
          $(LIBDIR)/xstrdup.o \
          $(LIBDIR)/alloc.o \

all:

$(LIBDIR)/libscc.a: $(LIB-OBJ)
	cd $(LIBDIR) && $(MAKE)
