CXX = icpc
CXXFLAGS = -g -O2 -shared-intel -debug inline-debug-info -DTBB_USE_THREADING_TOOLS -qopenmp -qopenmp-link dynamic -parallel-source-info=2
KNL = -gxx-name=gcc-9.3.0
HEADERS = ../core/*.h ../include/*.h

.PHONY: clean

APPS = PSS_v5_LG_degree_reorder
#APPS = middle4.app multi_queries.app simple_v3_profile_cache_miss.app simple_v3_single_thread.app

all: $(APPS)

PSS_v5_LG_degree_reorder: ../app/PSS_v5_LG_degree_reorder.cpp ../core/Searching.202103152056.PSS_v5.large_graph.degree_reorder.cpp $(HEADERS)
	$(CXX) -o $@ $< ../core/Searching.202103152056.PSS_v5.large_graph.degree_reorder.cpp $(CXXFLAGS)

#middle4.app: ../app/profile_para_single_query_top_m_search_middle_m.cpp $(HEADERS)
#	$(CXX) -o $@ $< $(CXXFLAGS) $(KNL)
#
#multi_queries.app: ../app/profile_para_multi_queries_top_m_search_vtune.cpp $(HEADERS)
#	$(CXX) -o $@ $< $(CXXFLAGS) $(KNL)
#
#simple_v3_profile_cache_miss.app: ../app/profile_para_single_query_search_simple_v3_profile_cache_miss.cpp ../core/Searching.202009021917.simple_v3.profile.cpp $(HEADERS)
#	$(CXX) -o $@ $< ../core/Searching.202009021917.simple_v3.profile.cpp $(CXXFLAGS) $(KNL)
#
#simple_v3_single_thread.app: ../app/profile_para_single_query_search_simple_v3.cpp ../core/Searching.202008310636.simple_v3.cpp $(HEADERS)
#	$(CXX) -o $@ $< ../core/Searching.202008310636.simple_v3.cpp $(CXXFLAGS) $(KNL)

clean:
	rm -f $(APPS)