/*
*    int_kidney_exchange
*    shapley+nucleolus.cpp
*    Purpose: computational study for
*             Benedek et al. (2021) - Computing Balanced Solutions for
*             Large Kidney Exchange Schemes
*             https://arxiv.org/abs/2109.06788
*             Shapley value and nucleolus allocations with equal country sizes
*
*    @author Xin Ye
*    @version 1.0 09/10/2023
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program. If not, see:
*    <https://github.com/blrzsvrzs/int_kidney_exchange>.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <lemon/list_graph.h> // needed for ListDigraph
#include <lemon/matching.h>
#include <lemon/adaptors.h>
#include <lemon/core.h>
#include <lemon/base.cc>
#include <time.h>
#include <glpk.h>
#include <iomanip>

#include <math.h>
#include <stdio.h>
//#include "windows.h"
//#include "psapi.h"
#include <iterator>
#include "gurobi_c++.h"


using namespace lemon;
using namespace std;

double cpuTime();
bool is_next_char_digit(string& line, unsigned int l);
unsigned int char2uint(char& p);
void undi_lemon(unsigned int& m, vector<unsigned int>& arc_in, vector<unsigned int>& arc_out, vector<unsigned short int>& label_positions, ListGraph& g, ListDigraph& g_original, vector<ListGraph::Node>& c, vector<ListGraph::Node>& c_b, vector<ListDigraph::Node>& c_original, ListGraph::EdgeMap<double>& edge_card_weight, ListDigraph::ArcMap<unsigned short int>& arc_card_weight, unsigned short int& no_of_nodes);
void coop_game(ListGraph& g, vector<double>& v, unsigned int& S, vector<unsigned short int>& s, vector<ListGraph::Node>& c, vector<ListGraph::Node>& c_b, ListGraph::EdgeMap<double>& edge_card_weight, bool& dispy, unsigned short int& Vp, unsigned short int& N, ListGraph::NodeMap<bool>& active_nodes, vector<bool>& leaving, map<int, int>& numofMaxSolution, unsigned short int& Q, bool& arbitray_maximum, vector<pair<int, int>>& cycle_distri, map<int, int>& cycle_dis, double& game_generation, std::map<int, std::map<int, int>>& cycle_dis_arbitrary_period);
void xml_parser(string& line, vector<unsigned short int>& node_labels, vector<unsigned short int>& label_positions, vector<ListGraph::Node>& c, vector<ListGraph::Node>& c_b, vector<ListDigraph::Node>& c_original, unsigned short int& k, ListGraph& g, ListDigraph& g_original, vector<unsigned int>& arc_in, vector<unsigned int>& arc_out, unsigned int& m, unsigned short int& no_of_nodes);
void shapley(vector<double>& shapl, vector<double>& v, unsigned short int& n, unsigned int& S);
void de2bi_card(unsigned int& k, vector<bool>& a, unsigned short int& n, unsigned short int& card);
void de2bi(unsigned int& k, vector<bool>& a, unsigned short int& n);
void insertion_sort(vector<unsigned short int>& w, vector<double>& y, unsigned short int& N);

void nucl(bool& disp, unsigned short int& n, unsigned int& s, vector<double>& x, vector<double>& v, double& prec);
void zeros_mem(vector<bool>& a, unsigned short int& n, unsigned int& s, vector<unsigned short int>& zeros);
void excess_init(vector<double>& exc, vector<bool>& unsettled, vector<double>& x, vector<double>& v, unsigned int& s, unsigned short int& n, vector<unsigned short int>& zeros);
void nucl_comp(bool& disp, unsigned short int& n, unsigned int& s, vector<double>& excess, double& prec, vector<bool>& unsettled, unsigned short int& iter, unsigned int& piv, unsigned int& sr, double& t, vector<double>& x, vector<bool>& a, double& t1, vector<double>& singleton_bounds, bool& nlsu, double& min_satisfaction, vector<unsigned short int>& zeros);
void vec_min_uns(double& m, vector<double>& x, vector<bool>& unsettled, unsigned int& s);
void tight_coal2(vector<bool>& T2, vector<double>& x, vector<double>& singleton_bounds, double& prec, unsigned short int& n, vector<unsigned int>& T2_coord, vector<bool>& unsettled_p, unsigned short int& t2_size);
void tight_coal(vector<bool>& T, vector<double>& excess, double& epsi, double& prec, unsigned int& s, vector<unsigned int>& T_coord, vector<bool>& unsettled, unsigned int& t_size);
void pivot(double& epsi, unsigned int& s, vector<double>& excess, double& prec, unsigned short int& n, vector<bool>& a, vector<vector<double>>& Arref, vector<bool>& J, vector<bool>& unsettled, unsigned short int& rank, vector<double>& d, vector<double>& x, bool& disp, vector<vector<bool>>& Asettled, unsigned int& piv, unsigned int& sr_count, unsigned short int& iter, vector<bool>& unsettled_p, vector<double>& singleton_bounds, double& epsi_old, bool& nlsu, vector<unsigned short int>& zeros, vector<bool>& T, vector<unsigned int>& T_coord, vector<bool>& T2, vector<unsigned int>& T2_coord, unsigned int& t_size, unsigned short int& t2_size, vector<vector<bool>>& Atight, vector<vector<bool>>& Atight2, vector<bool>& U, vector<bool>& U2, double& min_satisfaction);
void subr_upd(vector<vector<double>>& Arref, vector<bool>& J, unsigned int& i, unsigned short int& n, double& prec, vector<bool>& U, vector<bool>& U2, unsigned int& sumt, unsigned int& sumt2, vector<bool>& t, vector<bool>& t2, vector<vector<bool>>& Atight, vector<vector<bool>>& Atight2, unsigned int& tight_size, unsigned short int& tight2_size, unsigned short int& rank, vector<bool>& unsettled, vector<vector<bool>>& Asettled, bool& disp, unsigned int& s, vector<unsigned int>& T_coord, vector<unsigned int>& T2_coord, double& epsi_old, double& epsi, vector<bool>& unsettled_p, bool& settled, glp_prob*& lp, vector<bool>& ar0pos);
void subroutine(vector<bool>& U, vector<bool>& U2, vector<vector<bool>>& Atight, vector<vector<bool>>& Atight2, vector<vector<double>>& Arref, vector<bool>& J, double& prec, unsigned short int& n, unsigned int& tight_size, unsigned short int& tight2_size, unsigned short int& rank, bool& disp, vector<vector<bool>>& Asettled, unsigned int& sr_count, bool& u, unsigned int& s, vector<unsigned int>& T_coord, vector<unsigned int>& T2_coord, vector<bool>& unsettled, double& epsi_old, double& epsi, vector<bool>& unsettled_p, bool& settled, bool& nlsu);
void step(vector<bool>& T, vector<bool>& T2, vector<bool>& unsettled, vector<bool>& unsettled_p, unsigned int& s, double& epsi, vector<double>& excess, vector<double>& d, unsigned short int& n, vector<double>& x, vector<double>& singleton_bounds, bool& disp, double& prec, vector<unsigned short int>& zeros, vector<unsigned int>& T_coord, unsigned int& t_size, vector<unsigned int>& T2_coord, unsigned short int& t2_size, vector<vector<bool>>& Atight, vector<vector<bool>>& Atight2, vector<bool>& U, vector<bool>& U2);
void imprdir(vector<double>& d, unsigned short int& n, unsigned int& t_size, unsigned short int& t2_size, vector<vector<bool>>& Atight, vector<vector<bool>>& Atight2, vector<bool>& U, vector<bool>& U2, unsigned short int& rank, vector<vector<bool>>& Asettled, bool& disp);
bool binrank2(vector<vector<double>>& Arref, vector<bool>& J, vector<bool>& b, unsigned short int& n, unsigned short int& rank);
bool binrank(vector<vector<double>>& Arref, vector<bool>& J, vector<bool>& b, unsigned short int& n, unsigned short int& rank);
void sc_vec_prod(vector<double>& y, double a, vector<double>& x);
void vec_subtract(vector<double>& z, vector<double>& x, vector<double>& y);
void sum_vecb(unsigned int& s, vector<bool>& x);

void initial_pairs(unsigned short int& Vp, unsigned short int& N, ListGraph::NodeMap<bool>& active_nodes, ListDigraph::NodeMap<bool>& active_nodes_original, vector<ListGraph::Node>& c, vector<ListGraph::Node>& c_b, vector<ListDigraph::Node>& c_original, unsigned short int& initialSize);
void arrival_times(vector<unsigned short int>& node_arrives, unsigned short int& Vp, unsigned short int& N, ListGraph::NodeMap<bool>& active_nodes, vector<ListGraph::Node>& c, unsigned short int& periods, bool& disp);

void period_0(unsigned short int& Q, vector<unsigned short int>& no_of_active_nodes, unsigned short int& N, vector<unsigned short int>& s, unsigned short int& Vp, vector<unsigned short int>& node_arrives, ListGraph::NodeMap<bool>& active_nodes, ListDigraph::NodeMap<bool>& active_nodes_original, vector<ListGraph::Node>& c, vector<ListGraph::Node>& c_b, vector<ListDigraph::Node>& c_original, vector<double>& credit);
void min_d_1(vector<unsigned short int>& node_arrives, ListGraph& g, ListDigraph& g_original, vector<pair<int, int>>& arc_pair, vector<bool>& leaving, ListGraph::NodeMap<bool>& active_nodes, ListDigraph::NodeMap<bool>& active_nodes_original, vector<ListGraph::Node>& c, vector<ListGraph::Node>& c_b, vector<ListDigraph::Node>& c_original, bool& disp, vector<unsigned short int>& no_of_active_nodes, unsigned short int& N, unsigned short int& Vp, unsigned short int& periods, bool& dispy, vector<unsigned short int>& s, unsigned short int& Q, vector<double>& v, bool& initial_nucl, vector<double>& target, vector<double>& credit, ListGraph::EdgeMap<double>& edge_card_weight, double& t0, vector<vector<unsigned short int>>& actual_alloc, vector<int>& nodeset, vector<pair<int, int>>& cycle_distri, vector<double>& d, double& M_total, double& d_total, bool& c_involved, map<int, int>& cycle_dis, map<int, int>& numofMaxSolution, bool& arbitray_maximum, unsigned short int& initialSize, unsigned int& S, double& core_100, long& negative_core, double& prec, vector<double>& init_alloc, double& d_c_total, unsigned short int inst, bool lex_min, double& max_d, double& game_generation, double& solution_concept_time, double& scenario_time, std::map<int, std::map<int, int>>& cycle_dis_period, std::map<int, std::map<int, int>>& cycle_dis_arbitrary_period);
double core_dist(vector<double>& x, vector<double>& v, unsigned short int& N, unsigned int& S);
void ILP_d1_gurobi(unsigned short int& Q, unsigned short int& N, ListDigraph& g_original, unsigned short int& Vp, vector<unsigned short int>& node_arrives, ListGraph::NodeMap<bool>& active_nodes, ListDigraph::NodeMap<bool>& active_nodes_original, vector<pair<int, int>>& arc_pair, vector<int>& nodeset, vector<vector<unsigned short int>>& actual_alloc, double& M, double& M_total, vector<unsigned short int>& s, vector<pair<int, int>>& cycle_distri, vector<double>& target, vector<bool>& leaving, vector<double>& d, double& d_total, bool& c_involved, vector<double>& credit, map<int, int>& cycle_dis, vector<double>& init_alloc, bool lex_min, unsigned short int inst, std::map<int, std::map<int, int>>& cycle_dis_period);
void pair_arcs(unsigned short int& Q, ListDigraph& g_original, vector<unsigned short int>& node_arrives, ListDigraph::NodeMap<bool>& active_nodes_original, vector<pair<int, int>>& arc_pair, vector<int>& nodeset);
void arbitraryMaximum(vector<unsigned short int>& node_arrives, ListGraph& g, ListDigraph& g_original, vector<pair<int, int>>& arc_pair, vector<bool>& leaving, ListGraph::NodeMap<bool>& active_nodes, ListDigraph::NodeMap<bool>& active_nodes_original, vector<ListGraph::Node>& c, vector<ListGraph::Node>& c_b, vector<ListDigraph::Node>& c_original, bool& disp, vector<unsigned short int>& no_of_active_nodes, unsigned short int& N, unsigned short int& Vp, unsigned short int& periods, bool& dispy, vector<unsigned short int>& s, unsigned short int& Q, vector<double>& v, bool& initial_nucl, vector<double>& target, vector<double>& credit, ListGraph::EdgeMap<double>& edge_card_weight, double& t0, vector<vector<unsigned short int>>& actual_alloc, vector<int>& nodeset, vector<pair<int, int>>& cycle_distri, vector<double>& d, double& M_total, double& d_total, bool& c_involved, map<int, int>& cycle_dis, map<int, int>& numofMaxSolution, bool& arbitray_maximum, unsigned short int& initialSize, unsigned int& S, double& core_100, long& negative_core, double& prec, vector<double>& init_alloc, unsigned short int inst, double& max_d, double& game_generation, double& solution_concept_time, std::map<int, std::map<int, int>>& cycle_dis_arbitrary_period);
void changing_nodes(ListGraph::NodeMap<bool>& active_nodes, ListDigraph::NodeMap<bool>& active_nodes_original, vector<bool>& leaving, vector<unsigned short int>& no_of_active_nodes, unsigned short int& N, unsigned short int& Vp, vector<unsigned short int>& node_arrives, unsigned short int& Q, vector<ListGraph::Node>& c, vector<ListGraph::Node>& c_b, vector<ListDigraph::Node>& c_original, vector<unsigned short int>& s, vector<double>& d, vector<double>& target);
void cycle_distribution(std::map<int, std::map<int, int>>& cycle_dis_period, map<int, int>& cycle_dis, vector<pair<int, int>>& cycle_distri, unsigned short int& N, unsigned short int& Q);
void lex_min_d_star(vector<double>& d_t, bool& lex_min, unsigned short int& t_star, unsigned short int& N, long& col_num, double& epsilon, unsigned short int& n_star, GRBModel& model, vector<int>& ia, vector<int>& ja, vector<double>& ar, const unsigned short int& row_num, long& cnt_2, vector<double>& bound, vector<int>& nodeset, vector<unsigned short int>& N_star, unsigned short int& Vp, vector<pair<int, int>>& arc_pair, vector<double>& target, vector<double>& credit, vector<GRBVar>& var_lexmin, vector<GRBModel>& vector_model, unsigned short int& track);
void lex_min_n_star(vector<double>& d_t, bool& lex_min, unsigned short int& t_star, unsigned short int& N, long& col_num, double& epsilon, unsigned short int& n_star, GRBModel& model, vector<int>& ia, vector<int>& ja, vector<double>& ar, const unsigned short int& row_num, long& cnt_2, vector<double>& bound, vector<int>& nodeset, vector<unsigned short int>& N_star, vector<GRBVar>& var_lexmin, vector<GRBModel>& vector_model, unsigned short int& track);
void sort_d_t(vector<double>& d_t, vector<GRBVar>& var_bi, long& col_num, unsigned short int& N, unsigned short int& Vp, vector<pair<int, int>>& arc_pair, vector<double>& target, unsigned short int& t, vector<double>& credit, double& epsilon, vector<GRBVar>& var_lexmin, vector<unsigned short int>& N_star);
void lexmin_searching(vector<double>& d_t, bool& lex_min, unsigned short int& t_star, unsigned short int& N, long& col_num, double& epsilon, unsigned short int& n_star, GRBModel& model, vector<int>& ia, vector<int>& ja, vector<double>& ar, const unsigned short int& row_num, long& cnt_2, vector<double>& bound, vector<int>& nodeset, vector<unsigned short int>& N_star, unsigned short int& Vp, vector<pair<int, int>>& arc_pair, vector<double>& target, vector<double>& credit, vector<GRBVar>& var_lexmin, unsigned short int inst, vector<GRBModel>& vector_model, unsigned short int& track);
void epsilon_func(vector<double>& target, vector<double>& credit, double& epsilon, unsigned short int N);
double frac(double ori);

int main() {
	cout << "I solemnly swear that I am up to no good." << endl;
	bool c_involved = false;// true: credits considred; false:without credits 
	bool arbitray_maximum = false; //true: arbitray maximum cycple packing
	bool initial_nucl = false; // true: first to evaluate is the nucleolus
	bool lex_min = false;
	string solution_concept;
	string version;
	bool d1_dc = true;
	bool lexmin_call = false;
	bool lexmin_c_call = true;
	if (d1_dc) {
		version = "d1_dc";
	}
	else {
		if (lexmin_call) {
			version = "lexmin_call";
		}
		if (lexmin_c_call) {
			version = "lexmin_c_call";
		}
	}

	if (initial_nucl) {
		solution_concept = "nucleolus";
		cout << solution_concept << endl;
	}
	else {
		solution_concept = "shapley";
		cout << solution_concept << endl;
	}
	map<int, int> cycle_dis;
	map<int, int> cycle_dis_d;
	map<int, int> cycle_dis_t_c;
	map<int, int> cycle_dis_arbitrary;
	map<int, int> cycle_dis_lexmin;
	map<int, int> cycle_dis_lexmin_c;
	map<int, map<int, int>> cycle_dis_d_period;
	map<int, map<int, int>> cycle_dis_t_c_period;
	map<int, map<int, int>> cycle_dis_arbitrary_period;
	map<int, map<int, int>> cycle_dis_lexmin_period;
	map<int, map<int, int>> cycle_dis_lexmin_c_period;
	map<int, int> numofMaxSolution;
	map<int, int> numofMaxSolution_t_d;
	map<int, int> numofMaxSolution_arbitrary;
	vector<double> relative_d1_N(12, 0);
	vector<double> relative_d1_N_c(12, 0);
	vector<double> relative_arbitrary_N(12, 0);
	vector<double> relative_d1_N_initial_allocation(12, 0);
	vector<double> relative_lexmin(12, 0);
	vector<double> relative_lexmin_c(12, 0);
	vector<double> max_d1_N(12, 0);
	vector<double> max_d1_N_c(12, 0);
	vector<double> max_arbitrary_N(12, 0);
	vector<double> max_lexmin(12, 0);
	vector<double> max_lexmin_c(12, 0);
	vector<double> M_N(12, 0);
	vector<double> M_N_d_c(12, 0);
	vector<double> M_N_d_arbitrary(12, 0);
	vector<double> M_N_lex_min(12, 0);
	vector<double> M_N_lex_min_c(12, 0);
	vector<double> core_dis_N(12, 0);
	vector<double> core_dis_N_c(12, 0);
	vector<double> core_dis_N_arbitrary(12, 0);
	vector<long> out_of_core(12, 0);
	vector<long> out_of_core_c(12, 0);
	vector<long> out_of_core_arbitrary(12, 0);
	vector<double> data_preparation_N(12, 0);
	vector<double> graph_building_N(12, 0);
	vector<double> game_generation_arbitrary_N(12, 0);
	vector<double> game_generation_d1_N(12, 0);
	vector<double> game_generation_d1_c_N(12, 0);
	vector<double> game_generation_lexmin_N(12, 0);
	vector<double> game_generation_lexmin_c_N(12, 0);
	vector<double> time_arbitrary_N(12, 0);
	vector<double> time_d1_N(12, 0);
	vector<double> time_d1_c_N(12, 0);
	vector<double> time_lex_min_N(12, 0);
	vector<double> time_lex_min_c_N(12, 0);
	vector<double> total_time_arbitrary_N(12, 0);
	vector<double> total_time_d1_N(12, 0);
	vector<double> total_time_d1_c_N(12, 0);
	vector<double> total_time_lex_min_N(12, 0);
	vector<double> total_time_lex_min_c_N(12, 0);
	vector<double> solution_concept_time_arbitrary_N(12, 0);
	vector<double> solution_concept_time_d1_N(12, 0);
	vector<double> solution_concept_time_d1_c_N(12, 0);
	vector<double> solution_concept_time_lexmin_N(12, 0);
	vector<double> solution_concept_time_lexmin_c_N(12, 0);
	unsigned short int N; // number of countries/players
	unsigned short int inst; // instance number, integer between 0 and 99
	for (N = 9; N < 10; ++N) {
		cycle_dis.clear();
		cycle_dis_d.clear();
		cycle_dis_t_c.clear();
		cycle_dis_arbitrary.clear();
		cycle_dis_lexmin.clear();
		cycle_dis_lexmin_c.clear();
		double relative_d1 = 0;
		double relative_d1_c = 0;
		double relative_d1_arbitrary = 0;
		double relative_d1_initial_allocation = 0;
		double relative_lexmin_0 = 0;
		double relative_lexmin_c_0 = 0;
		double data_preparation = 0;
		double graph_building = 0;
		double game_generation_arbitrary = 0;
		double game_generation_d1 = 0;
		double game_generation_d1_c = 0;
		double game_generation_lexmin = 0;
		double game_generation_lexmin_c = 0;
		double time_arbitrary = 0;
		double time_d1 = 0;
		double time_d1_c = 0;
		double time_lex_min = 0;
		double time_lex_min_c = 0;
		double total_time_arbitrary = 0;
		double total_time_d1 = 0;
		double total_time_d1_c = 0;
		double total_time_lex_min = 0;
		double total_time_lex_min_c = 0;
		double solution_concept_time_arbitrary = 0;
		double solution_concept_time_d1 = 0;
		double solution_concept_time_d1_c = 0;
		double solution_concept_time_lexmin = 0;
		double solution_concept_time_lexmin_c = 0;
		double max_d1 = 0;
		double max_d1_c = 0;
		double max_d1_arbitrary = 0;
		double max_lexmin_0 = 0;
		double max_lexmin_c_0 = 0;
		double M_100 = 0;
		double M_100_d_c = 0;
		double M_100_d_arbitrary = 0;
		double M_lex_min = 0;
		double M_lex_min_c = 0;
		double core_100 = 0;
		double core_d = 0;
		double core_d_c = 0;
		double core_d_arbitrary = 0;
		long negative_core = 0;
		long negative_core_d = 0;
		long negative_core_d_c = 0;
		long negative_core_d_arbitrary = 0;
		for (inst = 0; inst < 100; ++inst) {
			// input parameters and data
			bool dispy = false; // true: information in terminal while running
			bool disp = false; // true: extremely detailed information while running, avoid with large graphs

			
			unsigned short int years = 6;
			unsigned short int periods_per_year = 4;
			string line;
			ifstream inp;
			unsigned short int graph_size = 2000;
			
			inp.open("C:/Users/kllg37/source/repos/ConsoleApplication1/ConsoleApplication1/genxml-" + to_string(inst) + ".xml"); // 1 out of the 100 instances generated by William Pettersson's web tool: https://wpettersson.github.io/kidney-webapp/#/
			getline(inp, line);
			inp.close();

			// building the (undirected) compatibility and the (directed) 'matching' graphs
			unsigned short int Vp = 4 * (unsigned short int)((graph_size / 4) / N);
			unsigned short int initialSize = Vp / 4;;
			unsigned short int no_of_nodes = N * Vp;
			vector<unsigned int> arc_out(0, 0);
			vector<unsigned int> arc_in(0, 0);
			unsigned int m = 0;
			unsigned short int k = 0;
			unsigned short int M = 0;//changed by XY: maximum size of cycle packing
			double M_total = 0;//changed by XY: maximum size of cycle packing
			vector<unsigned short int> node_labels(no_of_nodes, 0);
			vector<unsigned short int> label_positions(graph_size, graph_size + 1);
			ListGraph g;
			ListDigraph g_original;
			vector<ListGraph::Node> c(no_of_nodes);
			vector<ListGraph::Node> c_b(no_of_nodes); //changed by XY, creating an independent set of vertices for the biparite graph
			vector<ListDigraph::Node> c_original(no_of_nodes);//changed by XY, creating a node set for the original graph
			double t0 = cpuTime();
			xml_parser(line, node_labels, label_positions, c, c_b, c_original, k, g, g_original, arc_in, arc_out, m, no_of_nodes);
			double t1 = cpuTime();
			data_preparation += t1 - t0;

			// determining starting pairs and arrival times of others
			unsigned short int periods = years * periods_per_year;
			vector<unsigned short int> no_of_active_nodes(N, initialSize);
			ListGraph::NodeMap<bool> active_nodes(g);
			ListDigraph::NodeMap<bool> active_nodes_original(g_original);
			for (unsigned short int i = 0; i < N; i++) {
				for (unsigned short int j = 0; j < Vp; j++) {
					active_nodes[c[i * Vp + j]] = false;
					active_nodes[c_b[i * Vp + j]] = false;
					active_nodes_original[c_original[i * Vp + j]] = false;
				}
			}

			//read the seed
			string line_seed;
			ifstream seed_doc;
			seed_doc.open("seeds/seeds/n" + to_string(N) + "inst" + to_string(inst) + ".txt");
			getline(seed_doc, line_seed);
			seed_doc.close();
			unsigned int seed = 0;
			seed = stoi(line_seed); // for instance genxml-0.xml it is 18397106 for N=4, 20469263 for N=5, 22805501 for N=6, 25083567 for N=7, 27432197 for N=8, 30095162 for N=9, 33411331 for N=10, 6368187 for N=11, 13109406 for N=12, 23969593 for N=13, 43358281 for N=14, 79289906 for N=15
			//inp.open("n" + to_string(N) + "inst" + to_string(inst) + ".txt");
			//inp >> seed;
			//inp.close();
			//cout << seed;
			srand(seed);
			initial_pairs(Vp, N, active_nodes, active_nodes_original, c, c_b, c_original, initialSize);
			vector<unsigned short int> node_arrives(no_of_nodes, 0);
			arrival_times(node_arrives, Vp, N, active_nodes, c, periods, disp);
			if (disp) {
				cout << endl;
				vector<unsigned short int> bla(periods, 0);
				for (unsigned short int i = 0; i < no_of_nodes; i++)
					bla[node_arrives[i]]++;
				cout << "no of arrivals: ";
				for (unsigned short int i = 0; i < periods; i++)
					cout << bla[i] << " ";
				cout << endl << endl;
			}
			t1 = cpuTime();
			double rand_time = t1 - t0;

			ListGraph::EdgeMap<double> edge_card_weight(g, 0);
			ListDigraph::ArcMap<unsigned short int> arc_card_weight(g_original, 0);
			t0 = cpuTime();
			undi_lemon(m, arc_in, arc_out, label_positions, g, g_original, c, c_b, c_original, edge_card_weight, arc_card_weight, no_of_nodes);
			t1 = cpuTime();
			graph_building += t1 - t0;

			unsigned int S = pow(2, N) - 2;
			vector<double> v(S + 1, 0);
			vector<unsigned short int> s(N, 0);
			vector<vector<unsigned short int>> solution_LMC(periods, vector<unsigned short int>(N, 0));
			vector<vector<unsigned short int>> solution_LM(periods, vector<unsigned short int>(N, 0));
			vector<vector<unsigned short int>> solution_d1C(periods, vector<unsigned short int>(N, 0));
			vector<vector<unsigned short int>> solution_d1(periods, vector<unsigned short int>(N, 0));
			vector<vector<unsigned short int>> solution_rand(periods, vector<unsigned short int>(N, 0));
			vector<vector<unsigned short int>> solution_NC(periods, vector<unsigned short int>(N, 0));
			double prec = pow(10, -7);
			vector<double> init_alloc(N, 0);
			vector<vector<double>> init_alloc_LMC(periods, vector<double>(N, 0));
			vector<vector<double>> init_alloc_LM(periods, vector<double>(N, 0));
			vector<vector<double>> init_alloc_d1C(periods, vector<double>(N, 0));
			vector<vector<double>> init_alloc_d1(periods, vector<double>(N, 0));
			vector<vector<double>> init_alloc_rand(periods, vector<double>(N, 0));
			unsigned short int I1 = 0;
			unsigned short int I11 = 0;
			unsigned short int I2 = 0;
			vector<double> credit(N, 0);
			vector<double> deviation(N, 0);
			vector<bool> pos(N, false);
			vector<unsigned short int> w(N, 0);
			unsigned short int p;
			vector<unsigned short int> lb(N, 0);
			vector<unsigned short int> ub(N, 0);
			double opt = 0;
			vector<bool> leaving(no_of_nodes, false);
			unsigned short int Q = 0;
			vector<pair<int, int>> arc_pair;
			vector<int> nodeset(graph_size, 0);
			vector<pair<int, int>> cycle_distri;
			vector<vector<unsigned short int>> actual_alloc;
			vector<double> d(N, 0);
			double d_total = 0;
			double d_c_total = 0;
			vector<double> target(N, 0);
			vector<unsigned short int> s_ideal(N, 0);
			vector<double> init_alloc_ideal(N, 0);
			vector<unsigned short int> s_ideal_d1(N, 0);
			double max_d = 0;

			//arbitray maximum cycle packing
			/*arbitray_maximum = true;
			t0 = cpuTime();
			period_0(Q, no_of_active_nodes, N, s, Vp, node_arrives, active_nodes, active_nodes_original, c, c_b, c_original, credit);
			arbitraryMaximum(node_arrives, g, g_original, arc_pair, leaving, active_nodes, active_nodes_original, c, c_b, c_original, disp, no_of_active_nodes, N, Vp, periods, dispy, s, Q, v, initial_nucl, target, credit, edge_card_weight, t0, actual_alloc, nodeset, cycle_distri, d, M_total, d_total, c_involved, cycle_dis_arbitrary, numofMaxSolution, arbitray_maximum, initialSize, S, core_100, negative_core, prec, init_alloc, inst, max_d, game_generation_arbitrary, solution_concept_time_arbitrary, cycle_dis_arbitrary_period);
			t1 = cpuTime();
			total_time_arbitrary += t1 - t0;
			arbitray_maximum = false;
			M_100_d_arbitrary += M_total;
			//core_d_arbitrary += core_100;
			//negative_core_d_arbitrary += negative_core;
			relative_d1_arbitrary += (d_total / M_total);
			max_d1_arbitrary += max_d;
			cout << N << "countries" << " " << "instance_" << inst << "arbitrary done...";
			//------------------d1------------------
			cout << "start minimizing d_1" << endl;
			period_0(Q, no_of_active_nodes, N, s, Vp, node_arrives, active_nodes, active_nodes_original, c, c_b, c_original, credit);
			unsigned short int I = 0;
			t0 = cpuTime();
			min_d_1(node_arrives, g, g_original, arc_pair, leaving, active_nodes, active_nodes_original, c, c_b, c_original, disp, no_of_active_nodes, N, Vp, periods, dispy, s, Q, v, initial_nucl, target, credit, edge_card_weight, t0, actual_alloc, nodeset, cycle_distri, d, M_total, d_total, c_involved, cycle_dis_d, numofMaxSolution, arbitray_maximum, initialSize, S, core_100, negative_core, prec, init_alloc, d_c_total, inst, lex_min, max_d, game_generation_d1, solution_concept_time_d1, time_d1, cycle_dis_d_period, cycle_dis_arbitrary_period);
			t1 = cpuTime();
			total_time_d1 += t1 - t0;
			//cout << "negative core: " << negative_core;
			relative_d1 += (d_total / M_total);
			max_d1 += max_d;
			cout << "relative_d1: " << relative_d1 << endl;
			cout << "the number of countries: " << N << " " << "relative_d1" << " " << inst << " " << relative_d1 / (inst + 1) << endl;
			M_100 += M_total;
			cout << "the number of countries: " << N << " " << "M_100" << " " << inst << " " << M_100 / (inst + 1);
			//core_d += core_100;
			//negative_core_d += negative_core;
			cout << N << "countries" << " " << "instance_" << inst << "d1 done...";
			// --------------------d1+c-------------------
			c_involved = true;
			period_0(Q, no_of_active_nodes, N, s, Vp, node_arrives, active_nodes, active_nodes_original, c, c_b, c_original, credit);
			t0 = cpuTime();
			min_d_1(node_arrives, g, g_original, arc_pair, leaving, active_nodes, active_nodes_original, c, c_b, c_original, disp, no_of_active_nodes, N, Vp, periods, dispy, s, Q, v, initial_nucl, target, credit, edge_card_weight, t0, actual_alloc, nodeset, cycle_distri, d, M_total, d_total, c_involved, cycle_dis_t_c, numofMaxSolution, arbitray_maximum, initialSize, S, core_100, negative_core, prec, init_alloc, d_c_total, inst, lex_min, max_d, game_generation_d1_c, solution_concept_time_d1_c, time_d1_c, cycle_dis_t_c_period, cycle_dis_arbitrary_period);
			t1 = cpuTime();
			total_time_d1_c += t1 - t0;
			relative_d1_initial_allocation += (d_c_total / M_total);
			max_d1_c += max_d;
			c_involved = false;
			M_100_d_c += M_total;
			//core_d_c += core_100;
			//negative_core_d_c += negative_core;
			relative_d1_c += (d_total / M_total);
			cout << N << "countries" << " " << "instance_" << inst << "d1+c done..."<<endl;
			//-------------------lexmin--------------
			std::cout << N << "countries" << " " << "instance_" << inst << "lexmin starts...";
			lex_min = true;
			period_0(Q, no_of_active_nodes, N, s, Vp, node_arrives, active_nodes, active_nodes_original, c, c_b, c_original, credit);
			t0 = cpuTime();
			min_d_1(node_arrives, g, g_original, arc_pair, leaving, active_nodes, active_nodes_original, c, c_b, c_original, disp, no_of_active_nodes, N, Vp, periods, dispy, s, Q, v, initial_nucl, target, credit, edge_card_weight, t0, actual_alloc, nodeset, cycle_distri, d, M_total, d_total, c_involved, cycle_dis_lexmin, numofMaxSolution, arbitray_maximum, initialSize, S, core_100, negative_core, prec, init_alloc, d_c_total, inst, lex_min, max_d, game_generation_lexmin, solution_concept_time_lexmin, time_lex_min, cycle_dis_lexmin_period, cycle_dis_arbitrary_period);
			t1 = cpuTime();
			total_time_lex_min += t1 - t0;
			relative_lexmin_0 += (d_total / M_total);
			max_lexmin_0 += max_d;
			lex_min = false;
			M_lex_min += M_total;
			std::cout << N << "countries" << " " << "instance_" << inst << "relative deviation" << d_total / M_total << "lexmin done...";*/
			//-----------------lexmin+c----------------
			std::cout << N << "countries" << " " << "instance_" << inst << "lexmin+c starts...";
			lex_min = true;
			c_involved = true;
			period_0(Q, no_of_active_nodes, N, s, Vp, node_arrives, active_nodes, active_nodes_original, c, c_b, c_original, credit);
			t0 = cpuTime();
			min_d_1(node_arrives, g, g_original, arc_pair, leaving, active_nodes, active_nodes_original, c, c_b, c_original, disp, no_of_active_nodes, N, Vp, periods, dispy, s, Q, v, initial_nucl, target, credit, edge_card_weight, t0, actual_alloc, nodeset, cycle_distri, d, M_total, d_total, c_involved, cycle_dis_lexmin_c, numofMaxSolution, arbitray_maximum, initialSize, S, core_100, negative_core, prec, init_alloc, d_c_total, inst, lex_min, max_d, game_generation_lexmin_c, solution_concept_time_lexmin_c, time_lex_min_c, cycle_dis_lexmin_c_period, cycle_dis_arbitrary_period);
			t1 = cpuTime();
			total_time_lex_min_c += t1 - t0;
			relative_lexmin_c_0 += (d_total / M_total);
			max_lexmin_c_0 += max_d;
			lex_min = false;
			c_involved = false;
			M_lex_min_c += M_total;
			std::cout << N << "countries" << " " << "instance_" << inst << "relative deviation" << d_total / M_total << "lexmin+c done...";

		}
		relative_d1_N[N - 4] = relative_d1 / 100;
		relative_d1_N_c[N - 4] = relative_d1_c / 100;
		relative_d1_N_initial_allocation[N - 4] = relative_d1_initial_allocation / 100;
		relative_arbitrary_N[N - 4] = relative_d1_arbitrary / 100;
		relative_lexmin[N - 4] = relative_lexmin_0 / 100;
		relative_lexmin_c[N - 4] = relative_lexmin_c_0 / 100;
		max_d1_N[N - 4] = max_d1 / 100;
		max_d1_N_c[N - 4] = max_d1_c / 100;
		max_arbitrary_N[N - 4] = max_d1_arbitrary / 100;
		max_lexmin[N - 4] = max_lexmin_0 / 100;
		max_lexmin_c[N - 4] = max_lexmin_c_0 / 100;
		/*core_dis_N[N - 4] = core_d / 100;
		core_dis_N_c[N - 4] = core_d_c / 100;
		core_dis_N_arbitrary[N - 4] = core_d_arbitrary / 100;
		out_of_core[N - 4] = negative_core_d;
		out_of_core_c[N - 4] = negative_core_d_c;
		out_of_core_arbitrary[N - 4] = negative_core_d_arbitrary;*/
		M_N[N - 4] = M_100 / 100;
		M_N_d_c[N - 4] = M_100_d_c / 100;
		M_N_d_arbitrary[N - 4] = M_100_d_arbitrary / 100;
		M_N_lex_min[N - 4] = M_lex_min / 100;
		M_N_lex_min_c[N - 4] = M_lex_min_c / 100;
		data_preparation_N[N - 4] = data_preparation / 100;
		graph_building_N[N - 4] = graph_building / 100;
		game_generation_arbitrary_N[N - 4] = game_generation_arbitrary / 100;
		game_generation_d1_N[N - 4] = game_generation_d1 / 100;
		game_generation_d1_c_N[N - 4] = game_generation_d1_c / 100;
		game_generation_lexmin_N[N - 4] = game_generation_lexmin / 100;
		game_generation_lexmin_c_N[N - 4] = game_generation_lexmin_c / 100;
		time_arbitrary_N[N - 4] = time_arbitrary / 100;
		time_d1_N[N - 4] = time_d1 / 100;
		time_d1_c_N[N - 4] = time_d1_c / 100;
		time_lex_min_N[N - 4] = time_lex_min / 100;
		time_lex_min_c_N[N - 4] = time_lex_min_c / 100;
		total_time_arbitrary_N[N - 4] = total_time_arbitrary / 100;
		total_time_d1_N[N - 4] = total_time_d1 / 100;
		total_time_d1_c_N[N - 4] = total_time_d1_c / 100;
		total_time_lex_min_N[N - 4] = total_time_lex_min / 100;
		total_time_lex_min_c_N[N - 4] = total_time_lex_min_c / 100;
		solution_concept_time_arbitrary_N[N - 4] = solution_concept_time_arbitrary / 100;
		solution_concept_time_d1_N[N - 4] = solution_concept_time_d1 / 100;
		solution_concept_time_d1_c_N[N - 4] = solution_concept_time_d1_c / 100;
		solution_concept_time_lexmin_N[N - 4] = solution_concept_time_lexmin / 100;
		solution_concept_time_lexmin_c_N[N - 4] = solution_concept_time_lexmin_c / 100;
		ofstream res;
		res.open(solution_concept+ version+"results_10.txt", ofstream::out | ofstream::trunc);
		for (unsigned short int i = 0; i < N - 3; i++) {
			res << i + 4 << "countries" << endl;
			if (d1_dc) {
				res << "data preparation: " << data_preparation_N[i] << endl;
				res << "build graph: " << graph_building_N[i] << endl;
				res << "minimizing d_1: " << relative_d1_N[i] << endl;
				res << "minimizing max_d_1: " << max_d1_N[i] << endl;
				res << "average number of transplants: " << M_N[i] << endl;
				res << "total time: " << total_time_d1_N[i] << endl;
				res << "scenario time: " << time_d1_N[i] << endl;
				res << "game generation: " << game_generation_d1_N[i] << endl;
				res << "solution concept: " << solution_concept_time_d1_N[i] << endl;
				/*res << "core distance: " << core_dis_N[i] << endl;
				res << "out_of_core: " << out_of_core[i] << endl;*/
				res << "minimizing d_1_c: " << relative_d1_N_c[i] << endl;
				res << "minimizing d_1_c_initial_allocation: " << relative_d1_N_initial_allocation[i] << endl;
				res << "minimizing max_d_1_c: " << max_d1_N_c[i] << endl;
				res << "average number of transplants_c: " << M_N_d_c[i] << endl;
				res << "total time: " << total_time_d1_c_N[i] << endl;
				res << "scenario time: " << time_d1_c_N[i] << endl;
				res << "game generation: " << game_generation_d1_c_N[i] << endl;
				res << "solution concept: " << solution_concept_time_d1_c_N[i] << endl;
				/*res << "core distance_c: " << core_dis_N_c[i] << endl;
				res << "out_of_core_c: " << out_of_core_c[i] << endl;*/
			}
			if (lexmin_call) {
				res << "lex min " << relative_lexmin[i] << endl;
				res << "lex min max d " << max_lexmin[i] << endl;
				res << "average number of transplants: " << M_N_lex_min[i] << endl;
				res << "total time: " << total_time_lex_min_N[i] << endl;
				res << "scenario time: " << time_lex_min_N[i] << endl;
				res << "game generation: " << game_generation_lexmin_N[i] << endl;
				res << "solution concept: " << solution_concept_time_lexmin_N[i] << endl;
			}
			res << "minimizing d_1_arbitrary: " << relative_arbitrary_N[i] << endl;
			res << "minimizing max_d_1_arbitrary: " << max_arbitrary_N[i] << endl;
			res << "average number of transplants_arbitrary: " << M_N_d_arbitrary[i] << endl;
			res << "total time: " << total_time_arbitrary_N[i] << endl;
			res << "scenario time: " << time_arbitrary_N[i] << endl;
			res << "game generation: " << game_generation_arbitrary_N[i] << endl;
			res << "solution concept: " << solution_concept_time_arbitrary_N[i] << endl;
			/*res << "core distance_arbitrary: " << core_dis_N_arbitrary[i] << endl;
			res << "out_of_core_arbitrary: " << out_of_core_arbitrary[i] << endl;*/
			if (lexmin_c_call) {
				res << "lex min+c " << relative_lexmin_c[i] << endl;
				res << "lex min+c max " << max_lexmin_c[i] << endl;
				res << "average number of transplants: " << M_N_lex_min_c[i] << endl;
				res << "total time: " << total_time_lex_min_c_N[i] << endl;
				res << "scenario time: " << time_lex_min_c_N[i] << endl;
				res << "game generation: " << game_generation_lexmin_c_N[i] << endl;
				res << "solution concept: " << solution_concept_time_lexmin_c_N[i] << endl;
				res << endl;
			}
			
		}

		res.close();

		if (d1_dc) {
			ofstream res_dis;
			res_dis.open(solution_concept + version + "cycle_dis_d" + to_string(N) + ".txt", ofstream::out | ofstream::trunc);
			for (const auto& elem : cycle_dis_d) {
				res_dis << elem.first << ": " << elem.second << endl;
			}
			res_dis << endl;
			res_dis.close();


			ofstream res_dis_c;
			res_dis_c.open(solution_concept + version + "cycle_dis_c" + to_string(N) + ".txt", ofstream::out | ofstream::trunc);
			for (const auto& elem : cycle_dis_t_c) {
				res_dis_c << elem.first << ": " << elem.second << endl;
			}
			res_dis_c << endl;
			res_dis_c.close();
		}
		

		ofstream res_dis_arbitrary;
		res_dis_arbitrary.open(solution_concept + version + "cycle_dis_arbitrary" + to_string(N) + ".txt", ofstream::out | ofstream::trunc);
		for (const auto& elem : cycle_dis_arbitrary) {
			res_dis_arbitrary << elem.first << ": " << elem.second << endl;
		}
		res_dis_arbitrary << endl;
		res_dis_arbitrary.close();

		if (lexmin_call) {
			ofstream res_dis_lexmin;
			res_dis_lexmin.open(solution_concept + version + "cycle_dis_lexmin" + to_string(N) + ".txt", ofstream::out | ofstream::trunc);
			for (const auto& elem : cycle_dis_lexmin) {
				res_dis_lexmin << elem.first << ": " << elem.second << endl;
			}
			res_dis_lexmin << endl;
			res_dis_lexmin.close();
		}
		
		if (lexmin_c_call) {
			ofstream res_dis_lexmin_c;
			res_dis_lexmin_c.open(solution_concept + version + "cycle_dis_lexmin_c" + to_string(N) + ".txt", ofstream::out | ofstream::trunc);
			for (const auto& elem : cycle_dis_lexmin_c) {
				res_dis_lexmin_c << elem.first << ": " << elem.second << endl;
			}
			res_dis_lexmin_c << endl;
			res_dis_lexmin_c.close();
		}
		

		// cycle distributions seperating by periods
		if (d1_dc) {
			ofstream res_cycle_dis_d_period;
			unsigned short int check_value_d = 0;;
			for (unsigned short int i = 0; i < 24; ++i) {
				res_cycle_dis_d_period.open(solution_concept + version + "cycle_dis_d_period" + to_string(N) + "_" + to_string(i) + ".txt", ofstream::out | ofstream::trunc);
				for (const auto& elem : cycle_dis_d_period[(N) * (i + 1)]) {
					check_value_d += elem.first * elem.second;
					res_cycle_dis_d_period << elem.first << ": " << elem.second << endl;
				}
				res_cycle_dis_d_period << endl;
				res_cycle_dis_d_period.close();
			}
			cout << "check_value_d: " << check_value_d << endl;
			res_cycle_dis_d_period.close();

			ofstream res_cycle_dis_c_period;
			unsigned short int check_value_d_c = 0;
			for (unsigned short int i = 0; i < 24; ++i) {
				res_cycle_dis_c_period.open(solution_concept + version + "cycle_dis_d_c_period" + to_string(N) + "_" + to_string(i) + ".txt", ofstream::out | ofstream::trunc);
				for (const auto& elem : cycle_dis_t_c_period[(N) * (i + 1)]) {
					check_value_d_c += elem.first * elem.second;
					res_cycle_dis_c_period << elem.first << ": " << elem.second << endl;
				}
				res_cycle_dis_c_period << endl;
				res_cycle_dis_c_period.close();
			}
			cout << "check_value_d_c: " << check_value_d_c << endl;
			res_cycle_dis_c_period.close();
		}
		

		ofstream res_cycle_dis_arbitrary_period;
		unsigned short int check_value_arbitrary = 0;
		for (unsigned short int i = 0; i < 24; ++i) {
			res_cycle_dis_arbitrary_period.open(solution_concept + version + "cycle_dis_arbitrary_period" + to_string(N) + "_" + to_string(i) + ".txt", ofstream::out | ofstream::trunc);
			for (const auto& elem : cycle_dis_arbitrary_period[(N) * (i + 1)]) {
				check_value_arbitrary += elem.first * elem.second;
				res_cycle_dis_arbitrary_period << elem.first << ": " << elem.second << endl;
			}
			res_cycle_dis_arbitrary_period << endl;
			res_cycle_dis_arbitrary_period.close();
		}
		cout << "check_value_arbitrary: " << check_value_arbitrary << endl;
		res_cycle_dis_arbitrary_period.close();

		if (lexmin_call) {
			ofstream res_dis_lexmin_period;
			unsigned short int check_value_lexmin = 0;
			for (unsigned short int i = 0; i < 24; ++i) {
				res_dis_lexmin_period.open(solution_concept + version + "cycle_dis_lexmin_period" + to_string(N) + "_" + to_string(i) + ".txt", ofstream::out | ofstream::trunc);
				for (const auto& elem : cycle_dis_lexmin_period[(N) * (i + 1)]) {
					check_value_lexmin += elem.first * elem.second;
					res_dis_lexmin_period << elem.first << ": " << elem.second << endl;
				}
				res_dis_lexmin_period << endl;
				res_dis_lexmin_period.close();
			}
			cout << "check_value_lexmin: " << check_value_lexmin << endl;
			res_dis_lexmin_period.close();
		}
		
		if (lexmin_c_call) {
			ofstream res_cycle_dis_lexmin_c_period;
			unsigned short int check_value = 0;
			for (unsigned short int i = 0; i < 24; ++i) {
				res_cycle_dis_lexmin_c_period.open(solution_concept + version + "cycle_dis_lexmin_c_period" + to_string(N) + "_" + to_string(i) + ".txt", ofstream::out | ofstream::trunc);
				for (const auto& elem : cycle_dis_lexmin_c_period[(N) * (i + 1)]) {
					check_value += elem.first * elem.second;
					res_cycle_dis_lexmin_c_period << elem.first << ": " << elem.second << endl;
				}
				res_cycle_dis_lexmin_c_period << endl;
				res_cycle_dis_lexmin_c_period.close();
			}
			cout << "check_value: " << check_value << endl;
			res_cycle_dis_lexmin_c_period.close();
		}
		

	}
	
	

	/*if (second) {
		if (initial_nucl)
			initial_nucl = false;
		else
			initial_nucl = true;
		I1 = 0;
		I11 = 0;
		I2 = 0;
		Q = 0;
		t0 = cpuTime();
		for (unsigned short int i = 0; i < N; i++) {
			s[i] = 0;
			credit[i] = 0;
			no_of_active_nodes[i] = Vp / 4;
			for (unsigned short int j = 0; j < Vp; j++) {
				if (node_arrives[i * Vp + j] == 0) {
					active_nodes[c[i * Vp + j]] = true;
				}
				else {
					active_nodes[c[i * Vp + j]] = false;
				}
			}
		}
	


		res.open("results2.txt", ofstream::out | ofstream::trunc);
		res << fixed << setprecision(0) << seed << endl << endl;
		res << fixed << setprecision(17) << read_time << endl << graph_time << endl << rand_time << endl << init_time << endl << game_time << endl << init_alloc_time << endl << matching_time << endl << endl;
		res << fixed << setprecision(0) << I1 << endl << I11 << endl << I2 << endl;
		for (unsigned short int Q = 0; Q < periods; Q++) {
			for (unsigned short int i = 0; i < N; i++)
				res << fixed << setprecision(17) << init_alloc_LMC[Q][i] << endl;
			res << endl;
			for (unsigned short int i = 0; i < N; i++)
				res << fixed << setprecision(0) << solution_LMC[Q][i] << endl;
			res << endl;
		}
		res << endl;
		for (unsigned short int Q = 0; Q < periods; Q++) {
			for (unsigned short int i = 0; i < N; i++)
				res << fixed << setprecision(17) << init_alloc_d1C[Q][i] << endl;
			res << endl;
			for (unsigned short int i = 0; i < N; i++)
				res << fixed << setprecision(0) << solution_d1C[Q][i] << endl;
			res << endl;
		}
		res << endl;
		for (unsigned short int Q = 0; Q < periods; Q++) {
			for (unsigned short int i = 0; i < N; i++)
				res << fixed << setprecision(17) << init_alloc_LM[Q][i] << endl;
			res << endl;
			for (unsigned short int i = 0; i < N; i++)
				res << fixed << setprecision(0) << solution_LM[Q][i] << endl;
			res << endl;
		}
		res << endl;
		for (unsigned short int Q = 0; Q < periods; Q++) {
			for (unsigned short int i = 0; i < N; i++)
				res << fixed << setprecision(17) << init_alloc_d1[Q][i] << endl;
			res << endl;
			for (unsigned short int i = 0; i < N; i++)
				res << fixed << setprecision(0) << solution_d1[Q][i] << endl;
			res << endl;
		}
		res << endl;
		for (unsigned short int Q = 0; Q < periods; Q++) {
			for (unsigned short int i = 0; i < N; i++)
				res << fixed << setprecision(17) << init_alloc_rand[Q][i] << endl;
			res << endl;
			for (unsigned short int i = 0; i < N; i++)
				res << fixed << setprecision(0) << solution_rand[Q][i] << endl;
			res << endl;
		}
		res << endl;
		for (unsigned short int i = 0; i < N; i++)
			res << fixed << setprecision(17) << init_alloc_ideal[i] << endl;
		res << endl;
		for (unsigned short int i = 0; i < N; i++)
			res << fixed << setprecision(0) << s_ideal[i] << endl;
		res << endl;

		for (unsigned short int i = 0; i < N; i++)
			res << fixed << setprecision(0) << s_ideal_d1[i] << endl;
		res << endl;

		for (unsigned short int Q = 0; Q < periods; Q++) {
			for (unsigned short int i = 0; i < N; i++)
				res << fixed << setprecision(0) << solution_NC[Q][i] << endl;
			res << endl;
		}
		res.close();
	}*/
	cout << "Mischief managed!" << endl;
	return 0;
}



