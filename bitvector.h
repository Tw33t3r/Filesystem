enum {OFF,ON};

typedef unsigned int BitVecType;

typedef BitVecType *BitVec;

class BitVector {
  public:
    int *bv;
    unsigned short size;
    int wordWidth; //will be the size of the int array, using for bitvector
    int ui_size;  //size of unsigned integer * 8;

    BitVector(int sz); /* Creates a bit vector of size sz */
    ~BitVector();
    void setBit(int pos); /* sets the bit at position pos to 1 */
    void resetBit(int pos); /* sets the bit at position pos to 0 */
    int testBit(int pos); /* returns 0 if bit value at position pos is 0,
                             otherwise returns a number greater than 0 */
    void setBitVector(unsigned int *nbv); /* initializes a bit vector */
    void getBitVector(unsigned int *nbv); /* retrieves a bit vector */
};
