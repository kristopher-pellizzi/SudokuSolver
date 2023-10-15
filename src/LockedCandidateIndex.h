#ifndef LOCKEDCANDIDATEIDX
#define LOCKEDCANDIDATEIDX

class LockedCandidateIndex{
    private:
        unsigned block_idx;
        unsigned idx; // Either row or col idx inside the block

    public:
        LockedCandidateIndex(unsigned block_idx, unsigned idx);

        unsigned get_block_idx() const;
        unsigned get_idx() const;        

        friend bool operator<(const LockedCandidateIndex& x, const LockedCandidateIndex& y);
};

#endif // LOCKEDCANDIDATEIDX