void coop_game(ListGraph& g, vector<double>& v, unsigned int& S, vector<unsigned short int>& s, vector<ListGraph::Node>& c, vector<ListGraph::Node>& c_b, ListGraph::EdgeMap<double>& edge_card_weight, bool& dispy, unsigned short int& Vp, unsigned short int& N, ListGraph::NodeMap<bool>& active_nodes, vector<bool>& leaving, map<int, int>& numofMaxSolution, unsigned short int& Q, bool& arbitray_maximum, vector<pair<int, int>>& cycle_distri, map<int, int>& cycle_dis, double& game_generation, std::map<int, std::map<int, int>>& cycle_dis_arbitrary_period) {
	vector<bool> a(N, false);
	double t0 = cpuTime();
	for (unsigned int i = 0; i < S; i++) {
		de2bi(i, a, N);
		ListGraph::NodeMap<bool> coal3(g, false);
		for (unsigned short int j = 0; j < N; j++) {
			if (a[j]) {
				for (unsigned short int k = j * Vp; k < (j + 1) * Vp; ++k) {
					if (active_nodes[c[k]]) {
						coal3[c[k]] = true;
						coal3[c_b[k]] = true;
					}
				}
			}

		}
		MaxWeightedPerfectMatching<FilterNodes<ListGraph>, ListGraph::EdgeMap<double>> coal_m1(FilterNodes<ListGraph>(g, coal3), edge_card_weight);
		coal_m1.run();
		v[i] = coal_m1.matchingWeight();
	}


	cout << "finish generating the copy" << endl;
	FilterNodes<ListGraph> sg(g, active_nodes);
	MaxWeightedPerfectMatching<FilterNodes<ListGraph>, ListGraph::EdgeMap<double>> grand_coal(FilterNodes<ListGraph>(g, active_nodes), edge_card_weight);
	grand_coal.run();
	v[S] = grand_coal.matchingWeight();
	double t1 = cpuTime();
	game_generation += t1 - t0;
	grand_coal.matchingMap();
	if (arbitray_maximum) {
		unsigned short int a = 0;
		unsigned short int b = 0;
		for (FilterNodes<ListGraph>::NodeIt n(sg); n != INVALID; ++n) {
			//cout << sg.id(n) << endl;
			if (!(grand_coal.matching(n) == INVALID) && edge_card_weight[grand_coal.matching(n)] > 0 && sg.id(n) % 2 == 0) {
				cycle_distri.push_back({ sg.id(n) / 2, (sg.id(grand_coal.mate(n)) - 1) / 2 });
				leaving[sg.id(n) / 2] = true;
				//cout << "sg.id(n):"<<sg.id(n) << endl;
				//cout << "sg.id(n): " << sg.id(n) << endl;
				for (unsigned short int i = 0; i < N; ++i) {
					if (i * Vp <= sg.id(n) / 2 && sg.id(n) / 2 < (i + 1) * Vp) {
						++s[i];
					}
				}

			}
			//cycle_distri.push_back(arc_pair[i - 1]);
		}

		cycle_distribution(cycle_dis_arbitrary_period, cycle_dis, cycle_distri, N, Q);
	}


	if (dispy)
		cout << "grand coal: " << v[S] << endl;

	if (dispy) {
		cout << "s: ";
		for (unsigned short int i = 0; i < N; i++) {
			cout << s[i] << " ";
		}
		cout << endl;
	}
	return;
}


