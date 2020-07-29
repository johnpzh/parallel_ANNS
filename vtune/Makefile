CXX = icpc
CXXFLAGS = -g -O2 -shared-intel -debug inline-debug-info -DTBB_USE_THREADING_TOOLS -qopenmp -qopenmp-link dynamic -parallel-source-info=2

HEADERS = ../core/*.h ../include/*.h

.PHONY: clean

APPS = only_gather.app middle4.app

all: $(APPS)

only_gather.app: ../app/profile_para_single_query_top_m_search_selecting_out.only_gather_top_m.cpp  $(HEADERS)
	$(CXX) -o $@ $< $(CXXFLAGS)

middle4.app: ../app/profile_para_single_query_top_m_search_middle_m.cpp $(HEADERS)
	$(CXX) -o $@ $< $(CXXFLAGS)

clean:
	rm -f $(APPS)