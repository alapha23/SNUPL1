module integer_const;

// min/max int test
// min: -2147483648
// max:  2147483647

var i: integer;

//
// strict(): 1:1 implementation of SnuPL/1 EBNF
//
// strict() does not perform folding of negation and constants.
// Since 'number' accepts only positive numbers, there is no way
// to directly represent MIN_INT.
//
procedure strict();
var i: integer;
begin
  i := -2147483648;                 // fail
  i := -2147483647;                 // pass
  i :=  2147483647;                 // pass
  i :=  2147483648;                 // fail

  i := 0
end strict;

//
// simple(): fold negation with constant 'term'
//
// simple() interpretes the grammar is a slightly different way if a constant
// follows the initial sign of a simplexpr. simple() performs folding of nega-
// tion and constants iff the term follwing the +/- sign is a constant. This
// leads to unintuitive behavior since -MIN_INT + ... is accepted, but
// -MIN_INT * ... is not.
//
// To understand why consider the parse trees
//
//     -2147483648 + 5                  -2147483648 * 5
//
//       simpleexpr                       simpleexpr
//      /  |    |  \                        /   \
//    "-" term "+" term                   "-"   term
//         |        |                          /  |  \
//       factor   factor                 factor  "*"  factor
//         |        |                      |             |
//       number   number                 number       number
//         |        |                      |             |
//    2147483648    5                 2147483648         5
//
// in the left case, the first term is represented by an CAstConstant node
// which can be easily folded with the "-" sign on its left. For the multi-
// plication, the (first) term is of type CAstBinaryOp and can thus not be
// folded anymore.
//
//       simpleexpr                       simpleexpr
//      /  |    |  \                        /   \
//    "-" term "+" term                   "-"  term
//     (of type)  (of type)                  (of type)
//  CAstConstant CAstConstant               CAstBinaryOp
//                                         /     |     \
//      ^                               factor  "*"  factor
//      |                              (of type)    (of type)
//  fold "-" w/ CAstConstant         CAstConstant  CAstConstant
//
procedure simple();
var i: integer;
begin
  i := -2147483648;                 // pass
  i := -2147483647;                 // pass
  i :=  2147483647;                 // pass
  i :=  2147483648;                 // fail

  i := -2147483648 + 2147483648;    // fail
  i := -2147483648 + -2147483648;   // fail
  i := -2147483648 + (-2147483648); // pass

  i := -2147483648 + 3;             // pass
  i := -2147483648 * 3;             // fail

  i := 0
end simple;

//
// relaxed(): fold negation with constant.
//
// relaxed() improves the unintuitive behavior of simple() by also folding an
// integer constant if it appears as the leftmost leaf in the 'term' expression
// following the inital sign in 'simpleexpr'.
//
// I.e., for the parse trees
//
//     -2147483648 + 5                  -2147483648 * 5
//
//       simpleexpr                       simpleexpr
//      /  |    |  \                        /   \
//    "-" term "+" term                   "-"   term
//         |        |                          /  |  \
//       factor   factor                 factor  "*"  factor
//         |        |                      |             |
//       number   number                 number       number
//         |        |                      |             |
//    2147483648    5                 2147483648         5
//
//
// we first identify the leftmost leaf in the first term and fold if it is
// a CAstConstant:
//
//       simpleexpr                       simpleexpr
//      /  |    |  \                        /   \
//    "-" term "+" term                   "-"  term
//     (of type)  (of type)                  (of type)
//  CAstConstant CAstConstant               CAstBinaryOp
//      ^                                  /     |     \
//      |                               factor  "*"  factor
//  fold "-" w/ CAstConstant          (of type)    (of type)
//                                   CAstConstant  CAstConstant
//                                       ^
//                                       |
//                                   fold "-" w/ CAstConstant
//
// This approach requires modifications to the AST data structure since we need
// to know whether an expression had been enclosed in parentheses or not.
// Otherwise we would wrongfully accept expressions such as
//
//     i := -(2147483648 + 5)
//
// or even worse (b is of boolean type here)
//
//     b := -(1 > 0)
//
procedure relaxed();
var i: integer;
    b: boolean;
begin
  i := -2147483648;                 // pass
  i := -2147483647;                 // pass
  i :=  2147483647;                 // pass
  i :=  2147483648;                 // fail

  i := -2147483648 + 2147483648;    // fail
  i := -2147483648 + -2147483648;   // fail
  i := -2147483648 + (-2147483648); // pass

  i := -2147483648 + 3;             // pass
  i := -2147483648 * 3;             // pass
  i := -(2147483648 + 3);           // fail
  i := -(i + 2147483648);           // fail

  i := -(2147483648);               // fail
  i := -(-(-(1)));                  // pass

  b := -((1 > 0) && (2 < 5));       // fail

  i := 0
end relaxed;

begin
end integer_const.