void changing_nodes(ListGraph::NodeMap<bool>& active_nodes, ListDigraph::NodeMap<bool>& active_nodes_original, vector<bool>& leaving, vector<unsigned short int>& no_of_active_nodes, unsigned short int& N, unsigned short int& Vp, vector<unsigned short int>& node_arrives, unsigned short int& Q, vector<ListGraph::Node>& c, vector<ListGraph::Node>& c_b, vector<ListDigraph::Node>& c_original, vector<unsigned short int>& s, vector<double>& d, vector<double>& target) {
	for (unsigned short int i = 0; i < N; i++) {
		s[i] = 0;
		target[i] = 0;
		for (unsigned short int j = 0; j < Vp; j++) {
			if (leaving[i * Vp + j]) {
				active_nodes[c[i * Vp + j]] = false;
				active_nodes[c_b[i * Vp + j]] = false;
				active_nodes_original[c_original[i * Vp + j]] = false;
				no_of_active_nodes[i]--;
				leaving[i * Vp + j] = false;
			}
			else {
				if (active_nodes[c[i * Vp + j]] && node_arrives[i * Vp + j] == Q - 4) {
					active_nodes[c[i * Vp + j]] = false;
					active_nodes[c_b[i * Vp + j]] = false;
					active_nodes_original[c_original[i * Vp + j]] = false;
					no_of_active_nodes[i]--;
				}
			}
			if (node_arrives[i * Vp + j] == Q) {
				active_nodes[c[i * Vp + j]] = true;
				active_nodes[c_b[i * Vp + j]] = true;
				active_nodes_original[c_original[i * Vp + j]] = true;
				no_of_active_nodes[i]++;
			}
		}
	}
	return;
}

