#include "LockedCandidateIndex.h"

LockedCandidateIndex::LockedCandidateIndex(unsigned block_idx, unsigned idx) : block_idx(block_idx), idx(idx) {}

unsigned LockedCandidateIndex::get_block_idx() const{
    return block_idx;
}

unsigned LockedCandidateIndex::get_idx() const{
    return idx;
}

bool operator<(const LockedCandidateIndex& x, const LockedCandidateIndex& y){
    if (x.block_idx != y.block_idx)
        return x.block_idx < y.block_idx;

    return x.idx < y.idx;
}