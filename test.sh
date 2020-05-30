#!/bin/bash
assert() {
    expected="$1"
    input="$2"

    ./9cc "$input" > tmp.s
    cc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
}

assert  0 "main() { 0; }"
assert 42 "main() { 42; }"
assert 21 "main() { 5+20-4; }"
assert 41 "main() {  12 + 34 - 5 ; }"
assert 47 'main() { 5+6*7; }'
assert 15 'main() { 5*(9-6); }'
assert  4 'main() { (3+5)/2; }'
assert 10 'main() { -10+20; }'
assert 30 'main() { +10+20; }'
assert 10 'main() { - -10; }'
assert 10 'main() { - - +10; }'
assert  1 "main() { 5+20-4 == 21; }"
assert  0 "main() { - -10 == 21; }"
assert  1 "main() {  12 + 34 - 5 != 40; }"
assert  0 'main() { 5+6*7 != 47; }'
assert  1 "main() { 5*(9-6) < 16; }"
assert  0 "main() { 5*(9-6) < 15; }"
assert  1 "main() { 16 >= 5*(9-6); }"
assert  0 "main() { 15-1 >= 5*(9-6); }"
assert  1 'main() { 4 <= (3+5)/2; }'
assert  0 'main() { 5 <= (3+5)/2; }'
assert  1 'main() { (3+5)/2 > 3; }'
assert  0 'main() { (3+5)/2 > 4; }'
assert  3 "main() { int a; a=3; }"
assert  6 "main() { int a; a=3+3; }"
assert  4 "main() { int a; a=4; a; }"
assert  4 "main() { int foo; foo=4; foo; }"
assert  7 "main() { int foo; int bar; foo=4; bar=3; foo+bar; }"
assert  7 "main() { int foo; int fox; foo=4; fox=3; foo+fox; }"
assert  7 "main() { return 7; return 5; }"
assert  3 "main() { int foo; foo=3; return foo; }"
assert  4 "main() { return (3+5)/2; }"
assert  3 "main() { int ifa; ifa=1;if(ifa==1) return 3; }"
assert  3 "main() { int elsea; elsea=1;if (elsea==1) return 3; else return 5; }"
assert  5 "main() { int elsea; elsea=0;if(elsea==1 ) return 3; else return 5; }"
assert  2 "main() { int a; a=2; if(a==0) return 0; else if(a==1) return 1; else if(a==2) return 2; else return 3; }"
assert  3 "main() { int a; a=3; if(a==0) return 0; else if(a==1) return 1; else if(a==2) return 2; else return 3; }"
assert  6 "main() { int a; int b; int c; a=0; b=1; c=1; while(a<5) a = a + 2; a; }"
assert  8 "main() { int a; for(3;a<7;a = a + 2) 3; a; }"
assert  3 "main() { int a; { a = 0; a = a + 1; a = a * 3; } }"
assert  8 "main() { int a; int b; a=0; b=1; while(a<3) {a = a + 1; b = 2 * b;} b; }"

./9cc "main(){foo0();}" > tmp.s
cc -o tmp tmp.s lib.c
./tmp

./9cc "main(){foo1(1);}" > tmp.s
cc -o tmp tmp.s lib.c
./tmp

./9cc "main(){foo2(1,2);}" > tmp.s
cc -o tmp tmp.s lib.c
./tmp

./9cc "main(){foo3(1,2,3);}" > tmp.s
cc -o tmp tmp.s lib.c
./tmp

./9cc "main(){foo4(1,2,3,4);}" > tmp.s
cc -o tmp tmp.s lib.c
./tmp

./9cc "main(){foo5(1,2,3,4,5);}" > tmp.s
cc -o tmp tmp.s lib.c
./tmp

./9cc "main(){foo6(1,2,3,4,5,6);}" > tmp.s
cc -o tmp tmp.s lib.c
./tmp

./9cc "main(){foo7(1,2,3,4,5,6,7);}" > tmp.s
cc -o tmp tmp.s lib.c
./tmp

./9cc "main(){foo8(1,2,3,4,5,6,7,8);}" > tmp.s
cc -o tmp tmp.s lib.c
./tmp

