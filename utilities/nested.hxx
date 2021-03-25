#ifndef UTILITIES_NESTED_HXX_
#define UTILITIES_NESTED_HXX_

template <class Outer>
struct Nested {
    using nested_t = Nested<Outer>;
    using outer_t = Outer;
    Nested(outer_t* outer): outer(outer) { }
protected:
    outer_t* outer;
};


#endif /* UTILITIES_NESTED_HXX_ */