void initial_pairs(unsigned short int& Vp, unsigned short int& N, ListGraph::NodeMap<bool>& active_nodes, ListDigraph::NodeMap<bool>& active_nodes_original, vector<ListGraph::Node>& c, vector<ListGraph::Node>& c_b, vector<ListDigraph::Node>& c_original, unsigned short int& initialSize) {
	unsigned short int coal = rand() % Vp;
	unsigned short int count = 0;
	for (unsigned short int i = 0; i < N; i++) {
		while (count < Vp / 4) {
			if (active_nodes[c[i * Vp + coal]]) {
				coal = rand() % Vp;
			}
			else {
				active_nodes[c[i * Vp + coal]] = true;
				active_nodes[c_b[i * Vp + coal]] = true;
				active_nodes_original[c_original[i * Vp + coal]] = true;
				count++;
				coal = rand() % Vp;
			}
		}
		count = 0;
	}
	return;
}

void arrival_times(vector<unsigned short int>& node_arrives, unsigned short int& Vp, unsigned short int& N, ListGraph::NodeMap<bool>& active_nodes, vector<ListGraph::Node>& c, unsigned short int& periods, bool& disp) {
	for (unsigned short int i = 0; i < N; i++) {
		if (disp)
			cout << "Country " << i << " arrivals: ";
		for (unsigned short int j = 0; j < Vp; j++) {
			if (!(active_nodes[c[i * Vp + j]])) {
				node_arrives[i * Vp + j] = rand() % (periods - 1) + 1;
			}
			if (disp)
				cout << node_arrives[i * Vp + j] << " ";
		}
		if (disp)
			cout << endl;
	}
	return;
}

void insertion_sort(vector<unsigned short int>& w, vector<double>& deviation, unsigned short int& N) {
	w[0] = 0;
	for (unsigned short int i = 1; i < N; i++) {
		if (deviation[i] <= deviation[w[i - 1]]) {
			w[i] = i;
		}
		else {
			w[i] = w[i - 1];
			if (i == 1) {
				w[0] = i;
			}
			else {
				for (unsigned short int j = i - 2; j >= 0; j--) {
					if (deviation[i] <= deviation[w[j]]) {
						w[j + 1] = i;
						break;
					}
					else {
						w[j + 1] = w[j];
						if (j == 0) {
							w[0] = i;
							break;
						}
					}
				}
			}
		}
	}
	return;
}

void undi_lemon(unsigned int& m, vector<unsigned int>& arc_in, vector<unsigned int>& arc_out, vector<unsigned short int>& label_positions, ListGraph& g, ListDigraph& g_original, vector<ListGraph::Node>& c, vector<ListGraph::Node>& c_b, vector<ListDigraph::Node>& c_original, ListGraph::EdgeMap<double>& edge_card_weight, ListDigraph::ArcMap<unsigned short int>& arc_card_weight, unsigned short int& no_of_nodes) {
	bool halt = false;
	for (int i = 0; i < no_of_nodes; i++) {
		ListGraph::Edge e = g.addEdge(c[i], c_b[i]);
		edge_card_weight[e] = 0;
	}
	for (unsigned int i = 0; i < m; i++) {
		if (label_positions[arc_in[i]] < no_of_nodes) { //XY: filter 65535 positions
			ListGraph::Edge e = g.addEdge(c[label_positions[arc_out[i]]], c_b[label_positions[arc_in[i]]]);
			edge_card_weight[e] = 1;
			ListDigraph::Arc a_original = g_original.addArc(c_original[label_positions[arc_out[i]]], c_original[label_positions[arc_in[i]]]);
			arc_card_weight[a_original] = 1;
		}
	}
	return;
}
void de2bi(unsigned int& k, vector<bool>& a, unsigned short int& n) {
	vector<bool> zero(n, false);
	a = zero;
	unsigned int i = 2;
	for (unsigned short int c = 0; c < n - 2; c++)
		i += i;
	unsigned int j = k + 1;
	unsigned short int l = n - 1;
	while (j > 0) {
		if (j >= i) {
			a[l] = true;
			j -= i;
		}
		i /= 2;
		l--;
	}
	return;
}

void shapley(vector<double>& shapl, vector<double>& v, unsigned short int& n, unsigned int& S) {
	vector<double> w(n, 0);
	w[0] = 1 / (double)n;
	vector<double> expo(n, 1);
	for (unsigned short int j = 1; j < n; j++) {
		w[j] = w[j - 1] * j / (n - j);
		expo[j] = pow(2, j);
	}
	vector<bool> a(n, false);
	unsigned short int k = 0;
	for (unsigned short int i = 0; i < n - 1; i++)
		shapl[i] = (double)(v[expo[i] - 1]) / n;
	for (unsigned int i = 0; i < S; i++) {
		de2bi_card(i, a, n, k);
		for (unsigned short int j = 0; j < n - 1; j++) {
			if (!a[j])
				shapl[j] += w[k] * (v[i + expo[j]] - v[i]);
		}
	}
	shapl[n - 1] = v[S];
	for (unsigned short int j = 0; j < n - 1; j++)
		shapl[n - 1] -= shapl[j];
	return;
}

void xml_parser(string& line, vector<unsigned short int>& node_labels, vector<unsigned short int>& label_positions, vector<ListGraph::Node>& c, vector<ListGraph::Node>& c_b, vector<ListDigraph::Node>& c_original, unsigned short int& k, ListGraph& g, ListDigraph& g_original, vector<unsigned int>& arc_in, vector<unsigned int>& arc_out, unsigned int& m, unsigned short int& no_of_nodes) {
	unsigned int l = 6;
	unsigned short int n = 0; //XY: track the number of nodes
	while (l < line.size() - 7) {
		if (line[l] == '<' && line[l + 1] == 'e') {
			l = l + 17;
			n++;
			if (!is_next_char_digit(line, l)) {
				node_labels[n - 1] = char2uint(line[l]); //XY: donor id
			}
			else {
				if (!is_next_char_digit(line, l + 1)) {
					node_labels[n - 1] = 10 * char2uint(line[l]) + char2uint(line[l + 1]);
					l++;
				}
				else {
					if (!is_next_char_digit(line, l + 2)) {
						node_labels[n - 1] = 100 * char2uint(line[l]) + 10 * char2uint(line[l + 1]) + char2uint(line[l + 2]);
						l = l + 2;
					}
					else {
						node_labels[n - 1] = 1000 * char2uint(line[l]) + 100 * char2uint(line[l + 1]) + 10 * char2uint(line[l + 2]) + char2uint(line[l + 3]);
						l = l + 3;
					}
				}
			}
			if (n + k - 1 == node_labels[n - 1]) {
				label_positions[n + k - 1] = n - 1;
			}
			else {
				while (n + k - 1 < node_labels[n - 1]) {
					label_positions[n + k - 1] = 65535;
					label_positions.push_back(0);
					k++;
				}
				label_positions[n + k - 1] = n - 1;
			}

			c[n - 1] = g.addNode();//XY: add donor ids
			c_b[n - 1] = g.addNode();//changed by XY, add patient ids
			c_original[n - 1] = g_original.addNode();//changed by XY, add patient-donor pairs to the original graph
			l = l + 9;
			if (!is_next_char_digit(line, l)) {
				////donor_ages.push_back(char2uint(line[l]));
				//donor_ages[n - 1] = char2uint(line[l]);
			}
			else {
				if (!is_next_char_digit(line, l + 1)) {
					////donor_ages.push_back(10*char2uint(line[l])+char2uint(line[l+1]));
					//donor_ages[n - 1] = 10*char2uint(line[l])+char2uint(line[l+1]);
					l++;
				}
				else {
					if (!is_next_char_digit(line, l + 2)) {
						////donor_ages.push_back(100*char2uint(line[l])+10*char2uint(line[l+1])+char2uint(line[l+2]));
						//donor_ages[n - 1] = 100*char2uint(line[l])+10*char2uint(line[l+1])+char2uint(line[l+2]);
						l = l + 2;
					}
					else {
						////if (!is_next_char_digit(line, l + 3)){
						////donor_ages.push_back(1000*char2uint(line[l])+100*char2uint(line[l+1])+10*char2uint(line[l+2])+char2uint(line[l+3]));
						//donor_ages[n - 1] = 1000*char2uint(line[l])+100*char2uint(line[l+1])+10*char2uint(line[l+2])+char2uint(line[l+3]);
						l = l + 3;
						////}
					}
				}
			}
			l = l + 25;
			if (!is_next_char_digit(line, l)) {
				if (node_labels[n - 1] != char2uint(line[l]))
					cout << "ID ERROR!" << endl;
			}
			else {
				if (!is_next_char_digit(line, l + 1)) {
					if (node_labels[n - 1] != 10 * char2uint(line[l]) + char2uint(line[l + 1]))
						cout << "ID ERROR!" << endl;
					l++;
				}
				else {
					if (!is_next_char_digit(line, l + 2)) {
						if (node_labels[n - 1] != 100 * char2uint(line[l]) + 10 * char2uint(line[l + 1]) + char2uint(line[l + 2]))
							cout << "ID ERROR!" << endl;
						l = l + 2;
					}
					else {
						//if (!is_next_char_digit(line, l + 3)){
						if (node_labels[n - 1] != 1000 * char2uint(line[l]) + 100 * char2uint(line[l + 1]) + 10 * char2uint(line[l + 2]) + char2uint(line[l + 3]))
							cout << "ID ERROR!" << endl;
						l = l + 3;
						//}
					}
				}
			}
			if (line[l + 21] == 'm')
				l = l + 29;
			else
				l = l + 28;
		}
		// XY: recipients
		while (line[l] == '<' && line[l + 1] == 'm' && line[l + 6] == '>') {
			m++;//number of compatibilities
			l = l + 18;
			arc_out.push_back(node_labels[n - 1]);
			if (!is_next_char_digit(line, l)) {
				arc_in.push_back(char2uint(line[l]));
			}
			else {
				if (!is_next_char_digit(line, l + 1)) {
					arc_in.push_back(10 * char2uint(line[l]) + char2uint(line[l + 1]));
					l++;
				}
				else {
					if (!is_next_char_digit(line, l + 2)) {
						arc_in.push_back(100 * char2uint(line[l]) + 10 * char2uint(line[l + 1]) + char2uint(line[l + 2]));
						l = l + 2;
					}
					else {
						//if (!is_next_char_digit(line, l + 3)){
						arc_in.push_back(1000 * char2uint(line[l]) + 100 * char2uint(line[l + 1]) + 10 * char2uint(line[l + 2]) + char2uint(line[l + 3]));
						l = l + 3;
						//}
					}
				}
			}
			l = l + 20;
			if (!is_next_char_digit(line, l)) {
				//arc_weight.push_back(char2uint(line[l]));
			}
			else {
				if (!is_next_char_digit(line, l + 1)) {
					//arc_weight.push_back(10*char2uint(line[l])+char2uint(line[l+1]));
					l++;
				}
				else {
					if (!is_next_char_digit(line, l + 2)) {
						//arc_weight.push_back(100*char2uint(line[l])+10*char2uint(line[l+1])+char2uint(line[l+2]));
						l = l + 2;
					}
					else {
						////if (!is_next_char_digit(line, l + 3)){
						//arc_weight.push_back(1000*char2uint(line[l])+100*char2uint(line[l+1])+10*char2uint(line[l+2])+char2uint(line[l+3]));
						l = l + 3;
						////}
					}
				}
			}
			l = l + 17;
		}
		if (!(line[l] == '<' && line[l + 1] == 'e')) {
			l = l + 18;
		}
		if (n == no_of_nodes)
			break;
	}
	cout << "the number of nodes" << n;
	//cout << "node_labels[n - 1] " << node_labels[n - 1] <<"\n";
	/*cout << "size of label postions" << label_positions.size();
	for (int i = 0; i < label_positions.size(); i++) {
		cout << label_positions[i] << endl;
	}
	cout << "size of node labels" << node_labels.size();
	for (int i = 0; i < node_labels.size(); i++) {
		cout << node_labels[i] << endl;
	}*/
	//int max_arc_in = *max_element(arc_in.begin(), arc_in.end());
	//int max_arc_out= *max_element(arc_out.begin(), arc_out.end());
	//cout << "max value of arc_in: " <<max_arc_in<< "\n"<<"max value of arc_out : "<<max_arc_out<<endl;

	//cout << "k: " << k << "\n";
	cout << "m: " << m << "\n";
	cout << "arc_in.size(): " << arc_in.size() << "\n" << " arc_out.size(): " << arc_out.size() << endl;
	return;
}

bool is_next_char_digit(string& line, unsigned int l) {
	if (line[l + 1] == '0' || line[l + 1] == '1' || line[l + 1] == '2' || line[l + 1] == '3' || line[l + 1] == '4' || line[l + 1] == '5' || line[l + 1] == '6' || line[l + 1] == '7' || line[l + 1] == '8' || line[l + 1] == '9')
		return true;
	return false;
}

//unsigned int char2uint(char &p){
//	return (int)p-48;
//}

unsigned int char2uint(char& p) {
	if (p == '1')
		return 1;
	else
		if (p == '2')
			return 2;
		else
			if (p == '3')
				return 3;
			else
				if (p == '4')
					return 4;
				else
					if (p == '5')
						return 5;
					else
						if (p == '6')
							return 6;
						else
							if (p == '7')
								return 7;
							else
								if (p == '8')
									return 8;
								else
									if (p == '9')
										return 9;
									else
										return 0;
}

void de2bi_card(unsigned int& k, vector<bool>& a, unsigned short int& n, unsigned short int& card) {
	vector<bool> zero(n, false);
	card = 0;
	a = zero;
	unsigned int i = 2;
	for (unsigned short int c = 0; c < n - 2; c++)
		i += i;
	unsigned int j = k + 1;
	unsigned short int l = n - 1;
	while (j > 0) {
		if (j >= i) {
			a[l] = true;
			card++;
			j -= i;
		}
		i /= 2;
		l--;
	}
	return;
}

double cpuTime() {
	return (double)clock() / CLOCKS_PER_SEC;
}

void nucl(bool& disp, unsigned short int& n, unsigned int& s, vector<double>& x, vector<double>& v, double& prec) {
	double min_satisfaction = 0;
	bool nlsu = false;
	vector<double> singleton_bounds(n, 0);
	double impu = 0;
	vector<double> excess(s, 0);
	vector<bool> unsettled(s + 1, true);
	unsettled[s] = false;
	vector<unsigned short int> zeros(s, 0);
	unsigned short int iter = 0;
	unsigned int piv = 0;
	unsigned int sr = 0;
	double t = 0;
	double t1 = cpuTime();
	for (unsigned short int i = 0; i < n; i++) {
		singleton_bounds[i] = v[pow(2, i) - 1];
		impu += singleton_bounds[i];
	}
	x = singleton_bounds;
	for (unsigned short int i = 0; i < n; i++)
		x[i] += (v[s] - impu) / n;
	vector<bool> a(n, false);
	zeros_mem(a, n, s, zeros);
	excess_init(excess, unsettled, x, v, s, n, zeros);
	nucl_comp(disp, n, s, excess, prec, unsettled, iter, piv, sr, t, x, a, t1, singleton_bounds, nlsu, min_satisfaction, zeros);
	return;
}

void nucl_comp(bool& disp, unsigned short int& n, unsigned int& s, vector<double>& excess, double& prec, vector<bool>& unsettled, unsigned short int& iter, unsigned int& piv, unsigned int& sr, double& t, vector<double>& x, vector<bool>& a, double& t1, vector<double>& singleton_bounds, bool& nlsu, double& min_satisfaction, vector<unsigned short int>& zeros) {
	vector<bool> unsettled_p(n, true);
	vector<vector<double>> Arref(n, vector<double>(n, 0));
	Arref[0] = vector<double>(n, 1);
	vector<bool>J(n, true);
	J[0] = false;
	unsigned short int rank = 1;
	vector<vector<bool>> Asettled(n, vector<bool>(n, 0));
	Asettled[0] = vector<bool>(n, true);
	if (disp) {
		cout << "Starting point:" << endl;
		for (unsigned short int i = 0; i < n; i++)
			cout << x[i] << endl;
	}
	vector<double> d(n, 0);
	double epsi = 0;
	double epsi_old = -DBL_MAX;
	vec_min_uns(epsi, excess, unsettled, s);
	vector<bool> T(s, false);
	vector<unsigned int> T_coord(0, 0);
	vector<bool> T2(n, false);
	vector<unsigned int> T2_coord(0, 0);
	unsigned int t_size = 0;
	tight_coal(T, excess, epsi, prec, s, T_coord, unsettled, t_size);
	unsigned short int t2_size = 0;
	tight_coal2(T2, x, singleton_bounds, prec, n, T2_coord, unsettled_p, t2_size);
	vector<vector<bool>> Atight(t_size, vector<bool>(n, false));
	for (unsigned int i = 0; i < t_size; i++)
		de2bi(T_coord[i], Atight[i], n);
	vector<vector<bool>> Atight2(t2_size, vector<bool>(n, false));
	for (unsigned int i = 0; i < t2_size; i++)
		de2bi(T2_coord[i], Atight2[i], n);
	vector<bool> U(t_size, true);
	vector<bool> U2(t2_size, true);
	while (rank < n)
		pivot(epsi, s, excess, prec, n, a, Arref, J, unsettled, rank, d, x, disp, Asettled, piv, sr, iter, unsettled_p, singleton_bounds, epsi_old, nlsu, zeros, T, T_coord, T2, T2_coord, t_size, t2_size, Atight, Atight2, U, U2, min_satisfaction);
	//cout << "BNF finished!" << endl;
	if (disp) {
		cout << "The nucleolus solution:" << endl;
		for (unsigned short int i = 0; i < n; i++)
			cout << x[i] << endl;
		//cout << "Time needed: " << t << " seconds" << endl;
		cout << "Iterations needed: " << iter << endl;
		cout << "Pivots needed: " << piv << endl;
		cout << "Subroutine solves needed: " << sr << endl;
	}
	return;
}

void zeros_mem(vector<bool>& a, unsigned short int& n, unsigned int& s, vector<unsigned short int>& zeros) {
	a[0] = true;
	zeros[0] = 1;
	for (unsigned int j = 1; j != s + 1; j++) {
		for (unsigned short int i = 0; i < n; i++) {
			if (!a[i]) {
				zeros[j] = i;
				break;
			}
		}
		a[zeros[j]] = 1;
		for (unsigned short int i = 0; i < zeros[j]; i++)
			a[i] = 0;
	}
	return;
}

void excess_init(vector<double>& exc, vector<bool>& unsettled, vector<double>& x, vector<double>& v, unsigned int& S, unsigned short int& N, vector<unsigned short int>& zeros) {
	double ax = x[0];
	vector<double> Ux(N, 0);
	for (unsigned short int i = 0; i < N; i++) {
		Ux[i] = x[i];
		for (unsigned short int j = 0; j < i; j++)
			Ux[i] -= x[j];
	}
	exc[0] = ax - v[0];
	for (unsigned int i = 1; i < S; i++) {
		ax += Ux[zeros[i]];
		if (unsettled[i])
			exc[i] = ax - v[i];
		else
			exc[i] = DBL_MAX;
	}
	return;
}