./9cc "main(){foo9(1,2,3,4,5,6,7,8,9);}" > tmp.s
cc -o tmp tmp.s lib.c
./tmp


./9cc "main(){bar();}bar(){foo0();}" > tmp.s
cc -o tmp tmp.s lib.c
./tmp

assert 7 "main(){int a; a = bar(); a;} bar(){return 3+4;}"
assert 7 "main(){return bar();} bar(){return foo();} foo(){return 7;}"
assert 7 "main(){return bar();} bar(){int a; a = foo(); return a;} foo(){int a; int b; a = 3; b = 4;return a+b;}"
assert 7 "main(){return bar(3);} bar(a){return a+4;}"

assert 0 "main(){return bar(0);} bar(a) {return a;}"
assert 1 "main(){return bar(0,1);} bar(a,b) {return b;}"
assert 2 "main(){return bar(0,1,2);} bar(a,b,c) {return c;}"
assert 3 "main(){return bar(0,1,2,3);} bar(a,b,c,d) {return d;}"
assert 4 "main(){return bar(0,1,2,3,4);} bar(a,b,c,d,e) {return e;}"
assert 5 "main(){return bar(0,1,2,3,4,5);} bar(a,b,c,d,e,f) {return f;}"
assert 0 "main(){return bar(0,1,2,3,4,5);} bar(a,b,c,d,e,f) {return a;}"
assert 1 "main(){return bar(0,1,2,3,4,5);} bar(a,b,c,d,e,f) {return b;}"
assert 2 "main(){return bar(0,1,2,3,4,5);} bar(a,b,c,d,e,f) {return c;}"
assert 3 "main(){return bar(0,1,2,3,4,5);} bar(a,b,c,d,e,f) {return d;}"
assert 4 "main(){return bar(0,1,2,3,4,5);} bar(a,b,c,d,e,f) {return e;}"
assert 5 "main(){return bar(0,1,2,3,4,5);} bar(a,b,c,d,e,f) {return f;}"

assert 3 "main(){int x; int y; x = 3; y = &x; return *x;}"
assert 3 "main(){int x; int y; int z; x = 3; y = 5; z = &y + 8; return *x;}"

# ./9cc "main(){int x; y = 3;}" # errorになる

#assert 6 "main(){return bar(0,1,2,3,4,5,6);} bar(a,b,c,d,e,f,g) {return g;}"
#assert 7 "main(){return bar(0,1,2,3,4,5,6,7);} bar(a,b,c,d,e,f,g,h) {return h;}"
#assert 0 "main(){return bar(0,1,2,3,4,5,6,7,8,9);} bar(a,b,c,d,e,f,g,h,i,j) {return a;}"
#assert 1 "main(){return bar(0,1,2,3,4,5,6,7,8,9);} bar(a,b,c,d,e,f,g,h,i,j) {return b;}"
#assert 2 "main(){return bar(0,1,2,3,4,5,6,7,8,9);} bar(a,b,c,d,e,f,g,h,i,j) {return c;}"
#assert 3 "main(){return bar(0,1,2,3,4,5,6,7,8,9);} bar(a,b,c,d,e,f,g,h,i,j) {return d;}"
#assert 4 "main(){return bar(0,1,2,3,4,5,6,7,8,9);} bar(a,b,c,d,e,f,g,h,i,j) {return e;}"
#assert 5 "main(){return bar(0,1,2,3,4,5,6,7,8,9);} bar(a,b,c,d,e,f,g,h,i,j) {return f;}"
#assert 6 "main(){return bar(0,1,2,3,4,5,6,7,8,9);} bar(a,b,c,d,e,f,g,h,i,j) {return g;}"
#assert 7 "main(){return bar(0,1,2,3,4,5,6,7,8,9);} bar(a,b,c,d,e,f,g,h,i,j) {return h;}"
#assert 8 "main(){return bar(0,1,2,3,4,5,6,7,8,9);} bar(a,b,c,d,e,f,g,h,i,j) {return i;}"
#assert 9 "main(){return bar(0,1,2,3,4,5,6,7,8,9);} bar(a,b,c,d,e,f,g,h,i,j) {return j;}"

echo OK
