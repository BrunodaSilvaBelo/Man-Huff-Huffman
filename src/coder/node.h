#ifndef NODE_H
#define NODE_H

#include <memory>
#include <cstdint>

struct node {
    std::shared_ptr<node> rightnode;
    std::shared_ptr<node> leftnode;
    std::weak_ptr<node> father;

    unsigned int frequency;
    std::vector<std::uint8_t> children;
};

#endif /* NODE_H */
