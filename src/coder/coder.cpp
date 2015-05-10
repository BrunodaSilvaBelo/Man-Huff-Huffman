#include "coder.h"
#include "node.h"
#include <algorithm>

using namespace std;
using boost::dynamic_bitset;

void treewalker(map<uint8_t, dynamic_bitset<>> &out, shared_ptr<node> node,
                dynamic_bitset<> bits = dynamic_bitset<>{}) {
    if (!node->rightnode) {
        out[node->children.front()] = bits;
        return;
    }

    bits.push_back(false);
    treewalker(out, node->leftnode, bits);
    bits[bits.size() - 1] = true;
    treewalker(out, node->rightnode, bits);
}

map<uint8_t, dynamic_bitset<>> codetable(map<uint8_t, unsigned int> frequency) {
    vector<shared_ptr<node>> nodes;

    for (auto f : frequency) {
        auto n = make_shared<node>();
        n->frequency = f.second;
        n->children.push_back(f.first);

        nodes.push_back(n);
    }

    while (nodes.size() != 1) {
        auto i = nodes.front();
        nodes.erase(nodes.begin());
        auto j = nodes.front();
        nodes.erase(nodes.begin());

        auto n = make_shared<node>();
        n->leftnode = i;
        n->rightnode = j;
        i->father = n;
        j->father = n;

        n->frequency = i->frequency + j->frequency;
        n->children = i->children;
        n->children.insert(n->children.begin(),
                           j->children.begin(), j->children.end());

        nodes.insert(upper_bound(nodes.begin(), nodes.end(), n,
                                 [](decltype(n) lhs, decltype(n) rhs)
                                 {
                                     return lhs->frequency < rhs->frequency;
                                 }), n);
    }

    map<uint8_t, dynamic_bitset<>> map;
    treewalker(map, nodes.front());

    return map;
}
