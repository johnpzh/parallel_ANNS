CXX = icpc
CXXFLAGS = -g -O2 -shared-intel -debug inline-debug-info -DTBB_USE_THREADING_TOOLS -qopenmp -qopenmp-link dynamic -parallel-source-info=2
#KNL = -gxx-name=gcc-9.3.0
HEADERS = ../core/*.h ../include/*.h

.PHONY: clean

APPS = only_gather.app middle4.app multi_queries.app interval_merge.app

all: $(APPS)

only_gather.app: ../app/profile_para_single_query_top_m_search_selecting_out.only_gather_top_m.cpp  $(HEADERS)
	$(CXX) -o $@ $< $(CXXFLAGS) $(KNL)

middle4.app: ../app/profile_para_single_query_top_m_search_middle_m.cpp $(HEADERS)
	$(CXX) -o $@ $< $(CXXFLAGS) $(KNL)

multi_queries.app: ../app/profile_para_multi_queries_top_m_search_vtune.cpp $(HEADERS)
	$(CXX) -o $@ $< $(CXXFLAGS) $(KNL)

interval_merge.app: ../app/profile_para_single_query_top_m_search_interval_merge.cpp $(HEADERS)
	$(CXX) -o $@ $< $(CXXFLAGS) $(KNL)

clean:
	rm -f $(APPS)