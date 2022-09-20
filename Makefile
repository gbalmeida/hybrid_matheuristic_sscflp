CFLAGS= -Wall -m64 -g -w
#CFLAGS= -Wall -m64 -o2 
CXX=g++
ILOG=/opt/ibm/ILOG/CPLEX_Studio221   
CPPFLAGS= -DIL_STD -I$(ILOG)/cplex/include -I$(ILOG)/concert/include
CPLEXLIB=-L$(ILOG)/cplex/lib/x86-64_linux/static_pic -lilocplex -lcplex -L$(ILOG)/concert/lib/x86-64_linux/static_pic -lconcert -lm -lpthread

comp-sscflp:  
	$(CXX) $(CFLAGS) $(CPPFLAGS) -o sscflp sscflp.cpp  $(CPLEXLIB) -ldl
run:comp-sscflp
	./sscflp
clean:
	rm -f  *.out *.aux *.log *.nav *.snm *.out *.toc 