void vec_min_uns(double& m, vector<double>& x, vector<bool>& unsettled, unsigned int& s) {
	m = DBL_MAX;
	for (unsigned int i = 0; i < s; i++) {
		if (unsettled[i] && x[i] < m)
			m = x[i];
	}
	return;
}

void tight_coal(vector<bool>& T, vector<double>& excess, double& epsi, double& prec, unsigned int& s, vector<unsigned int>& T_coord, vector<bool>& unsettled, unsigned int& t_size) {
	for (unsigned int i = 0; i < s; i++) {
		if (unsettled[i]) {
			if (abs(excess[i] - epsi) < prec) {
				t_size++;
				T[i] = true;
				T_coord.push_back(i);
			}
		}
	}
	return;
}

void tight_coal2(vector<bool>& T2, vector<double>& x, vector<double>& singleton_bounds, double& prec, unsigned short int& n, vector<unsigned int>& T2_coord, vector<bool>& unsettled_p, unsigned short int& t2_size) {
	for (unsigned int i = 0; i < n; i++) {
		if (unsettled_p[i]) {
			if (abs(x[i] - singleton_bounds[i]) < prec) {
				t2_size++;
				T2[i] = true;
				T2_coord.push_back(pow(2, i) - 1);
			}
		}
	}
	return;
}

void pivot(double& epsi, unsigned int& s, vector<double>& excess, double& prec, unsigned short int& n, vector<bool>& a, vector<vector<double>>& Arref, vector<bool>& J, vector<bool>& unsettled, unsigned short int& rank, vector<double>& d, vector<double>& x, bool& disp, vector<vector<bool>>& Asettled, unsigned int& piv, unsigned int& sr_count, unsigned short int& iter, vector<bool>& unsettled_p, vector<double>& singleton_bounds, double& epsi_old, bool& nlsu, vector<unsigned short int>& zeros, vector<bool>& T, vector<unsigned int>& T_coord, vector<bool>& T2, vector<unsigned int>& T2_coord, unsigned int& t_size, unsigned short int& t2_size, vector<vector<bool>>& Atight, vector<vector<bool>>& Atight2, vector<bool>& U, vector<bool>& U2, double& min_satisfaction) {
	if (disp)
		cout << "Epsilon: " << epsi << endl;
	bool u = true;
	bool settled = false;
	subroutine(U, U2, Atight, Atight2, Arref, J, prec, n, t_size, t2_size, rank, disp, Asettled, sr_count, u, s, T_coord, T2_coord, unsettled, epsi_old, epsi, unsettled_p, settled, nlsu);
	if (disp)
		cout << endl << "   ---===   SUBROUTINE FINISHED   ===---   " << endl << endl;
	if (settled) {
		iter++;
		if (iter == 1)
			min_satisfaction = epsi;
	}
	if (disp) {
		cout << "T:" << endl;
		for (unsigned int i = 0; i < t_size; i++) {
			if (!U[i])
				cout << T_coord[i] + 1 << endl;
		}
		cout << "U:" << endl;
		for (unsigned int i = 0; i < t_size; i++) {
			if (U[i])
				cout << T_coord[i] + 1 << endl;
		}
		cout << "T0:" << endl;
		for (unsigned int i = 0; i < t2_size; i++) {
			if (!U2[i])
				cout << T2_coord[i] + 1 << endl;
		}
		cout << "U0:" << endl;
		for (unsigned int i = 0; i < t2_size; i++) {
			if (U2[i])
				cout << T2_coord[i] + 1 << endl;
		}
	}
	if (u) {
		piv++;
		if (disp)
			cout << endl << "   ---===   SOLVING IMPROVING DIRECTION LP   ===---   " << endl << endl;
		imprdir(d, n, t_size, t2_size, Atight, Atight2, U, U2, rank, Asettled, disp);
		if (disp)
			cout << endl << "   ---===   IMPROVING DIRECTION OBTAINED   ===---   " << endl << endl;
		if (disp) {
			cout << "Improving direction:" << endl;
			for (unsigned short int i = 0; i < n; i++) {
				cout << d[i] << "    ";
			}
			cout << endl;
		}
		if (disp)
			cout << endl << "   ---===   COMPUTING STEP SIZE   ===---   " << endl << endl;
		step(T, T2, unsettled, unsettled_p, s, epsi, excess, d, n, x, singleton_bounds, disp, prec, zeros, T_coord, t_size, T2_coord, t2_size, Atight, Atight2, U, U2);
	}
	else {
		if (disp)
			cout << "Min tight set found! Rank increased to: " << rank << endl;
		if (rank == n)
			return;
		if (!nlsu) {
			a[0] = true;
			for (unsigned short int i = 1; i < n; i++)
				a[i] = false;
			for (unsigned int i = 0; i < s; i++) {
				if (unsettled[i]) {
					//de2bi(i, a, n);
					if (!(binrank(Arref, J, a, n, rank))) {
						unsettled[i] = false;
						unsettled[s - 1 - i] = false;
					}
				}
				a[zeros[i + 1]] = 1;
				for (unsigned short int j = 0; j < zeros[i + 1]; j++)
					a[j] = 0;
			}
		}
		for (unsigned short int i = 0; i < n; i++)
			if (unsettled_p[i] == true && unsettled[pow(2, i) - 1] == false)
				unsettled_p[i] = false;
		vec_min_uns(epsi, excess, unsettled, s);
		T = vector<bool>(s, false);
		T_coord.clear();
		T2 = vector<bool>(n, false);
		T2_coord.clear();
		t_size = 0;
		tight_coal(T, excess, epsi, prec, s, T_coord, unsettled, t_size);
		t2_size = 0;
		if (epsi > prec || epsi < -prec)
			tight_coal2(T2, x, singleton_bounds, prec, n, T2_coord, unsettled_p, t2_size);
		Atight = vector<vector<bool>>(t_size, vector<bool>(n, false));
		for (unsigned int i = 0; i < t_size; i++)
			de2bi(T_coord[i], Atight[i], n);
		Atight2 = vector<vector<bool>>(t2_size, vector<bool>(n, false));
		for (unsigned int i = 0; i < t2_size; i++)
			de2bi(T2_coord[i], Atight2[i], n);
		U = vector<bool>(t_size, true);
		U2 = vector<bool>(t2_size, true);
	}
	return;
}

void subroutine(vector<bool>& U, vector<bool>& U2, vector<vector<bool>>& Atight, vector<vector<bool>>& Atight2, vector<vector<double>>& Arref, vector<bool>& J, double& prec, unsigned short int& n, unsigned int& tight_size, unsigned short int& tight2_size, unsigned short int& rank, bool& disp, vector<vector<bool>>& Asettled, unsigned int& sr_count, bool& u, unsigned int& s, vector<unsigned int>& T_coord, vector<unsigned int>& T2_coord, vector<bool>& unsettled, double& epsi_old, double& epsi, vector<bool>& unsettled_p, bool& settled, bool& nlsu) {
	unsigned int sumt = 0;
	vector<bool> t(tight_size, false);
	unsigned int sumt2 = 0;
	vector<bool> t2(tight2_size, false);
	glp_prob* lp;
	lp = glp_create_prob();
	//glp_set_prob_name(lp, "sr");
	//glp_set_obj_name(lp, "obj");
	glp_set_obj_dir(lp, GLP_MAX);
	glp_add_rows(lp, n + 1);
	glp_add_cols(lp, tight_size + tight2_size + rank);
	for (unsigned short int i = 1; i < n + 1; i++)
		glp_set_row_bnds(lp, i, GLP_FX, 0, 0);
	glp_set_row_bnds(lp, n + 1, GLP_FX, 1, 1);
	for (unsigned int i = 1; i < tight_size + tight2_size + 1; i++) {
		glp_set_col_bnds(lp, i, GLP_LO, 0, DBL_MAX);
		glp_set_obj_coef(lp, i, 1);
	}
	for (unsigned int i = tight_size + tight2_size + 1; i < tight_size + tight2_size + rank + 1; i++) {
		glp_set_col_bnds(lp, i, GLP_FR, -DBL_MAX, DBL_MAX);
		glp_set_obj_coef(lp, i, 0);
	}
	vector<int> ia((n + 1) * (tight_size + tight2_size + rank) + 1, 0);
	vector<int> ja((n + 1) * (tight_size + tight2_size + rank) + 1, 0);
	vector<double> ar((n + 1) * (tight_size + tight2_size + rank) + 1, 0);
	vector<bool> ar0pos(tight_size, false);
	unsigned int count = 0;
	for (unsigned int j = 1; j < tight_size + 1; j++) {
		for (unsigned short int i = 1; i < n + 1; i++) {
			count++;
			ia[count] = i;
			ja[count] = j;
			if (Atight[j - 1][i - 1]) {
				ar[count] = 1;
			}
			else {
				ar[count] = 0;
			}
		}
	}
	for (unsigned short int j = 1; j < tight2_size + 1; j++) {
		for (unsigned short int i = 1; i < n + 1; i++) {
			count++;
			ia[count] = i;
			ja[count] = j + tight_size;
			if (Atight2[j - 1][i - 1]) {
				ar[count] = 1;
			}
			else {
				ar[count] = 0;
			}
		}
	}
	for (unsigned short int j = 1; j < rank + 1; j++) {
		for (unsigned short int i = 1; i < n + 1; i++) {
			count++;
			ia[count] = i;
			ja[count] = j + tight_size + tight2_size;
			if (Asettled[j - 1][i - 1]) {
				ar[count] = 1;
			}
			else {
				ar[count] = 0;
			}
		}
	}
	for (unsigned int j = 1; j < tight_size + 1; j++) {
		count++;
		ia[count] = n + 1;
		ja[count] = j;
		ar[count] = 1;
	}
	for (unsigned int j = tight_size + 1; j < tight_size + tight2_size + rank + 1; j++) {
		count++;
		ia[count] = n + 1;
		ja[count] = j;
		ar[count] = 0;
	}
	int* ia_arr = ia.data();
	int* ja_arr = ja.data();
	double* ar_arr = ar.data();
	glp_load_matrix(lp, count, ia_arr, ja_arr, ar_arr);
	if (disp)
		cout << endl << "  --==  solving subroutine LP  ==--  " << endl << endl;
	glp_smcp parm;
	glp_init_smcp(&parm);
	if (!disp)
		parm.msg_lev = GLP_MSG_OFF;
	parm.presolve = GLP_ON;
	glp_simplex(lp, &parm);
	bool feas = false;
	if (glp_get_prim_stat(lp) == 2)
		feas = true;
	if (disp)
		cout << "subroutine feasibility: " << feas << endl;
	if (feas && nlsu)
		settled = true;
	sr_count++;
	unsigned int i;
	unsigned short int rank_old = rank;
	while (feas) {
		subr_upd(Arref, J, i, n, prec, U, U2, sumt, sumt2, t, t2, Atight, Atight2, tight_size, tight2_size, rank, unsettled, Asettled, disp, s, T_coord, T2_coord, epsi_old, epsi, unsettled_p, settled, lp, ar0pos);
		if (rank == n) {
			u = false;
			glp_delete_prob(lp);
			glp_free_env();
			return;
		}
		for (unsigned int i = 0; i < tight_size; i++) {
			if (ar0pos[i]) {
				ar[count - tight_size - tight2_size - rank_old + i + 1] = 0;
				ar0pos[i] = false;
			}
		}
		if (sumt < tight_size) {
			i = 0;
			while (i < tight_size) {
				if (t[i] == false) {
					if (!(binrank(Arref, J, Atight[i], n, rank))) {
						U[i] = false;
						t[i] = true;
						ar[count - tight_size - tight2_size - rank_old + i + 1] = 0;
						glp_set_obj_coef(lp, i + 1, 0);
						sumt++;
						unsettled[T_coord[i]] = false;
						unsettled[s - 1 - T_coord[i]] = false;
						if (disp)
							cout << T_coord[i] + 1 << " and " << s - T_coord[i] << " got settled without rank increase." << endl;
						if (sumt == tight_size && sumt2 == tight2_size) {
							u = false;
							glp_delete_prob(lp);
							glp_free_env();
							return;
						}
					}
				}
				i++;
			}
			i = 0;
			while (i < tight2_size) {
				if (t2[i] == false) {
					if (!(binrank(Arref, J, Atight2[i], n, rank))) {
						U2[i] = false;
						t2[i] = true;
						glp_set_obj_coef(lp, tight_size + i + 1, 0);
						sumt2++;
						unsettled[T2_coord[i]] = false;
						unsettled[s - 1 - T2_coord[i]] = false;
						if (disp)
							cout << T2_coord[i] + 1 << " and " << s - T2_coord[i] << " got settled without rank increase." << endl;
						if (sumt == tight_size && sumt2 == tight2_size) {
							u = false;
							glp_delete_prob(lp);
							glp_free_env();
							return;
						}
					}
				}
				i++;
			}
			for (unsigned short int i = 0; i < n; i++)
				if (unsettled_p[i] == true && unsettled[pow(2, i) - 1] == false)
					unsettled_p[i] = false;
			glp_load_matrix(lp, count, ia_arr, ja_arr, ar_arr);
			if (disp)
				cout << endl << "  --==  solving subroutine LP again  ==--  " << endl << endl;
			glp_simplex(lp, &parm);
			feas = false;
			if (glp_get_prim_stat(lp) == 2)
				feas = true;
			if (disp)
				cout << "subroutine feasibility: " << feas << endl;
			sr_count++;
		}
		else {
			u = false;
			glp_delete_prob(lp);
			glp_free_env();
			return;
		}
	}
	glp_delete_prob(lp);
	glp_free_env();
	return;
}

void subr_upd(vector<vector<double>>& Arref, vector<bool>& J, unsigned int& i, unsigned short int& n, double& prec, vector<bool>& U, vector<bool>& U2, unsigned int& sumt, unsigned int& sumt2, vector<bool>& t, vector<bool>& t2, vector<vector<bool>>& Atight, vector<vector<bool>>& Atight2, unsigned int& tight_size, unsigned short int& tight2_size, unsigned short int& rank, vector<bool>& unsettled, vector<vector<bool>>& Asettled, bool& disp, unsigned int& s, vector<unsigned int>& T_coord, vector<unsigned int>& T2_coord, double& epsi_old, double& epsi, vector<bool>& unsettled_p, bool& settled, glp_prob*& lp, vector<bool>& ar0pos) {
	i = 0;
	vector<double> lambdi(tight_size + tight2_size, 0);
	for (unsigned int j = 0; j < tight_size; j++) {
		if (t[j] == false)
			lambdi[j] = glp_get_col_prim(lp, j + 1);
	}
	for (unsigned short int j = 0; j < tight2_size; j++) {
		if (t2[j] == false)
			lambdi[j + tight_size] = glp_get_col_prim(lp, tight_size + j + 1);
	}
	while (i < tight_size && sumt < tight_size) {
		if (lambdi[i] > prec) {
			U[i] = false;
			t[i] = true;
			ar0pos[i] = true;
			//ar[count - tight_size - tight2_size - rank + i + 1] = 0;
			glp_set_obj_coef(lp, i + 1, 0);
			sumt++;
			unsettled[T_coord[i]] = false;
			unsettled[s - 1 - T_coord[i]] = false;
			if (binrank2(Arref, J, Atight[i], n, rank)) {
				rank++;
				if (epsi > epsi_old) {
					settled = true;
					epsi_old = epsi;
				}
				if (disp)
					cout << "lambda_" << T_coord[i] + 1 << " > 0, rank = " << rank << " (" << s - T_coord[i] << " settled as well)" << endl;
				//rowechform(Arref, J, Atight[i], n, rank);
				Asettled[rank - 1] = Atight[i];
				if (disp) {
					cout << "Arref:" << endl;
					for (unsigned short int j = 0; j < n; j++) {
						for (unsigned short int k = 0; k < n; k++) {
							cout << Arref[j][k] << " ";
						}
						cout << endl;
					}
					cout << "J: ";
					for (unsigned short int j = 0; j < n; j++)
						cout << J[j] << " ";
					cout << endl;
				}
				if (rank == n) {
					if (disp)
						cout << "Rank condition satisfied!" << endl;
					return;
				}
				glp_set_col_bnds(lp, i + 1, GLP_FR, -DBL_MAX, DBL_MAX);
			}
			else {
				if (disp)
					cout << "lambda_" << T_coord[i] + 1 << " > 0, got settled (with " << s - T_coord[i] << ") without rank increase" << endl;
			}
		}
		i++;
	}
	i = 0;
	while (i < tight2_size && sumt2 < tight2_size) {
		if (lambdi[i + tight_size] > prec) {
			U2[i] = false;
			t2[i] = true;
			sumt2++;
			glp_set_obj_coef(lp, tight_size + i + 1, 0);
			unsettled[T2_coord[i]] = false;
			unsettled[s - 1 - T2_coord[i]] = false;
			if (binrank2(Arref, J, Atight2[i], n, rank)) {
				rank++;
				if (epsi > epsi_old) {
					settled = true;
					epsi_old = epsi;
				}
				if (disp)
					cout << "lambda_" << T2_coord[i] + 1 << " > 0, rank = " << rank << " (" << s - T2_coord[i] << " settled as well)" << endl;
				//rowechform(Arref, J, Atight2[i], n, rank);
				Asettled[rank - 1] = Atight2[i];
				if (disp) {
					cout << "Arref:" << endl;
					for (unsigned short int j = 0; j < n; j++) {
						for (unsigned short int k = 0; k < n; k++) {
							cout << Arref[j][k] << " ";
						}
						cout << endl;
					}
					cout << "J: ";
					for (unsigned short int j = 0; j < n; j++)
						cout << J[j] << " ";
					cout << endl;
				}
				if (rank == n) {
					if (disp)
						cout << "Rank condition satisfied!" << endl;
					return;
				}
				glp_set_col_bnds(lp, tight_size + i + 1, GLP_FR, -DBL_MAX, DBL_MAX);
			}
			else {
				if (disp)
					cout << "lambda_" << T_coord[i] + 1 << " > 0, got settled (with " << s - T_coord[i] << ") without rank increase" << endl;
			}
		}
		i++;
	}
	for (unsigned short int i = 0; i < n; i++)
		if (unsettled_p[i] == true && unsettled[pow(2, i) - 1] == false)
			unsettled_p[i] = false;
	return;
}

