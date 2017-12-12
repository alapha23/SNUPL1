//
// test04
//
// computations with boolean arrays
//
// expected output: 0001111111 (no newline)
//
// if you get an endless loop, make sure you store
// booleans as 1-byte values.
//

module test04;

var a : integer;
    i : integer;

begin
  i := 0;
  while (i < 10) do
    a := i ;
    WriteInt(a);
    i := i+1
  end
end test04.
