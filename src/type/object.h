/*
 * src/type/object.h
 * Wopslang Object Template Headerfile
 *
 * 2023, Wops Team
 * */

#ifndef WOPS_OBJECT_H
#define WOPS_OBJECT_H
#include "variable.h"
#include <vector>

/**
 * class Object  
 * Wopslang Object Class
 *
 * Structure
 *
 * Private
 *   - data
 * Public 
 *   - size 
 *   - precalc_size
 *   - dim
 *   - runtime_codeline
 *   - token 
 *   - errvalue
*/
class Object {
    private:
    std::vector<Object> data; // for base, data = {}
    Variable base; // base (0-dim)

    public:
    // a[100][200][300] -> size = {100, 200, 300}, dim = 3
    // single variable -> dim = 0
    std::vector<Int> size; // multiplication of all size <= 10^9
    std::vector<Int> precalc_size;
    Int dim;
    Int runtime_codeline; // for error analyzing
    String token;
    Err errvalue; // special variable: only used to deliver error code

    // constructor
    Object(String objname = "_", std::vector<Object> objdata = {}, std::vector<Int> objsize = {}, Variable objbase = {}, Int objdim = 0, Int codeline = -1, Err err = OK) {
        token = objname;
        data = objdata;
        base = objbase;
        size = objsize;
        dim = objdim;
        runtime_codeline = codeline;
        errvalue = err;
        Prepare();
    }

    // void Prepare()
    // Prerequisite Job
    void Prepare() {
        if (!dim) return;
        if (dim != size.size()) ErrHandler().CallErr(runtime_codeline, OBJECT_WRONG_DIMENSION, {token, std::to_string(dim), std::to_string(size.size())}); 

        precalc_size.resize(dim);
        precalc_size[dim-1] = 1;
        for (int idx = dim-2; idx >= 0; idx--) {
            Int *checker; *checker = precalc_size[idx+1]*size[idx+1];
            if (precalc_size[idx+1] > 0 && size[idx+1] > 0 && *checker < 0) ErrHandler().CallErr(runtime_codeline, OBJECT_OVERFLOW, {token});
            if (precalc_size[idx+1] < 0 && size[idx+1] < 0 && *checker > 0) ErrHandler().CallErr(runtime_codeline, OBJECT_OVERFLOW, {token});
            if (precalc_size[idx+1]*size[idx+1] > 1000000000) ErrHandler().CallErr(runtime_codeline, OBJECT_OVERFLOW, {token});
            precalc_size[idx] = precalc_size[idx+1]*size[idx+1];
        }
        if (precalc_size[0]*size[0] != data.size()) ErrHandler().CallErr(runtime_codeline, OBJECT_NOT_MATCHING_DATA, {token});
    }

    // Object At(std::vector<Int> dimidx)
    // Get the data addressed by dimidx
    Object At(std::vector<Int> dimidx) {
        if (!dim) return Object("_", {}, {}, base);
        if (dim != dimidx.size()) ErrHandler().CallErr(runtime_codeline, OBJECT_WRONG_DIMENSION, {token, std::to_string(dim), std::to_string(dimidx.size())});

        Int idx = IdxFinder(dimidx);
        if (idx >= data.size()) ErrHandler().CallErr(runtime_codeline, OBJECT_OVERFLOW, {token});
        return data[IdxFinder(dimidx)];
    }

    // void Replace(std::vector<Int> dimidx, Object newdata)
    // Replace the data addressed by dimidx with newdata
    void Replace(std::vector<Int> dimidx, Object newdata) {
        if (dim != newdata.dim) ErrHandler().CallErr(runtime_codeline, OBJECT_WRONG_DIMENSION, {token, std::to_string(dim), std::to_string(newdata.dim)});
        if (dimidx.size() != dim) ErrHandler().CallErr(runtime_codeline, OBJECT_WRONG_DIMENSION, {token, std::to_string(dim), std::to_string(dimidx.size())}); 
        if (!dim) { base = newdata.base; return; }
        data[IdxFinder(dimidx)] = newdata;
    }

    // void ReplaceContainer(std::vector<Object> container)
    // Replace whole data with container 
    void ReplaceContainer(std::vector<Object> container) {
        data = container;
    }

    // std::vector<Object> GetContainer()
    // Getter function for data
    std::vector<Object> GetContainer() { return data; }

    // Variable GetBase()
    // Getter function for base
    Variable& GetBase() { 
        if (dim) ErrHandler().CallErr(runtime_codeline, TOO_HIGH_DIMENSION, {token, "0", std::to_string(dim)});
        return base; 
    }

    // Int IdxFinder(std::vector<Int> dimidx)
    Int IdxFinder(std::vector<Int> dimidx) {
        Int cur = 0;
        for (int idx = 0; idx < dim; idx++) cur += dimidx[idx]*precalc_size[idx];
        return cur;
    }
};

#endif