void step(vector<bool>& T, vector<bool>& T2, vector<bool>& unsettled, vector<bool>& unsettled_p, unsigned int& s, double& epsi, vector<double>& excess, vector<double>& d, unsigned short int& n, vector<double>& x, vector<double>& singleton_bounds, bool& disp, double& prec, vector<unsigned short int>& zeros, vector<unsigned int>& T_coord, unsigned int& t_size, vector<unsigned int>& T2_coord, unsigned short int& t2_size, vector<vector<bool>>& Atight, vector<vector<bool>>& Atight2, vector<bool>& U, vector<bool>& U2) {
	double ad = d[0];
	vector<double> Ud(n, 0);
	double alpha = DBL_MAX;
	double alpha1 = DBL_MAX;
	double alpha2 = DBL_MAX;
	vector<unsigned int> argmin(0, 0);
	vector<unsigned int> argmin2(0, 0);
	for (unsigned short int i = 0; i < n; i++) {
		Ud[i] = d[i];
		for (unsigned short int j = 0; j < i; j++)
			Ud[i] -= d[j];
	}
	if ((ad < 1 - prec) && unsettled[0] && !T2[0] && !T[0]) {
		alpha1 = (epsi - excess[0]) / (ad - 1);
		argmin.push_back(0);
	}
	for (unsigned int j = 1; j < s; j++) {
		ad += Ud[zeros[j]];
		if ((ad < 1 - prec) && unsettled[j] && !T[j])
			if (alpha1 - prec > (epsi - excess[j]) / (ad - 1)) {
				alpha1 = (epsi - excess[j]) / (ad - 1);
				argmin = vector<unsigned int>(1, 0);
				argmin[0] = j;
			}
			else {
				if (abs(alpha1 - (epsi - excess[j]) / (ad - 1)) < prec)
					argmin.push_back(j);
			}
	}
	for (unsigned short int i = 0; i < n; i++) {
		if (!T2[i] && unsettled_p[i] && d[i] < -prec) {
			if ((singleton_bounds[i] - x[i]) / d[i] < alpha2 - prec) {
				alpha2 = (singleton_bounds[i] - x[i]) / d[i];
				argmin2 = vector<unsigned int>(1, 0);
				argmin2[0] = i;
			}
			else {
				if (abs((singleton_bounds[i] - x[i]) / d[i] - alpha2) < prec)
					argmin2.push_back(i);
			}
		}
	}
	if (alpha1 < alpha2 - prec)
		alpha = alpha1;
	else {
		if (alpha2 < alpha1 - prec)
			alpha = alpha2;
		else
			alpha = alpha1;
	}
	if (disp)
		cout << "Step size: " << alpha << endl;
	if (disp)
		cout << endl << "  --==  step size obtained  ==--  " << endl << endl;
	for (unsigned short int i = 0; i < n; i++)
		x[i] += alpha * d[i];
	if (disp) {
		cout << "New point: " << endl;
		for (unsigned short int i = 0; i < n; i++)
			cout << x[i] << endl;
	}
	ad = d[0];
	if (unsettled[0]) {
		excess[0] += alpha * ad;
		if (ad > 1 + prec && T[0]) {
			for (unsigned int i = 0; i < t_size; i++) {
				if (T_coord[i] == 0) {
					T[0] = false;
					T_coord.erase(T_coord.begin() + i);
					Atight.erase(Atight.begin() + i);
					U.erase(U.begin() + i);
					t_size--;
					break;
				}
			}
		}
	}
	else {
		if (T[0]) {
			for (unsigned int i = 0; i < t_size; i++) {
				if (T_coord[i] == 0) {
					T[0] = false;
					T_coord.erase(T_coord.begin() + i);
					Atight.erase(Atight.begin() + i);
					U.erase(U.begin() + i);
					t_size--;
					break;
				}
			}
		}
	}
	for (unsigned int j = 1; j < s; j++) {
		ad += Ud[zeros[j]];
		if (unsettled[j]) {
			excess[j] += alpha * ad;
			if (ad > 1 + prec && T[j]) {
				for (unsigned int i = 0; i < t_size; i++) {
					if (T_coord[i] == j) {
						T[j] = false;
						T_coord.erase(T_coord.begin() + i);
						Atight.erase(Atight.begin() + i);
						U.erase(U.begin() + i);
						t_size--;
						break;
					}
				}
			}
		}
		else {
			if (T[j]) {
				for (unsigned int i = 0; i < t_size; i++) {
					if (T_coord[i] == j) {
						T[j] = false;
						T_coord.erase(T_coord.begin() + i);
						Atight.erase(Atight.begin() + i);
						U.erase(U.begin() + i);
						t_size--;
						break;
					}
				}
			}
		}
	}
	for (unsigned short int j = 0; j < n; j++) {
		if (unsettled_p[j] && T2[j] && d[j] > prec) {
			for (unsigned short int i = 0; i < t2_size; i++) {
				if (T2_coord[i] == pow(2, j) - 1) {
					T2[j] = false;
					T2_coord.erase(T2_coord.begin() + i);
					Atight2.erase(Atight2.begin() + i);
					U2.erase(U2.begin() + i);
					t2_size--;
					break;
				}
			}
		}
		else {
			if (!unsettled_p[j] && T2[j]) {
				for (unsigned short int i = 0; i < t2_size; i++) {
					if (T2_coord[i] == pow(2, j) - 1) {
						T2[j] = false;
						T2_coord.erase(T2_coord.begin() + i);
						Atight2.erase(Atight2.begin() + i);
						U2.erase(U2.begin() + i);
						t2_size--;
						break;
					}
				}
			}
		}
	}
	for (unsigned int i = 0; i < t_size; i++)
		U[i] = true;
	for (unsigned short int i = 0; i < t2_size; i++)
		U2[i] = true;
	epsi += alpha;
	if (alpha1 < alpha2 - prec) {
		for (unsigned int j = 0; j < argmin.size(); j++) {
			T[argmin[j]] = true;
			T_coord.push_back(argmin[j]);
			Atight.push_back(vector<bool>(n, false));
			de2bi(argmin[j], Atight[t_size], n);
			U.push_back(true);
			t_size++;
		}
	}
	else {
		if (alpha2 < alpha1 - prec) {
			for (unsigned int j = 0; j < argmin2.size(); j++) {
				T2[argmin2[j]] = true;
				T2_coord.push_back(pow(2, argmin2[j]) - 1);
				Atight2.push_back(vector<bool>(n, false));
				de2bi(T2_coord[t2_size], Atight2[t2_size], n);
				U2.push_back(true);
				t2_size++;
			}
		}
		else {
			for (unsigned int j = 0; j < argmin.size(); j++) {
				T[argmin[j]] = true;
				T_coord.push_back(argmin[j]);
				Atight.push_back(vector<bool>(n, false));
				de2bi(argmin[j], Atight[t_size], n);
				U.push_back(true);
				t_size++;
			}
			for (unsigned int j = 0; j < argmin2.size(); j++) {
				T2[argmin2[j]] = true;
				T2_coord.push_back(pow(2, argmin2[j]) - 1);
				Atight2.push_back(vector<bool>(n, false));
				de2bi(T2_coord[t2_size], Atight2[t2_size], n);
				U2.push_back(true);
				t2_size++;
			}
		}
	}
	return;
}

void imprdir(vector<double>& d, unsigned short int& n, unsigned int& t_size, unsigned short int& t2_size, vector<vector<bool>>& Atight, vector<vector<bool>>& Atight2, vector<bool>& U, vector<bool>& U2, unsigned short int& rank, vector<vector<bool>>& Asettled, bool& disp) {
	glp_prob* dir_lp;
	dir_lp = glp_create_prob();
	glp_set_obj_dir(dir_lp, GLP_MIN);
	glp_add_cols(dir_lp, n);
	glp_add_rows(dir_lp, t_size + t2_size + rank);
	for (unsigned int i = 1; i < t_size + 1; i++) {
		if (U[i - 1]) {
			glp_set_row_bnds(dir_lp, i, GLP_LO, 1, DBL_MAX);
		}
		else {
			glp_set_row_bnds(dir_lp, i, GLP_FX, 0, 0);
		}
	}
	for (unsigned short int i = 1; i < t2_size + 1; i++)
		glp_set_row_bnds(dir_lp, t_size + i, GLP_LO, 0, DBL_MAX);
	for (unsigned short int i = 1; i < rank + 1; i++)
		glp_set_row_bnds(dir_lp, t_size + t2_size + i, GLP_FX, 0, 0);
	for (unsigned short int i = 1; i < n + 1; i++)
		glp_set_col_bnds(dir_lp, i, GLP_FR, -DBL_MAX, DBL_MAX);
	vector<unsigned int> sumd(n, 0);
	for (unsigned int i = 0; i < t_size; i++) {
		if (U[i]) {
			for (unsigned short int j = 0; j < n; j++) {
				if (Atight[i][j])
					sumd[j]++;
			}
		}
	}
	for (unsigned short int i = 1; i < n + 1; i++)
		glp_set_obj_coef(dir_lp, i, sumd[i - 1]);
	vector<int> ia(n * (t_size + t2_size + rank) + 1, 0);
	vector<int> ja(n * (t_size + t2_size + rank) + 1, 0);
	vector<double> ar(n * (t_size + t2_size + rank) + 1, 0);
	unsigned int count = 0;
	for (unsigned int i = 1; i < t_size + 1; i++) {
		for (unsigned short int j = 1; j < n + 1; j++) {
			count++;
			ia[count] = i;
			ja[count] = j;
			if (Atight[i - 1][j - 1])
				ar[count] = 1;
			else
				ar[count] = 0;
		}
	}
	for (unsigned int i = 1; i < t2_size + 1; i++) {
		for (unsigned short int j = 1; j < n + 1; j++) {
			count++;
			ia[count] = i + t_size;
			ja[count] = j;
			if (Atight2[i - 1][j - 1])
				ar[count] = 1;
			else
				ar[count] = 0;
		}
	}
	for (unsigned int i = 1; i < rank + 1; i++) {
		for (unsigned short int j = 1; j < n + 1; j++) {
			count++;
			ia[count] = i + t_size + t2_size;
			ja[count] = j;
			if (Asettled[i - 1][j - 1])
				ar[count] = 1;
			else
				ar[count] = 0;
		}
	}
	int* ia_arr = ia.data();
	int* ja_arr = ja.data();
	double* ar_arr = ar.data();
	glp_load_matrix(dir_lp, count, ia_arr, ja_arr, ar_arr);
	glp_smcp parm;
	glp_init_smcp(&parm);
	if (!disp)
		parm.msg_lev = GLP_MSG_OFF;
	glp_simplex(dir_lp, &parm);
	for (unsigned short int j = 1; j < n + 1; j++)
		d[j - 1] = glp_get_col_prim(dir_lp, j);
	glp_delete_prob(dir_lp);
	glp_free_env();
	return;
}

bool binrank(vector<vector<double>>& Arref, vector<bool>& J, vector<bool>& b, unsigned short int& n, unsigned short int& rank) {
	double prec = pow(10, -10);
	vector<double> B(n, 0);
	for (unsigned short int i = 0; i < n; i++) {
		if (b[i] == true)
			B[i] = 1;
	}
	// m = rank
	// pivot_col[i] = !J[i]
	if (rank >= n)
		return false;
	else {
		unsigned short int j = 0;
		vector<bool> piv(n, false);
		vector<double> aux(n, 0);
		unsigned short int k = 0;
		unsigned short int I = 0;
		unsigned int s = 0;
		unsigned short int ind = 0;
		unsigned short int count = 0;
		while (j < n) {
			for (unsigned short i = 0; i < n; i++) {
				if (B[i] > prec || B[i] < -prec)
					piv[i] = true;
			}
			sum_vecb(s, piv);
			if (s == 0)
				return false;
			else {
				while (k == 0) {
					if (piv[I] == true)
						k = I + 1;
					I++;
				}
				k--;
				I = 0;
				if (J[k] == true) {
					return true;
				}
				else {
					while (count < k + 1) {
						if (!J[count])
							ind++;
						count++;
					}
					ind--;
					count = 0;
					sc_vec_prod(aux, B[k] / Arref[ind][k], Arref[ind]);
					vec_subtract(B, B, aux);
					j++;
				}
			}
			for (unsigned short int l = 0; l < n; l++)
				piv[l] = false;
			k = 0;
			ind = 0;
		}
		return false;
	}
}

bool binrank2(vector<vector<double>>& Arref, vector<bool>& J, vector<bool>& b, unsigned short int& n, unsigned short int& rank) {
	double prec = pow(10, -10);
	vector<double> B(n, 0);
	for (unsigned short int i = 0; i < n; i++) {
		if (b[i] == true)
			B[i] = 1;
	}
	// m = rank
	// pivot_col[i] = !J[i]
	if (rank >= n)
		return false;
	else {
		unsigned short int j = 0;
		vector<bool> piv(n, false);
		vector<double> aux(n, 0);
		unsigned short int k = 0;
		unsigned short int I = 0;
		unsigned int s = 0;
		unsigned short int ind = 0;
		unsigned short int count = 0;
		while (j < n) {
			for (unsigned short i = 0; i < n; i++) {
				if (B[i] > prec || B[i] < -prec)
					piv[i] = true;
			}
			sum_vecb(s, piv);
			if (s == 0)
				return false;
			else {
				while (k == 0) {
					if (piv[I] == true)
						k = I + 1;
					I++;
				}
				k--;
				I = 0;
				if (J[k] == true) {
					for (unsigned short i = 0; i < k; i++)
						if (!J[i])
							I++;
					for (unsigned short i = 0; i < rank - I; i++)
						Arref[rank - i] = Arref[rank - i - 1];
					Arref[I] = B;
					J[k] = false;
					return true;
				}
				else {
					while (count < k + 1) {
						if (!J[count])
							ind++;
						count++;
					}
					ind--;
					count = 0;
					sc_vec_prod(aux, B[k] / Arref[ind][k], Arref[ind]);
					vec_subtract(B, B, aux);
					j++;
				}
			}
			for (unsigned short int l = 0; l < n; l++)
				piv[l] = false;
			k = 0;
			ind = 0;
		}
		return false;
	}
}

void sum_vecb(unsigned int& s, vector<bool>& x) {
	// sums up the values of boolean x
	s = 0;
	for (unsigned int i = 0; i < x.size(); i++)
		s += x[i];
	return;
}

void vec_subtract(vector<double>& z, vector<double>& x, vector<double>& y) {
	// subtracts vector (double) y from vector (double) x
	for (unsigned int i = 0; i != x.size(); i++)
		z[i] = x[i] - y[i];
	return;
}

void sc_vec_prod(vector<double>& y, double a, vector<double>& x) {
	for (unsigned int i = 0; i < x.size(); i++)
		y[i] = a * x[i];
	return;
}

void period_0(unsigned short int& Q, vector<unsigned short int>& no_of_active_nodes, unsigned short int& N, vector<unsigned short int>& s, unsigned short int& Vp, vector<unsigned short int>& node_arrives, ListGraph::NodeMap<bool>& active_nodes, ListDigraph::NodeMap<bool>& active_nodes_original, vector<ListGraph::Node>& c, vector<ListGraph::Node>& c_b, vector<ListDigraph::Node>& c_original, vector<double>& credit) {
	Q = 0;
	for (unsigned short int i = 0; i < N; i++) {
		s[i] = 0;
		credit[i] = 0;
		no_of_active_nodes[i] = Vp/4;
		for (unsigned short int j = 0; j < Vp; j++) {
			if (node_arrives[i * Vp + j] == 0) {
				active_nodes[c[i * Vp + j]] = true;
				active_nodes[c_b[i * Vp + j]] = true;
				active_nodes_original[c_original[i * Vp + j]] = true;
			}
			else {
				active_nodes[c[i * Vp + j]] = false;
				active_nodes[c_b[i * Vp + j]] = false;
				active_nodes_original[c_original[i * Vp + j]] = false;
			}
		}
	}
	return;
}

void min_d_1(vector<unsigned short int>& node_arrives, ListGraph& g, ListDigraph& g_original, vector<pair<int, int>>& arc_pair, vector<bool>& leaving, ListGraph::NodeMap<bool>& active_nodes, ListDigraph::NodeMap<bool>& active_nodes_original, vector<ListGraph::Node>& c, vector<ListGraph::Node>& c_b, vector<ListDigraph::Node>& c_original, bool& disp, vector<unsigned short int>& no_of_active_nodes, unsigned short int& N, unsigned short int& Vp, unsigned short int& periods, bool& dispy, vector<unsigned short int>& s, unsigned short int& Q, vector<double>& v, bool& initial_nucl, vector<double>& target, vector<double>& credit, ListGraph::EdgeMap<double>& edge_card_weight, double& t0, vector<vector<unsigned short int>>& actual_alloc, vector<int>& nodeset, vector<pair<int, int>>& cycle_distri, vector<double>& d, double& M_total, double& d_total, bool& c_involved, map<int, int>& cycle_dis, map<int, int>& numofMaxSolution, bool& arbitray_maximum, unsigned short int& initialSize,  unsigned int& S, double& core_100, long& negative_core, double &prec, vector<double> &init_alloc, double& d_c_total, unsigned short int inst, bool lex_min, double& max_d, double& game_generation, double& solution_concept_time, double& scenario_time, std::map<int, std::map<int, int>>& cycle_dis_period, std::map<int, std::map<int, int>>& cycle_dis_arbitrary_period) {
	Q = 0;
	d_total = 0;
	d_c_total = 0;
	M_total = 0;
	core_100 = 0;
	negative_core = 0;
	max_d = 0;
	if (dispy)
		cout << " --== Without lex min matching == -- " << endl;
	for (unsigned short int i = 0; i < N; i++) {
		d[i] = 0;
		s[i] = 0;
		credit[i] = 0;
		no_of_active_nodes[i] = initialSize;
		for (unsigned short int j = 0; j < Vp; j++) {
			if (node_arrives[i * Vp + j] == 0) {
				active_nodes[c[i * Vp + j]] = true;
				active_nodes[c_b[i * Vp + j]] = true;
				active_nodes_original[c_original[i * Vp + j]] = true;
			}
			else {
				active_nodes[c[i * Vp + j]] = false;
				active_nodes[c_b[i * Vp + j]] = false;
				active_nodes_original[c_original[i * Vp + j]] = false;
			}
		}
	}
	while (Q < periods) {
		if (dispy) {
			cout << "--== PERIOD " << Q + 1 << " ==--" << endl;
		}
		if (dispy) {
			cout << "Number of active nodes: ";
			for (unsigned short int i = 0; i < N; i++)
				cout << no_of_active_nodes[i] << " ";
			cout << endl;
		}
		// cooperative game and target
		cout << "start generating values" << endl;
		coop_game(g, v, S, s, c, c_b, edge_card_weight, dispy, Vp, N, active_nodes, leaving, numofMaxSolution, Q, arbitray_maximum, cycle_distri, cycle_dis, game_generation, cycle_dis_arbitrary_period);
		double t0 = cpuTime();
		if (initial_nucl)
			nucl(disp, N, S, init_alloc, v, prec);
		else {
			shapley(init_alloc, v, N, S);

		}
		double t1 = cpuTime();
		solution_concept_time += t1 - t0;
			

		/*if (core_dist(init_alloc, v, N, S) >= 0) {
			cout << "core_dist: " << core_dist(init_alloc, v, N, S) << endl;
			core_100 += core_dist(init_alloc, v, N, S);
		}
		else {
			if (abs(core_dist(init_alloc, v, N, S) > prec)) {
				cout << "error_core_dist: " << core_dist(init_alloc, v, N, S) << endl;
				negative_core += 1;
				cout << "error_core_dist_arbitrary: " << core_dist(init_alloc, v, N, S) << endl;
				std::string fileName = "Error_not_in_the_core.txt";
				std::ofstream outputFile(fileName);

				if (outputFile.is_open()) {
					// Write data to the file
					outputFile << N << "countries" << "," << "instance: " << inst << "Period: " << Q << "error_core_dist: " << core_dist(init_alloc, v, N, S) << endl;

					// Close the file
					outputFile.close();
				}
			}

		}*/


		//init_alloc[Q] = target;
		if (dispy) {
			if (initial_nucl) {
				cout << "nucl: ";
			}
			else {
				cout << "Shapley: ";
			}
			for (unsigned short int i = 0; i < N; i++) {
				cout << init_alloc[i] << " ";
			}
			cout << endl;
		}
		t0 = cpuTime();
		ILP_d1_gurobi(Q, N, g_original, Vp, node_arrives, active_nodes, active_nodes_original, arc_pair, nodeset, actual_alloc, v[S], M_total, s, cycle_distri, target, leaving, d, d_total, c_involved, credit, cycle_dis, init_alloc, lex_min, inst, cycle_dis_period);
		t1 = cpuTime();
		scenario_time += t1 - t0;
		std::string fileName = "output_credits_benefit_no_c.txt";
		std::ofstream outputFile(fileName);

		if (outputFile.is_open()) {
			// Write data to the file
			outputFile << "credits--Period: " << Q << endl;
			for (unsigned short int i = 0; i < N - 4; ++i) {
				outputFile << "credits" << credit[i] << endl;
			}

			// Close the file
			outputFile.close();

			std::cout << "Data has been written to the file." << std::endl;
		}
		else {
			std::cout << "Unable to open the file." << std::endl;
		}
		Q++;
		changing_nodes(active_nodes, active_nodes_original, leaving, no_of_active_nodes, N, Vp, node_arrives, Q, c, c_b, c_original, s, d, target);
		if (dispy)
			cin.get();
	}
	for (unsigned short int i = 0; i < N; ++i) {
		d_total += abs(d[i]);
		if (c_involved) {
			d_c_total+= abs(credit[i]);
		}
	}
	vector<double> max_deviation(N, 0);
	for (unsigned short int i = 0; i < N; ++i) {
		max_deviation[i] = abs(d[i]) / M_total;
	}
	std::sort(max_deviation.begin(), max_deviation.end());
	max_d = max_deviation[N - 1];
	return;
}

