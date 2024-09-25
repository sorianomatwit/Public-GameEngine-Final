CC = g++
LDC = g++
LD_FLAGS = -std=c++20 -O2 -fopenmp -g
FLAGS = -std=c++20 -O2 -fopenmp -MMD -MP -g
NOWARN = -Wno-deprecated-declarations -Wno-enum-compare
PROG = prog.x
SRCDIR = src
OBJDIR = obj
BINDIR = bin
EXLIBDIR = external/lib
EXINCDIR = external/include
IMGUIDIR = external/imgui
EXDIR = external/
RM = /bin/rm

#platform raylib library 
uname_p := $(shell uname -p)
uname_s := $(shell uname -s)
ifeq ($(uname_s),Darwin)
ifeq ($(uname_p),arm)
RAYLIB=${EXLIBDIR}/libraylibARMmac.a
LDLIBS= -L./${EXLIBDIR} -lraylibARMmac
LDFRAMEWORKS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
else
RAYLIB=${EXLIBDIR}/libraylibx64mac.a
LDLIBS= -L./${EXLIBDIR} -lraylibx64mac
LDFRAMEWORKS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
endif
else ifeq ($(uname_s),Linux)
RAYLIB=${EXLIBDIR}/libraylibx64linux.a
LDLIBS= -L./${EXLIBDIR} -lraylibx64linux -ldl
LDFRAMEWORKS = 
endif

#for logging library (if there is one)
ifdef LOGLEVEL
FLAGS:=$(FLAGS) -DLOGLEVEL=$(LOGLEVEL)
LD_FLAGS:=$(LD_FLAGS) -DLOGLEVEL=$(LOGLEVEL)
endif

#using all of the above variables:
#setup generic vars for libraries, includes, and linker flags
LIBS=${RAYLIB}
INC=-I./ -I./${EXINCDIR} -I./${EXDIR}
LD_FLAGS:=${LD_FLAGS} $(LDLIBS) ${LDFRAMEWORKS}
#and setup all the needed files, objs, and sources
FILES = $(shell find $(SRCDIR)/*.cpp)
OBJS = $(FILES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
IMGUIFILES = $(shell find $(IMGUIDIR)/*.cpp)
OBJS := $(OBJS) $(IMGUIFILES:$(IMGUIDIR)/%.cpp=$(OBJDIR)/%.o)
DEPS = $(patsubst %.o,%.d,$(OBJS))


#########
#RULES
#########

#all the phonys
.PHONY: all clean view

#default rule
all: $(PROG)

#link rule
$(PROG): $(OBJS)
	$(LDC) ${LIBS} $^ $(LD_FLAGS)  -o $(BINDIR)/$(PROG)

#rule fo compiling src and imgui src
$(OBJDIR)/%.o:$(SRCDIR)/%.cpp
	$(CC) -c $< $(FLAGS) $(INC) $(NOWARN) -o $@
$(OBJDIR)/%.o:$(IMGUIDIR)/%.cpp
	$(CC) -c $< $(FLAGS) $(INC) $(NOWARN) -o $@

#include all dependency rules from *.d files in OBJDIR
-include $(DEPS)


clean:
	-$(RM) $(OBJDIR)/*.o $(OBJDIR)/*.d $(BINDIR)/$(PROG) $(BINDIR)/*.ini $(PROG) 2>/dev/null || true
	
view:
	@echo $(FILES)
	@echo $(IMGUIFILES)
	@echo $(OBJS)
