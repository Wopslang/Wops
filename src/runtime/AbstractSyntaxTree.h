#ifndef WOPS_AST_IMPL_H
#define WOPS_AST_IMPL_H

#include <vector>

template <typename T>
class WopsASTNode {
private:
    T data_;
public:
    WopsAST(T data) {
        data_ = data;
    }
    WopsAST<T> *parent;
    std::vector<WopsAST<T> *> children;
};

#endif //WOPS_AST_IMPL_H

