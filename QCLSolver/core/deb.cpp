#include "deb.h"

#include "derive_data.h"

namespace ayaji {
	LabelSeq deb_generateLabelSeq(unsigned size, ...) {
		va_list valist;
		va_start(valist, size);
		LabelSeq seq;
		for (unsigned i = 0; i < size; ++i) {
			int value = va_arg(valist, int);
			seq.push_back((Label)value);
		}
		va_end(valist);
		return seq;
	}

	void deb_PrintTree(const OPTree& tree) {
		if (tree.root == nullptr) return;
		LabelSeq seqBuf;
		for (int i = 0; i <= tree.childSize; ++i) {
			pNode node;
			if ((node = tree.root->children[i]) != nullptr) {
				deb_PrintTree_(node, seqBuf, tree.childSize);
			}
		}
	}

	void deb_PrintTree_(pNode& node, LabelSeq& seqBuf, int cSize) {
		if (node == nullptr) return;
		seqBuf.push_back(node->label);
		if (!node->value.isZero()) {
			std::string coef = node->value.getString();
			std::cout << "Coef: " << coef << "  Struct: [";
			for (auto it = seqBuf.cbegin(); it != seqBuf.cend(); ++it) {
				std::cout << *it << ", ";
			}
			std::cout << "]" << std::endl;
		}
		for (int i = 0; i <= cSize; ++i) {
			pNode tnode;
			if ((tnode = node->children[i]) != nullptr) {
				deb_PrintTree_(tnode, seqBuf, cSize);
			}
		}
		seqBuf.pop_back();
	}

	std::vector<std::pair<Complex, LabelSeq>> deb_Tree2Pair(const OPTree& tree) {
		if (tree.root == nullptr) return std::vector<std::pair<Complex, LabelSeq>>();
		std::vector<std::pair<Complex, LabelSeq>> ans;
		LabelSeq seqBuf;
		for (int i = 0; i <= tree.childSize; ++i) {
			pNode node;
			if ((node = tree.root->children[i]) != nullptr) {
				deb_Tree2Pair_(node, seqBuf, tree.childSize, ans);
			}
		}
		return std::move(ans);
	}

	void deb_Tree2Pair_(pNode& node, LabelSeq& seqBuf, int cSize, std::vector<std::pair<Complex, LabelSeq>>& pairs) {
		if (node == nullptr) return;
		seqBuf.push_back(node->label);
		if (!node->value.isZero()) {
			pairs.push_back(
				std::pair<Complex, LabelSeq>(node->value, seqBuf)
			);
		}
		for (int i = 0; i <= cSize; ++i) {
			pNode tnode;
			if ((tnode = node->children[i]) != nullptr) {
				deb_Tree2Pair_(tnode, seqBuf, cSize, pairs);
			}
		}
		seqBuf.pop_back();
	}

	void deb_PrintData(DeriveData& data) {
		printf("---------------------------\n");
		printf("Tracking Nodes:\n");
		for (int i = 0; i < data.size; ++i) {
			LabelSeq seq = data.trackNodes[i]->seqFromRoot();
			printf("Nodes[%d]:  {%d", i, (int)seq[0]);
			for (int j = 1; j < seq.size(); ++j)
				printf(", %d", (int)seq[j]);
			printf("}  curValue: %.3lf+%.3lfi\n", data.curValues[i].getReal(), data.curValues[i].getImage());
		}
		printf("---------------------------");
		printf("\n\n\n");

		for (int i = 0; i < data.size; ++i) {
			printf("Tracking Operator %d :\n", i);
			for (int j = 0; j < data.hoSize; j++) {
				printf("HO_Tree %d, coef is %.3lf+(%.3lf)j:\n", j,
					data.hoCoefs[j].getReal(), data.hoCoefs[j].getImage());
				deb_PrintTree(data.evoTrees_HO[i][j]);
				putchar('\n');
			}
			putchar('\n');
			for (int j = 0; j < data.coSize; j++) {
				printf("CO_Tree %d:, coef is %.3lf+(%.3lf)j:\n", j,
					data.coCoefs[j].getReal(), data.coCoefs[j].getImage());
				deb_PrintTree(data.evoTrees_CO[i][j]);
				putchar('\n');
			}
			printf("---------------------------");
			printf("\n\n\n");
		}
	}

}