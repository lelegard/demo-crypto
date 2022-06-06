OS   := $(shell uname -s | tr A-Z a-z)
EXE  := $(if $(filter, cygwin%, $(OS)),.exe)
EXECS = encrypt-image$(EXE) prime-count$(EXE)

MAGICK_CXXFLAGS = $(shell Magick++-config --cppflags --cxxflags)
MAGICK_LDFLAGS  = $(filter-out, -l%, $(shell Magick++-config --ldflags))
MAGICK_LDLIBS   = $(shell Magick++-config --libs)

default: $(EXECS)

encrypt-image$(EXE): CXXFLAGS += $(MAGICK_CXXFLAGS)
encrypt-image$(EXE): LDFLAGS += $(MAGICK_LDFLAGS)
encrypt-image$(EXE): LDLIBS += -lcrypto $(MAGICK_LDLIBS)
prime-count$(EXE): LDLIBS += -lm

images: encrypt-image$(EXE)
	./encrypt-image$(EXE) secret.gif

primes: prime-count$(EXE)
	./prime-count$(EXE) 512

clean:
	rm -rf $(EXECS) *.gif.rgb.*.jpg *.gif.rgba.*.jpg \
	    *~ *.o *.ko *.a *.gcov *.dep *.tmp *.out *.log *.tlog core core.* vgcore.* *.stackdump \
	    [Dd]ebug* [Rr]elease* ipch *.exe *.obj *.so .kdbgrc.* *.t2d *.ncb *.suo *.idb *.sdf *.opensdf \
	    *.vcproj.*.user *.vcxproj.user *.dll.embed.manifest.dll *.dll.embed.manifest.ilk .tmp_versions
