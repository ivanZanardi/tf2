FCOMP = gfortran
FCFLAGS = -g -O3
LDFLAGS = -I${PYCOMET_INC} -I${PYCOMET_MOD} -L${PYCOMET_LIB} -lpycomet
PROGRAM = test.x
SRCS = test.f90
OBJECTS = $(SRCS:.f90=.o)

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(FCOMP) $(FCFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.f90
	$(FCOMP) $(FCFLAGS) -c $< $(LDFLAGS)

.PHONY: clean

clean:
	rm -f *.o *.mod *.MOD *~ $(PROGRAM)