double core_dist(vector<double>& x, vector<double>& v, unsigned short int& N, unsigned int& S) {
	double eps = x[0] - v[0];
	if (x[1] - v[1] < eps)
		eps = x[1] - v[1];
	vector<bool> a(N, false);
	double xS = 0;
	for (unsigned int i = 2; i < S; i++) {
		de2bi(i, a, N);
		for (unsigned short int j = 0; j < N; j++) {
			if (a[j]) {
				xS += x[j];
			}

		}
		if (xS - v[i] < eps)
			eps = xS - v[i];
		xS = 0;
	}
	return eps;
}

void ILP_d1_gurobi(unsigned short int& Q, unsigned short int& N, ListDigraph& g_original, unsigned short int& Vp, vector<unsigned short int>& node_arrives, ListGraph::NodeMap<bool>& active_nodes, ListDigraph::NodeMap<bool>& active_nodes_original, vector<pair<int, int>>& arc_pair, vector<int>& nodeset, vector<vector<unsigned short int>>& actual_alloc, double& M, double& M_total, vector<unsigned short int>& s, vector<pair<int, int>>& cycle_distri, vector<double>& target, vector<bool>& leaving, vector<double>& d, double& d_total, bool& c_involved, vector<double>& credit, map<int, int>& cycle_dis, vector<double>& init_alloc, bool lex_min, unsigned short int inst, std::map<int, std::map<int, int>>& cycle_dis_period) {
	pair_arcs(Q, g_original, node_arrives, active_nodes_original, arc_pair, nodeset);
	FilterNodes<ListDigraph> sg(g_original, active_nodes_original);
	long col_num = countArcs(sg) + 1;
	cout << "column: " << col_num - 1 << endl;
	cout << "countArcs(g_original): " << countArcs(g_original) << endl;
	M_total += M;
	//int node_number = countNodes(g_original);
	const unsigned short int row_num = N + 2 * nodeset.size() + 1;
	// Create an environment
	GRBEnv env = GRBEnv(true);
	env.set("LogFile", "mip1.log");
	env.start();

	// Create an empty model
	GRBModel model = GRBModel(env);
	vector<GRBModel> vector_model(2 * N - 1, GRBEnv(env));
	// Create variables
	vector<GRBVar> var_bi(col_num + 2 * N);
	for (unsigned short int i = 0; i < col_num - 1; ++i) {
		var_bi[i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x" + to_string(i));
	}
	var_bi[col_num - 1] = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "x" + to_string(col_num - 1));
	// N difference variables
	for (unsigned short int i = col_num; i < col_num + N; ++i) {
		var_bi[i] = model.addVar(-GRB_INFINITY, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "x" + to_string(i));
	}
	// N abs variables
	for (unsigned short int i = col_num + N; i < col_num + 2 * N; ++i) {
		var_bi[i] = model.addVar(0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "x" + to_string(i));
	}
	model.setObjective(1 * var_bi[col_num - 1], GRB_MINIMIZE);
	vector<double> bound(row_num, 0);
	for (unsigned short int i = 1; i < N + 1; ++i) {
		bound[i - 1] = init_alloc[i - 1]+credit[i-1];
	}
	/*for (unsigned short int i = 1 + N; i < 2 * N + 1; ++i) {
		bound[i - 1] = target[i - 1];
	}*/
	for (unsigned short int i = N + 1; i < N + 1 + nodeset.size(); ++i) {
		bound[i - 1] = 0;
	}
	for (unsigned short int i = N + nodeset.size() + 1; i < N + 2 * (nodeset.size()) + 1; ++i) {
		bound[i - 1] = 1;
	}
	bound[row_num - 1] = M;
	cout << "size of max-weighted perfect matching: " << M << endl;
	long matrix_num = row_num * col_num;
	vector<int> ia(matrix_num + 1, 0);
	//cout << "ia.size(): " << ia.size();
	vector<int> ja(matrix_num + 1, 0);
	vector<double> ar(matrix_num + 1, 0);

	//cout << "Period" << to_string(Q) << endl;
	//cout << "N: " << N << "col_num: " << col_num << "N*col_num: " << N * col_num;
	long cnt_2 = 0;

	//cout << ":arc_pair.size(): " << arc_pair.size() << endl;
	/*for (unsigned short int k = 0; k < arc_pair.size(); ++k) {
		cout << "arc_pair[k].first: " << arc_pair[k].first << " " << "arc_pair[k].second: " << arc_pair[k].second << endl;
	}*/
	// sum_y[i]+d1>=target[i]
	for (int i = 1; i < N + 1; ++i) {
		for (int k = 1; k < arc_pair.size() + 1; ++k) {
			if ((i - 1) * Vp <= arc_pair[k - 1].second && arc_pair[k - 1].second < i * Vp) {
				++cnt_2;
				ia[cnt_2] = i;
				ja[cnt_2] = k;
				ar[cnt_2] = 1.0;
			}

		}
	}

	int cnt_2_row = N;
	for (int i = 0; i < nodeset.size(); ++i) {
		for (int k = 1; k < arc_pair.size() + 2; ++k) {
			if (k < arc_pair.size() + 1) {
				if (arc_pair[k - 1].first == nodeset[i]) {
					++cnt_2;
					ia[cnt_2] = cnt_2_row + i + 1;
					ja[cnt_2] = k;
					ar[cnt_2] = 1.0;
				}
				if (arc_pair[k - 1].second == nodeset[i]) {
					++cnt_2;
					ia[cnt_2] = cnt_2_row + i + 1;
					ja[cnt_2] = k;
					ar[cnt_2] = -1.0;
				}
			}

		}
	}



	int cnt_3_row = N + nodeset.size();
	for (int i = 0; i < nodeset.size(); ++i) {
		for (int k = 1; k < arc_pair.size() + 2; ++k) {
			if (k < arc_pair.size() + 1) {
				if (arc_pair[k - 1].second == nodeset[i]) {
					++cnt_2;
					ia[cnt_2] = cnt_3_row + i + 1;
					ja[cnt_2] = k;
					ar[cnt_2] = 1.0;
				}
			}


		}
	}


	int cnt_4_row = N + 2 * nodeset.size();
	for (int k = 1; k < arc_pair.size() + 2; ++k) {
		if (k < arc_pair.size() + 1) {
			++cnt_2;
			ia[cnt_2] = row_num;
			ja[cnt_2] = k;
			ar[cnt_2] = 1.0;
		}

	}

	vector<GRBLinExpr> sum_row(row_num, 0);

	for (long j = 1; j < cnt_2 + 1; ++j) {
		sum_row[ia[j] - 1] += ar[j] * var_bi[ja[j] - 1];
	}
	cout << "finish loading efficiencies" << endl;
	for (unsigned short int i = 1; i < N + 1; ++i) {
		model.addConstr(var_bi[col_num + i - 1] == sum_row[i - 1] - bound[i - 1]);
		model.addGenConstrAbs(var_bi[col_num + N + i - 1], var_bi[col_num + i - 1]);
		model.addConstr(var_bi[col_num + N + i - 1] <= var_bi[col_num - 1]);
	}

	for (unsigned short int i = N + 1; i < N + 1 + nodeset.size(); ++i) {
		model.addConstr(sum_row[i - 1] == bound[i - 1]);
	}
	for (unsigned short int i = N + nodeset.size() + 1; i < N + 2 * (nodeset.size()) + 1; ++i) {
		model.addConstr(sum_row[i - 1] <= bound[i - 1]);
	}
	model.addConstr(sum_row[row_num - 1] == bound[row_num - 1]);


	model.optimize();

	vector<double> d_t(N, 0);
	d_t[0] = var_bi[col_num - 1].get(GRB_DoubleAttr_X);
	std::cout << "d_t[0]" << d_t[0] << endl;
	vector<unsigned short int> N_star(N, 0);
	unsigned short int n_star = 0;
	unsigned short int t_star = 0;
	double epsilon = 0;
	unsigned short int track = 0;
	vector<GRBVar> var_lexmin(arc_pair.size());
	for (unsigned short int i = 0; i < N; ++i) {
		cout << "init_alloc[i]: " << init_alloc[i] << endl;
		cout << "credit[i]: " << credit[i] << endl;
	}
	if (d_t[0] > 0.5 && lex_min) {
		epsilon_func(init_alloc, credit, epsilon, N);
		sort_d_t(d_t, var_bi, col_num, N, Vp, arc_pair, init_alloc, t_star, credit, epsilon, var_lexmin, N_star);
	}
	std::cout << "finish sorting" << "epsilon:" << epsilon << endl;
	std::cout << "start n_star_1" << endl;
	if (lex_min && d_t[0] > 0.5 && abs(epsilon) > pow(10, -4)) {
		lex_min_n_star(d_t, lex_min, t_star, N, col_num, epsilon, n_star, model, ia, ja, ar, row_num, cnt_2, bound, nodeset, N_star, var_lexmin, vector_model, track);
	}
	std::cout << "finish n_star_1" << endl;
	lexmin_searching(d_t, lex_min, t_star, N, col_num, epsilon, n_star, model, ia, ja, ar, row_num, cnt_2, bound, nodeset, N_star, Vp, arc_pair, init_alloc, credit, var_lexmin, inst, vector_model, track);

	cout << "finish lexmin_searching" << endl;

	unsigned short int t = 0;
	for (unsigned short int i = 1; i < arc_pair.size() + 1; ++i) {
		//cout << "{" << arc_pair[i].first << "," << arc_pair[i].second << "}" << endl;
		//cout << var_bi[i - 1].get(GRB_DoubleAttr_X);
		if (lex_min && d_t[0] > 0.5) {
			if (var_lexmin[i - 1].get(GRB_DoubleAttr_X) > pow(10, -4)) {
				++t;
				leaving[arc_pair[i - 1].first] = true;
				leaving[arc_pair[i - 1].second] = true;
				cycle_distri.push_back(arc_pair[i - 1]);
				for (unsigned short int j = 0; j < N; ++j) {
					if (j * Vp <= arc_pair[i - 1].second && arc_pair[i - 1].second < (j + 1) * Vp) {
						++s[j];
					}
				}
			}

		}
		else {
			if (var_bi[i - 1].get(GRB_DoubleAttr_X) > pow(10, -4)) {
				++t;
				leaving[arc_pair[i - 1].first] = true;
				leaving[arc_pair[i - 1].second] = true;
				cycle_distri.push_back(arc_pair[i - 1]);
				for (unsigned short int j = 0; j < N; ++j) {
					if (j * Vp <= arc_pair[i - 1].second && arc_pair[i - 1].second < (j + 1) * Vp) {
						++s[j];
					}
				}
			}
		}

	}
	model.reset();
	model.update();
	for (unsigned short int i = 0; i < track + 1; ++i) {
		vector_model[i].reset();
		vector_model[i].update();
	}


	/*for (unsigned short int i = 0; i < cycle_distri.size(); ++i) {
		cout << "{" << cycle_distri[i].first << "," << cycle_distri[i].second << "}" << endl;
	}*/
	cycle_distribution(cycle_dis_period, cycle_dis, cycle_distri, N, Q);
	std::cout << "maximum size: " << t;
	cout << "maximum size: " << t;
	for (unsigned short int i = 0; i < N; ++i) {
		d[i] += init_alloc[i] - s[i];
		if (c_involved) {
			credit[i] += init_alloc[i] - s[i];
		}
		else {
			credit[i] = 0;
		}
		cout << "country" << to_string(i) << "init_alloc[i]: " << init_alloc[i] << '/n' << "s[i]: " << s[i] << "d[i]: " << d[i] << "credit[i]: " << credit[i] << endl;
		//actual_alloc[Q].push_back(s[i]);
	}
	return;
}

void pair_arcs(unsigned short int& Q, ListDigraph& g_original, vector<unsigned short int>& node_arrives, ListDigraph::NodeMap<bool>& active_nodes_original, vector<pair<int, int>>& arc_pair, vector<int>& nodeset) {
	FilterNodes<ListDigraph> sg(g_original, active_nodes_original);
	arc_pair.clear();
	nodeset.clear();
	for (FilterNodes<ListDigraph>::NodeIt n(sg); n != INVALID; ++n) {
		nodeset.push_back(sg.id(n));
	}
	sort(nodeset.begin(), nodeset.end());
	for (FilterNodes<ListDigraph>::ArcIt a(sg); a != INVALID; ++a) {
		arc_pair.push_back({ sg.id(sg.source(a)),sg.id(sg.target(a)) });
	}
	sort(arc_pair.begin(), arc_pair.end());
	return;

}

void arbitraryMaximum(vector<unsigned short int>& node_arrives, ListGraph& g, ListDigraph& g_original, vector<pair<int, int>>& arc_pair, vector<bool>& leaving, ListGraph::NodeMap<bool>& active_nodes, ListDigraph::NodeMap<bool>& active_nodes_original, vector<ListGraph::Node>& c, vector<ListGraph::Node>& c_b, vector<ListDigraph::Node>& c_original, bool& disp, vector<unsigned short int>& no_of_active_nodes, unsigned short int& N, unsigned short int& Vp, unsigned short int& periods, bool& dispy, vector<unsigned short int>& s, unsigned short int& Q, vector<double>& v, bool& initial_nucl, vector<double>& target, vector<double>& credit, ListGraph::EdgeMap<double>& edge_card_weight, double& t0, vector<vector<unsigned short int>>& actual_alloc, vector<int>& nodeset, vector<pair<int, int>>& cycle_distri, vector<double>& d, double& M_total, double& d_total, bool& c_involved, map<int, int>& cycle_dis, map<int, int>& numofMaxSolution, bool& arbitray_maximum, unsigned short int& initialSize, unsigned int& S, double& core_100, long & negative_core, double& prec, vector<double>& init_alloc, unsigned short int inst, double& max_d, double& game_generation, double& solution_concept_time, std::map<int, std::map<int, int>>& cycle_dis_arbitrary_period) {
	Q = 0;
	d_total = 0;
	M_total = 0;
	core_100 = 0;
	negative_core = 0;
	max_d = 0;
	if (dispy)
		cout << " --== Without lex min matching == -- " << endl;
	for (unsigned short int i = 0; i < N; i++) {
		s[i] = 0;
		credit[i] = 0;
		no_of_active_nodes[i] = initialSize;
		for (unsigned short int j = 0; j < Vp; j++) {
			if (node_arrives[i * Vp + j] == 0) {
				active_nodes[c[i * Vp + j]] = true;
				active_nodes[c_b[i * Vp + j]] = true;
				active_nodes_original[c_original[i * Vp + j]] = true;
			}
			else {
				active_nodes[c[i * Vp + j]] = false;
				active_nodes[c_b[i * Vp + j]] = false;
				active_nodes_original[c_original[i * Vp + j]] = false;
			}
		}
	}
	while (Q < periods) {
		if (dispy) {
			cout << "--== PERIOD " << Q + 1 << " ==--" << endl;
		}
		if (dispy) {
			cout << "Number of active nodes: ";
			for (unsigned short int i = 0; i < N; i++)
				cout << no_of_active_nodes[i] << " ";
			cout << endl;
		}
		// cooperative game and target
		cout << "start generating values" << endl;
		coop_game(g, v, S, s, c, c_b, edge_card_weight, dispy, Vp, N, active_nodes, leaving, numofMaxSolution, Q, arbitray_maximum, cycle_distri, cycle_dis, game_generation, cycle_dis_arbitrary_period);
		double t0 = cpuTime();
		if (initial_nucl)
			nucl(disp, N, S, init_alloc, v, prec);
		else {
			shapley(init_alloc, v, N, S);

		}
		double t1 = cpuTime();
		solution_concept_time += t1 - t0;
		M_total += v[S];

		//compute deviations


		/*if (core_dist(init_alloc, v, N, S) > 0) {
			cout << "core_dist: " << core_dist(init_alloc, v, N, S) << endl;
			core_100 += core_dist(init_alloc, v, N, S);
		}
		else {
			if (abs(core_dist(init_alloc, v, N, S) > prec)) {
				cout << "error_core_dist: " << core_dist(init_alloc, v, N, S) << endl;
				negative_core += 1;
				cout << "error_core_dist_arbitrary: " << core_dist(init_alloc, v, N, S) << endl;
				std::string fileName = "Error_not_in_the_core.txt";
				std::ofstream outputFile(fileName);

				if (outputFile.is_open()) {
					// Write data to the file
					outputFile << N << "countries" << "," << "instance: " << inst << "Period: " << Q << "error_core_dist: " << core_dist(init_alloc, v, N, S) << endl;

					// Close the file
					outputFile.close();
				}
			}

		}*/

		for (unsigned short int i = 0; i < N; ++i) {
			d[i] += init_alloc[i] - s[i];
			if (c_involved) {
				credit[i] += init_alloc[i] - s[i];
			}
			else {
				credit[i] = 0;
			}
			cout << "country" << to_string(i) << "init_alloc[i]: " << init_alloc[i] << '/n' << "s[i]: " << s[i] << "d[i]: " << d[i] << "credit[i]: " << credit[i] << endl;
			//actual_alloc[Q].push_back(s[i]);
		}


		Q++;
		changing_nodes(active_nodes, active_nodes_original, leaving, no_of_active_nodes, N, Vp, node_arrives, Q, c, c_b, c_original, s, d, target);
		if (dispy)
			cin.get();
	}
	for (unsigned short int i = 0; i < N; ++i) {
		d_total += abs(d[i]);
	}
	vector<double> max_deviation(N, 0);
	for (unsigned short int i = 0; i < N; ++i) {
		max_deviation[i] = abs(d[i]) / M_total;
	}
	std::sort(max_deviation.begin(), max_deviation.end());
	max_d = max_deviation[N - 1];
	return;
}

