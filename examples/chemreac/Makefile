FCOMP = gfortran
FCFLAGS = -g -O3
LDFLAGS = -I${TF2_INC} -I${TF2_MOD} -L${TF2_LIB} -ltf2
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
