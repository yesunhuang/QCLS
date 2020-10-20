#include <memory>
#include <iostream>
#include "operator_tree.h"
#include "deb.h"
#include "common.h"
#include "cluster_expansion.h"
#include "static_dtree.h"

//#include <crtdbg.h>

using namespace ayaji;

int main() {
	{
		double E = 1;
		double g = 0.12;
		double kappa_a = 2;
		double kappa_b = 2;
		//OPTree tree1;
		//tree1.insert(deb_generateLabelSeq(2, 1, 2));
		//tree1.insert(deb_generateLabelSeq(2, 1, 2), Complex(-1, 0));
		//tree1.search(deb_generateLabelSeq(2, 1, 2))->value = Complex();
		//tree1.normalize();
		//tree1 = ClusterExp::ClusterExpansion(deb_generateLabelSeq(4, 1, 2, 3, 4));
		//tree1 = ClusterExp::DeltaTree(1);
		//tree1 = OPTree::BuildFromPTree(t, deb_generateLabelSeq(2, 1, 2));

		//OPTree tree2;
		//tree2.insert(deb_generateLabelSeq(5, 1, 2, 3, 4, 5));
		//tree2.insert(deb_generateLabelSeq(5, 0, 2, 3, 4, 5));

		//deb_printTree(tree1);
		//deb_printTree(tree1);
		//_CrtDumpMemoryLeaks();


		/* DeriveData Debug Area 1 */

		//std::vector<LabelSeq> H;
		//H.push_back(deb_generateLabelSeq(1, 2));
		//H.push_back(deb_generateLabelSeq(1, 1));
		//H.push_back(deb_generateLabelSeq(3, 1, 1, 4));
		//H.push_back(deb_generateLabelSeq(3, 2, 2, 3));
		//std::vector<LabelSeq> Co_ps;
		//Co_ps.push_back(deb_generateLabelSeq(1, 1));
		//Co_ps.push_back(deb_generateLabelSeq(1, 3));
		//std::vector<Complex> h_coef;
		//h_coef.push_back(Complex(E, 0));
		//h_coef.push_back(Complex(E, 0));
		//h_coef.push_back(Complex(0, 0));
		//h_coef.push_back(Complex(0, 0));
		//std::vector<Complex> c_coef;
		//c_coef.push_back(Complex(kappa_a, 0));
		//c_coef.push_back(Complex(kappa_b, 0));
		//std::vector<LabelSeq> track;
		//track.push_back(deb_generateLabelSeq(2, 1, 2));
		//track.push_back(deb_generateLabelSeq(2, 3, 4));
		//track.push_back(deb_generateLabelSeq(4, 1, 1, 2, 2));
		//track.push_back(deb_generateLabelSeq(4, 3, 3, 4, 4));
		//int maxoplen = 2;
		//LabelSeq init;
		//init.push_back(1);
		//init.push_back(3);
		//auto outputdata = DeriveData::deriveAssign(H, h_coef, Co_ps, c_coef, init, track, maxoplen);
		//deb_printData(outputdata);
		//auto ans = outputdata.calEvolution();
		//while(true){}

		//auto a = OPTree::MONormalize(deb_generateLabelSeq(5, 1, 1, 4, 1, 2));
		//deb_printTree(a);
		//auto b = Common::barSeq(deb_generateLabelSeq(4, 1, 1, 3, 4));

		/* debug de_ce */
		//OPTree t;
		//t.insert(deb_generateLabelSeq(3, 1, 1, 4));
		//DeriveData::_DeleteAndCE(t, 2);
		//deb_printTree(t);

		/* debug tree add */
		//OPTree t1;
		//t1.insert(deb_generateLabelSeq(4, 1, 1, 1, 2));
		//t1.insert(deb_generateLabelSeq(2, 1, 1), Complex(2,0));
		//OPTree t2;
		//t2.insert(deb_generateLabelSeq(4, 1, 1, 1, 2), Complex(-1, 0));
		//t1.add(t2);
		//deb_printTree(t1);

		/* DeriveData Debug Area 2 */
		//std::vector<LabelSeq> H;
		//H.push_back(deb_generateLabelSeq(3, 1, 1, 4));
		//H.push_back(deb_generateLabelSeq(3, 2, 2, 3));
		//H.push_back(deb_generateLabelSeq(1, 3));
		//H.push_back(deb_generateLabelSeq(1, 4));
		//std::vector<LabelSeq> Co_ps;
		//Co_ps.push_back(deb_generateLabelSeq(1, 2));
		//Co_ps.push_back(deb_generateLabelSeq(1, 4));
		//std::vector<Complex> h_coef;
		//h_coef.push_back(Complex(0, 0));
		//h_coef.push_back(Complex(0, 0));
		//h_coef.push_back(Complex(E, 0));
		//h_coef.push_back(Complex(E, 0));
		//std::vector<Complex> c_coef;
		//c_coef.push_back(Complex(kappa_a, 0));
		//c_coef.push_back(Complex(kappa_b, 0));
		//std::vector<LabelSeq> track;
		//track.push_back(deb_generateLabelSeq(2, 1, 2));
		//track.push_back(deb_generateLabelSeq(2, 3, 4));
		//track.push_back(deb_generateLabelSeq(4, 1, 1, 2, 2));
		//track.push_back(deb_generateLabelSeq(4, 3, 3, 4, 4));
		//int maxoplen = 2;
		//LabelSeq init;
		//init.push_back(0);
		//init.push_back(0);
		//init.push_back(0);
		//init.push_back(0);
		//auto outputdata = DeriveData::deriveAssign(H, h_coef, Co_ps, c_coef, init, track, maxoplen);
		//deb_printData(outputdata);
		//auto ans = outputdata.calEvolution();

		/* DeriveData Debug Area 3 */
		std::vector<LabelSeq> H;
		H.push_back(deb_generateLabelSeq(2, 1, 2));
		H.push_back(deb_generateLabelSeq(2, 3, 4));
		H.push_back(deb_generateLabelSeq(3, 1, 1, 4));
		H.push_back(deb_generateLabelSeq(3, 2, 2, 3));
		H.push_back(deb_generateLabelSeq(1, 1));
		H.push_back(deb_generateLabelSeq(1, 2));
		std::vector<LabelSeq> Co_ps;
		Co_ps.push_back(deb_generateLabelSeq(1, 2));
		Co_ps.push_back(deb_generateLabelSeq(1, 4));
		std::vector<Complex> h_coef;
		h_coef.push_back(Complex(1, 0));
		h_coef.push_back(Complex(1, 0));
		h_coef.push_back(Complex(1, 0));
		h_coef.push_back(Complex(1, 0));
		h_coef.push_back(Complex(1, 0));
		h_coef.push_back(Complex(1, 0));
		std::vector<Complex> c_coef;
		c_coef.push_back(Complex(5, 0));
		c_coef.push_back(Complex(8, 0));
		std::vector<LabelSeq> track;
		track.push_back(deb_generateLabelSeq(2, 1, 2));
		//track.push_back(deb_generateLabelSeq(2, 3, 4));
		int maxoplen = 2;
		LabelSeq init;
		init.push_back(0);
		init.push_back(1);
		auto outputdata = DeriveData::deriveAssign(H, h_coef, Co_ps, c_coef, init, track, maxoplen);
		//deb_printTree(outputdata.trackValueTree);
		deb_printData(outputdata);
		//for (int i = 0; i < 100000; ++i) {
		//	auto ans = outputdata.calEvolution();
		//}
		
		while(true){}
	}
	_CrtDumpMemoryLeaks();
	return 0;
}