void cycle_distribution(std::map<int, std::map<int, int>>& cycle_dis_period, map<int, int>& cycle_dis, vector<pair<int, int>>& cycle_distri, unsigned short int& N, unsigned short int& Q) {
	unsigned short int i = 0;
	while (i < cycle_distri.size()) {
		int first, last;
		unsigned short int cycle_length = 1;
		first = cycle_distri[i].first;
		last = cycle_distri[i].second;
		cycle_distri.erase(cycle_distri.begin());
		//cout << cycle_distri.size() << '\n' << i << endl;
		for (unsigned short int j = i; j < cycle_distri.size(); ++j) {
			if (last == cycle_distri[j].first) {
				++cycle_length;
				if (first == cycle_distri[j].second) {
					//cout << "period " << j << " " << cycle_length << endl;
					++cycle_dis_period[N * (Q + 1)][cycle_length];
					cout << "cycle_dis_period[N*(Q+1)][cycle_length]: " << cycle_dis_period[N * (Q + 1)][cycle_length] << endl;
					++cycle_dis[cycle_length];
					cycle_length = 1;
					cycle_distri.erase(cycle_distri.begin() + j);
					i = 0;
					//cout << "i:" << i << '\n' << "j: " << j << endl;
					break;
				}
				else {
					last = cycle_distri[j].second;
					//cout << last << endl;
					cycle_distri.erase(cycle_distri.begin() + j);
					j = -1;
				}

			}
		}
	}
}

void sort_d_t(vector<double>& d_t, vector<GRBVar>& var_bi, long& col_num, unsigned short int& N, unsigned short int& Vp, vector<pair<int, int>>& arc_pair, vector<double>& target, unsigned short int& t, vector<double>& credit, double& epsilon, vector<GRBVar>& var_lexmin, vector<unsigned short int>& N_star) {
	unsigned short int n_star = 0;
	vector<unsigned short int> s_copy(N, 0);
	vector<double> d_copy(N, 0);
	for (unsigned short int i = 1; i < arc_pair.size() + 1; ++i) {
		var_lexmin[i - 1] = var_bi[i - 1];
		//cout << "{" << arc_pair[i].first << "," << arc_pair[i].second << "}" << endl;
		//cout << var_bi[i - 1].get(GRB_DoubleAttr_X);
		if (var_bi[i - 1].get(GRB_DoubleAttr_X) > pow(10, -4)) {
			for (unsigned short int j = 0; j < N; ++j) {
				if (j * Vp <= arc_pair[i - 1].second && arc_pair[i - 1].second < (j + 1) * Vp) {
					++s_copy[j];
				}
			}
		}
	}
	for (unsigned short int i = 0; i < N; ++i) {
		d_copy[i] = abs(target[i] + credit[i] - s_copy[i]);
	}
	//sort
	std::sort(d_copy.begin(), d_copy.end());
	for (unsigned short int i = 0; i < N; ++i) {
		std::cout << "d_copy[i]" << d_copy[i] << endl;
		std::cout << "s_copy" << s_copy[i] << endl;
	}
	//epsilon_func(target, credit, epsilon, N);
	if (epsilon > 3 * pow(10, -4)) {
		d_t[t] = d_copy[N - 1 - t] + pow(10, -4);
	}
	else {
		d_t[t] = d_copy[N - 1 - t];
	}
	if (t < N - 1) {
		d_t[t + 1] = d_copy[N - 2 - t];
	}
	std::cout << "d" << to_string(t - 1) << d_t[t - 1] << endl;
	std::cout << "epsilon: " << epsilon << endl;
}

void epsilon_func(vector<double> &target, vector<double> &credit, double& epsilon, unsigned short int N) {
	vector<double> target_credit(N, 0);
	vector<double> epsilon_sort(N * (N - 1), 0);
	for (unsigned short int i = 0; i < N; ++i) {
		cout << "target[i]: " << target[i] << endl;
		target_credit[i] = target[i] + credit[i];
	}

	unsigned short int t = -1;
	for (unsigned short int i = 0; i < N - 1; ++i) {
		for (unsigned short j = i + 1; j < N; ++j) {
			++t;
			cout << "target_credit[i]: " << target_credit[i] << "target_credit[j]: " << target_credit[j] << endl;
			epsilon_sort[t] = abs(frac(target_credit[i]) - frac(target_credit[j]));
			cout << "a-b: " << epsilon_sort[t] << endl;
			++t;
			epsilon_sort[t] = abs(frac(target_credit[i]) - (1 - frac(target_credit[j])));
			//cout << "a-(1-b): " << epsilon_sort[t] << endl;
		}
	}
	cout << "t" << t << endl;

	for (unsigned short int i = 0; i < epsilon_sort.size(); ++i) {
		cout << "epsilon_sort: " << epsilon_sort[i] << endl;
	}

	auto newEnd = std::remove_if(epsilon_sort.begin(), epsilon_sort.end(), [](double num) {
		return num < 2 * pow(10, -4);
		});
	epsilon_sort.erase(newEnd, epsilon_sort.end());

	std::sort(epsilon_sort.begin(), epsilon_sort.end());
	epsilon = epsilon_sort[0];
	cout << "epsilon_sort[0]" << epsilon_sort[0] << endl;
}

double frac(double ori) {
	double abs_frac;
	abs_frac = abs(ori) - abs(int(ori));
	return abs_frac;
}

void lex_min_n_star(vector<double>& d_t, bool& lex_min, unsigned short int& t_star, unsigned short int& N, long& col_num, double& epsilon, unsigned short int& n_star, GRBModel& model, vector<int>& ia, vector<int>& ja, vector<double>& ar, const unsigned short int& row_num, long& cnt_2, vector<double>& bound, vector<int>& nodeset, vector<unsigned short int>& N_star, vector<GRBVar>& var_lexmin, vector<GRBModel>& vector_model, unsigned short int& track) {
	++track;
	std::cout << "t_star: " << t_star << endl;
	vector<GRBVar> var_bi(col_num + N * (t_star + 3));
	std::cout << "var_bi.size(): " << var_bi.size() << endl;
	//arc variables
	for (unsigned short int i = 0; i < col_num - 1; ++i) {
		var_bi[i] = vector_model[track].addVar(0.0, 1.0, 0.0, GRB_BINARY, "x" + to_string(i));
	}
	// N difference variables
	for (unsigned short int i = col_num; i < col_num + N; ++i) {
		var_bi[i] = vector_model[track].addVar(-GRB_INFINITY, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "x" + to_string(i));
	}
	// N abs variables
	for (unsigned short int i = col_num + N; i < col_num + N * 2; ++i) {
		var_bi[i] = vector_model[track].addVar(0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "x" + to_string(i));
	}
	// z^t_p variables
	for (unsigned short i = col_num + N * 2; i < col_num + N * (t_star + 3); ++i) {
		var_bi[i] = vector_model[track].addVar(0.0, 1.0, 0.0, GRB_BINARY, "x" + to_string(i));
	}

	//sum zp
	GRBLinExpr sum_zp = 0;
	for (unsigned short int i = col_num + N * (t_star + 2); i < col_num + N * (t_star + 3); ++i) {
		sum_zp += var_bi[i];
	}
	vector_model[track].setObjective(sum_zp, GRB_MINIMIZE);
	vector<GRBLinExpr> sum_row(row_num, 0);
	vector<GRBLinExpr> sum_zp_dp(N, 0);
	vector<GRBLinExpr> sum_zp_0(N, 0);
	vector<GRBLinExpr> sum_t_star(t_star + 1, 0);
	vector<GRBLinExpr> sum_p(N, 0);
	for (unsigned short int i = 0; i < t_star; ++i) {
		for (unsigned short int j = 0; j < N; ++j) {
			sum_zp_dp[j] += var_bi[i * N + col_num + N * 2 + j] * d_t[i];
			sum_zp_0[j] += var_bi[i * N + col_num + N * 2 + j];
			sum_t_star[i] += var_bi[i * N + col_num + N * 2 + j];
			sum_p[j] += var_bi[i * N + col_num + N * 2 + j];
		}
	}
	for (unsigned short int j = 0; j < N; ++j) {
		sum_p[j] += var_bi[t_star * N + col_num + N * 2 + j];
	}
	std::cout << "finish loading z^t_p" << endl;
	for (long j = 1; j < cnt_2 + 1; ++j) {
		sum_row[ia[j] - 1] += ar[j] * var_bi[ja[j] - 1];
	}
	std::cout << "finish loading coefficiencies" << endl;
	for (unsigned short int i = 1; i < N + 1; ++i) {
		vector_model[track].addConstr(var_bi[col_num + i - 1] == sum_row[i - 1] - bound[i - 1]);
		vector_model[track].addGenConstrAbs(var_bi[col_num + N + i - 1], var_bi[col_num + i - 1]);
		if (t_star == 0) {
			vector_model[track].addConstr(var_bi[col_num + N + i - 1] <= d_t[t_star] - epsilon * (1 - var_bi[col_num + 2 * N + t_star * N + i - 1]));
		}
		if (t_star > 0) {
			vector_model[track].addQConstr(var_bi[col_num + N + i - 1] <= (1 - sum_zp_0[i - 1]) * (d_t[t_star] - epsilon * (1 - var_bi[col_num + 2 * N + t_star * N + i - 1])) + sum_zp_dp[i - 1]);
		}
	}
	std::cout << "finish loading constraint 1" << endl;
	for (unsigned short int i = N + 1; i < N + 1 + nodeset.size(); ++i) {
		vector_model[track].addConstr(sum_row[i - 1] == bound[i - 1]);
	}
	std::cout << "finish loading constraint 2" << endl;
	for (unsigned short int i = N + nodeset.size() + 1; i < N + 2 * (nodeset.size()) + 1; ++i) {
		vector_model[track].addConstr(sum_row[i - 1] <= bound[i - 1]);
	}
	std::cout << "finish loading constraint 3" << endl;
	vector_model[track].addConstr(sum_row[row_num - 1] == bound[row_num - 1]);
	std::cout << "finish loading constraint 4" << endl;
	for (unsigned short int i = 0; i < N; ++i) {
		vector_model[track].addConstr(sum_p[i] <= 1);
	}
	std::cout << "finish loading constraint 5" << endl;
	for (unsigned short int i = 0; i < t_star; ++i) {
		vector_model[track].addConstr(sum_t_star[i] == N_star[i]);
	}
	std::cout << "finish loading constraint 6" << endl;
	std::cout << "finish loading constraints" << endl;
	vector_model[track].optimize();
	for (unsigned short int i = 0; i < t_star + 1; ++i) {
		for (unsigned short int j = 0; j < N; ++j) {
			std::cout << "t: " << i << " " << "country:" << j << ": " << var_bi[col_num + N * (2 + i) + j].get(GRB_DoubleAttr_X) << endl;
		}
	}
	for (unsigned short int i = 0; i < N; ++i) {
		if (var_bi[col_num + N * (2 + t_star) + i].get(GRB_DoubleAttr_X) > pow(10, -4)) {
			N_star[t_star] += 1;
		}
		std::cout << "var_bi[col_num + N * (2 + t_star) + i].get(GRB_DoubleAttr_X): " << var_bi[col_num + N * (2 + t_star) + i].get(GRB_DoubleAttr_X) << endl;

	}
	std::cout << "epsilon: " << epsilon << "d_t[t_star: " << d_t[t_star] << endl;
	std::cout << "N_star[t_star]: " << N_star[t_star] << endl;
	std::cout << "finish n_" << to_string(t_star) << endl;
	n_star = 0;
	for (unsigned short int i = 0; i < N; ++i) {
		n_star += N_star[i];
	}
	t_star += N_star[t_star];
	for (unsigned short int i = 0; i < col_num - 1; ++i) {
		var_lexmin[i] = var_bi[i];
	}

}

void lex_min_d_star(vector<double>& d_t, bool& lex_min, unsigned short int& t_star, unsigned short int& N, long& col_num, double& epsilon, unsigned short int& n_star, GRBModel& model, vector<int>& ia, vector<int>& ja, vector<double>& ar, const unsigned short int& row_num, long& cnt_2, vector<double>& bound, vector<int>& nodeset, vector<unsigned short int>& N_star, unsigned short int& Vp, vector<pair<int, int>>& arc_pair, vector<double>& target, vector<double>& credit, vector<GRBVar>& var_lexmin, vector<GRBModel>& vector_model, unsigned short int& track) {
	++track;
	vector<GRBVar> var_bi(col_num + N * (t_star + 3));
	//arc variables
	for (unsigned short int i = 0; i < col_num - 1; ++i) {
		var_bi[i] = vector_model[track].addVar(0.0, 1.0, 0.0, GRB_BINARY, "x" + to_string(i));
	}
	// d_t variable
	var_bi[col_num - 1] = vector_model[track].addVar(0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "d" + to_string(t_star));
	// N difference variables
	for (unsigned short int i = col_num; i < col_num + N; ++i) {
		var_bi[i] = vector_model[track].addVar(-GRB_INFINITY, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "x" + to_string(i));
	}
	// N abs variables
	for (unsigned short int i = col_num + N; i < col_num + N * 2; ++i) {
		var_bi[i] = vector_model[track].addVar(0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "x" + to_string(i));
	}
	//zp variables
	for (unsigned short i = col_num + N * 2; i < col_num + N * (t_star + 2); ++i) {
		var_bi[i] = vector_model[track].addVar(0.0, 1.0, 0.0, GRB_BINARY, "x" + to_string(i));
	}
	vector_model[track].setObjective(1 * var_bi[col_num - 1], GRB_MINIMIZE);
	vector<GRBLinExpr> sum_row(row_num, 0);
	vector<GRBLinExpr> sum_zp_dp(N, 0);
	vector<GRBLinExpr> sum_zp_0(N, 0);
	vector<GRBLinExpr> sum_t_star(t_star + 1, 0);
	vector<GRBLinExpr> sum_p(N, 0);
	for (unsigned short int i = 0; i < t_star; ++i) {
		for (unsigned short int j = 0; j < N; ++j) {
			sum_zp_dp[j] += var_bi[i * N + col_num + N * 2 + j] * d_t[i];
			sum_zp_0[j] += var_bi[i * N + col_num + N * 2 + j];
			sum_t_star[i] += var_bi[i * N + col_num + N * 2 + j];
			sum_p[j] += var_bi[i * N + col_num + N * 2 + j];
		}
	}


	for (long j = 1; j < cnt_2 + 1; ++j) {
		sum_row[ia[j] - 1] += ar[j] * var_bi[ja[j] - 1];
	}

	for (unsigned short int i = 1; i < N + 1; ++i) {
		vector_model[track].addConstr(var_bi[col_num + i - 1] == sum_row[i - 1] - bound[i - 1]);
		vector_model[track].addGenConstrAbs(var_bi[col_num + N + i - 1], var_bi[col_num + i - 1]);
		if (t_star == 0) {
			vector_model[track].addConstr(var_bi[col_num + N + i - 1] <= var_bi[col_num - 1] + sum_zp_dp[i - 1]);
		}
		if (t_star > 0) {
			vector_model[track].addQConstr(var_bi[col_num + N + i - 1] <= (1 - sum_zp_0[i - 1]) * var_bi[col_num - 1] + sum_zp_dp[i - 1]);
		}
	}

	for (unsigned short int i = N + 1; i < N + 1 + nodeset.size(); ++i) {
		vector_model[track].addConstr(sum_row[i - 1] == bound[i - 1]);
	}
	for (unsigned short int i = N + nodeset.size() + 1; i < N + 2 * (nodeset.size()) + 1; ++i) {
		vector_model[track].addConstr(sum_row[i - 1] <= bound[i - 1]);
	}
	vector_model[track].addConstr(sum_row[row_num - 1] == bound[row_num - 1]);

	for (unsigned short int i = 0; i < N; ++i) {
		vector_model[track].addConstr(sum_p[i] <= 1);
	}
	for (unsigned short int i = 0; i < t_star; ++i) {
		vector_model[track].addConstr(sum_t_star[i] == N_star[i]);
	}

	vector_model[track].optimize();
	sort_d_t(d_t, var_bi, col_num, N, Vp, arc_pair, target, t_star, credit, epsilon, var_lexmin, N_star);
}

void lexmin_searching(vector<double>& d_t, bool& lex_min, unsigned short int& t_star, unsigned short int& N, long& col_num, double& epsilon, unsigned short int& n_star, GRBModel& model, vector<int>& ia, vector<int>& ja, vector<double>& ar, const unsigned short int& row_num, long& cnt_2, vector<double>& bound, vector<int>& nodeset, vector<unsigned short int>& N_star, unsigned short int& Vp, vector<pair<int, int>>& arc_pair, vector<double>& target, vector<double>& credit, vector<GRBVar>& var_lexmin, unsigned short int inst, vector<GRBModel>& vector_model, unsigned short int& track) {
	while (lex_min && abs(d_t[t_star - 1]) > 0.5 && n_star < N) {
		std::cout << "begin search d_t" << endl;
		lex_min_d_star(d_t, lex_min, t_star, N, col_num, epsilon, n_star, model, ia, ja, ar, row_num, cnt_2, bound, nodeset, N_star, Vp, arc_pair, target, credit, var_lexmin, vector_model, track);
		std::cout << "inst: " << inst << endl;
		std::cout << "abs(d_t[t_star])" << abs(d_t[t_star]) << endl;
		std::cout << "t_star: " << t_star << endl;
		std::cout << "N-1: " << N - 1 << endl;
		if (t_star == N - 1) {
			std::cout << "N-1==t_star" << endl;
		}
		else {
			std::cout << "N-1!=t_star" << endl;
		}
		std::cout << "epsilon in the loop: " << epsilon << endl;
		std::cout << "absolute epsilon in the loop: " << abs(epsilon) << endl;
		if (abs(d_t[t_star]) > 0.5) {
			if (abs(epsilon) > pow(10, -4)) {
				if (t_star == N - 1) {
					std::cout << "congratulations t_star == N - 1" << endl;
					//break
					n_star = N;
					std::cout << "congratulations after break" << endl;
				}
				else
				{
					if (d_t[t_star + 1] < 0.5) {
						std::cout << "congratulations d_t[t_star + 1] < 0.5" << endl;
						//break;
						n_star = N;
					}
					else {
						std::cout << "congratulations d_t[t_star + 1] > 0.5" << endl;
						lex_min_n_star(d_t, lex_min, t_star, N, col_num, epsilon, n_star, model, ia, ja, ar, row_num, cnt_2, bound, nodeset, N_star, var_lexmin, vector_model, track);
					}
				}
			}
			if (abs(epsilon) < pow(10, -4)) {
				std::cout << "congratulations abs(epsilon) < pow(10, -7))" << endl;
				lexmin_searching(d_t, lex_min, t_star, N, col_num, epsilon, n_star, model, ia, ja, ar, row_num, cnt_2, bound, nodeset, N_star, Vp, arc_pair, target, credit, var_lexmin, inst, vector_model, track);
			}
		}
		else {
			std::cout << "congratulations break" << endl;
			//break
			n_star = N;
		}
	}
}
