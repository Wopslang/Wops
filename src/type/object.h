/*
 * src/type/object.h
 * Wopslang Object Template Headerfile
 *
 * 2021, Wops Team
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
*/
class Object {
    private:
    std::vector<Variable> data;

    public:
    // a[100][200][300] -> size = {100, 200, 300}, dim = 3
    // single variable -> dim = 0
    std::vector<Int> size; // multiplication of all size <= 10^9
    std::vector<Int> precalc_size;
    Int dim;
    Int runtime_codeline; // for error analyzing
    String token;

    // constructor
    Object(String objname = "_", std::vector<Variable> objdata = {}, std::vector<Int> objsize = {}, Int objdim = 0, Int codeline = -1) {
        token = objname;
        data = objdata;
        size = objsize;
        dim = objdim;
        runtime_codeline = codeline;
        Prepare();
    }

    // void Prepare()
    // Prerequisite Job
    void Prepare() {
        if (!dim) return;

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

    // Variable At(std::vector<Int> dimidx)
    // Get the data addressed by dimidx
    Variable At(std::vector<Int> dimidx) {
        return data[IdxFinder(dimidx)];
    }

    // void Replace(std::vector<Int> dimidx, Object newdata)
    // Replace the data addressed by dimidx with newdata
    void Replace(std::vector<Int> dimidx, Variable newdata) {
        data[IdxFinder(dimidx)] = newdata;
    }

    // Int IdxFinder(std::vector<Int> dimidx)
    Int IdxFinder(std::vector<Int> dimidx) {
        Int cur = 0;
        for (int idx = 0; idx < dim; idx++) cur += dimidx[idx]*precalc_size[idx];
        return cur;
    }
};

#endif
