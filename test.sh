#!/bin/bash
cat <<EOF | gcc -xc -c -o tmp2.o -
int ret3() { return 3; }
int ret5() { return 5; }
int plus2(int x, int y) { return x + y; }
int plus3(int x, int y, int z) { return x + y + z; }
int plus4(int x, int y, int z, int a) { return x + y + z + a; }
int plus5(int x, int y, int z, int a, int b) { return x + y + z + a + b; }
int plus6(int x, int y, int z, int a, int b, int c) { return x + y + z + a + b + c; }
int minus(int x, int y) { return x - y; }
EOF

assert() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  cc -o tmp tmp.s tmp2.o
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert 0 '{ return 0; }'
assert 42 '{ return 42; }'
assert 21 '{ return 5+20-4; }'
assert 41 '{ return  12 + 34 - 5 ; }'
assert 47 '{ return 5+6*7; }'
assert 15 '{ return 5*(9-6); }'
assert 4 '{ return (3+5)/2; }'
assert 10 '{ return -10+20; }'
assert 10 '{ return - -10; }'
assert 10 '{ return - - +10; }'

assert 0 '{ return 0==1; }'
assert 1 '{ return 42==42; }'
assert 1 '{ return 0!=1; }'
assert 0 '{ return 42!=42; }'

assert 1 '{ return 0<1; }'
assert 0 '{ return 1<1; }'
assert 0 '{ return 2<1; }'
assert 1 '{ return 0<=1; }'
assert 1 '{ return 1<=1; }'
assert 0 '{ return 2<=1; }'

assert 1 '{ return 1>0; }'
assert 0 '{ return 1>1; }'
assert 0 '{ return 1>2; }'
assert 1 '{ return 1>=0; }'
assert 1 '{ { return 1>=1; }} '
assert 0 '{ { return 1>=2; }} '

assert 3 '{ a=3; return a;} '
assert 8 '{ a=3; z=5; return a+z;} '

assert 3 '{ a=3; return a;} '
assert 8 '{ a=3; z=5; return a+z;} '
assert 6 '{ a=b=3; return a+b;} '
assert 3 '{ foo=3; return foo;} '
assert 8 '{ foo123=3; bar=5; return foo123+bar;} '

assert 1 '{ return 1; 2; 3;} '
assert 2 '{ 1; return 2; 3;} '
assert 3 '{ 1; 2; return 3;} '

assert 5 '{ if (1 == 1) return 5; return 3;} '
assert 3 '{ if (1 == 2) return 5; return 3;} '
assert 5 '{ if (1 == 1) if (2 == 2) return 5; return 3;} '
assert 5 '{ if (1 == 1) return 5; else return 3;} '
assert 3 '{ if (1 == 2) return 5; else return 3;} '

assert 10 'for (i = 0; i < 10; i = i + 1) i; return i;'
assert 11 'for (i = 0; i < 10; i = i + 1) i; return i + 1;'
assert 20 'a=0; for (; a < 20; a = a + 1) a; return a;'

assert 10 'i=0; while (i < 10) i = i+1; return i;'

assert 3 '{ {1; {2;} return 3;}}'

assert 3 '{ return ret3(); }'
assert 5 '{ return ret5(); }'
assert 8 '{ return plus2(3, 5); }'
assert 12 '{ return plus3(3, 4, 5); }'
assert 15 '{ return plus4(3, 4, 5, 3); }'
assert 21 '{ return plus5(3, 4, 5, 3, 6); }'
assert 6 '{ return plus6(1, 1, 1, 1, 1, 1); }'
assert 136 '{return plus6(1, 2, plus6(3, plus6(4, 5, 6, 7, 8, 9), 10, 11, 12, 13), 14, 15, 16); }'
assert 2 '{ return minus(5, 3); }'

echo OK