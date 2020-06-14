#!/bin/bash
assert() {
    expected="$1"
    input="$2"

    ./9cc "$input" > tmp.s
    cc -o tmp tmp.s lib.c
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
}

assert  0 "int main() { 0; }"
assert 42 "int main() { 42; }"
assert 21 "int main() { 5+20-4; }"
assert 41 "int main() {  12 + 34 - 5 ; }"
assert 47 'int main() { 5+6*7; }'
assert 15 'int main() { 5*(9-6); }'
assert  4 'int main() { (3+5)/2; }'
assert 10 'int main() { -10+20; }'
assert 30 'int main() { +10+20; }'
assert 10 'int main() { - -10; }'
assert 10 'int main() { - - +10; }'
assert  1 "int main() { 5+20-4 == 21; }"
assert  0 "int main() { - -10 == 21; }"
assert  1 "int main() {  12 + 34 - 5 != 40; }"
assert  0 'int main() { 5+6*7 != 47; }'
assert  1 "int main() { 5*(9-6) < 16; }"
assert  0 "int main() { 5*(9-6) < 15; }"
assert  1 "int main() { 16 >= 5*(9-6); }"
assert  0 "int main() { 15-1 >= 5*(9-6); }"
assert  1 'int main() { 4 <= (3+5)/2; }'
assert  0 'int main() { 5 <= (3+5)/2; }'
assert  1 'int main() { (3+5)/2 > 3; }'
assert  0 'int main() { (3+5)/2 > 4; }'
assert  3 "int main() { int a; a=3; }"
assert  6 "int main() { int a; a=3+3; }"
assert  4 "int main() { int a; a=4; a; }"
assert  4 "int main() { int foo; foo=4; foo; }"
assert  7 "int main() { int foo; int bar; foo=4; bar=3; foo+bar; }"
assert  7 "int main() { int foo; int fox; foo=4; fox=3; foo+fox; }"
assert  7 "int main() { return 7; return 5; }"
assert  3 "int main() { int foo; foo=3; return foo; }"
assert  4 "int main() { return (3+5)/2; }"
assert  3 "int main() { int ifa; ifa=1;if(ifa==1) return 3; }"
assert  3 "int main() { int elsea; elsea=1;if (elsea==1) return 3; else return 5; }"
assert  5 "int main() { int elsea; elsea=0;if(elsea==1 ) return 3; else return 5; }"
assert  2 "int main() { int a; a=2; if(a==0) return 0; else if(a==1) return 1; else if(a==2) return 2; else return 3; }"
assert  3 "int main() { int a; a=3; if(a==0) return 0; else if(a==1) return 1; else if(a==2) return 2; else return 3; }"
assert  6 "int main() { int a; int b; int c; a=0; b=1; c=1; while(a<5) a = a + 2; a; }"
assert  8 "int main() { int a; for(3;a<7;a = a + 2) 3; a; }"
assert  3 "int main() { int a; { a = 0; a = a + 1; a = a * 3; } }"
assert  8 "int main() { int a; int b; a=0; b=1; while(a<3) {a = a + 1; b = 2 * b;} b; }"

assert 0 "int main(){foo0(); return 0;}"
assert 0 "int main(){foo1(1); return 0;}"
assert 0 "int main(){foo2(1,2); return 0;}"
assert 0 "int main(){foo3(1,2,3); return 0;}"
assert 0 "int main(){foo4(1,2,3,4); return 0;}"
assert 0 "int main(){foo5(1,2,3,4,5); return 0;}"
assert 0 "int main(){foo6(1,2,3,4,5,6); return 0;}"
assert 0 "int main(){foo7(1,2,3,4,5,6,7); return 0;}"
assert 0 "int main(){foo8(1,2,3,4,5,6,7,8); return 0;}"
assert 0 "int main(){foo9(1,2,3,4,5,6,7,8,9); return 0;}"

assert 0 "int main(){bar();} int bar(){foo0(); return 0;}"

assert 7 "int main(){int a; a = bar(); a;} int bar(){return 3+4;}"
assert 7 "int main(){return bar();} int bar(){return foo();} int foo(){return 7;}"
assert 7 "int main(){return bar();} int bar(){int a; a = foo(); return a;} int foo(){int a; int b; a = 3; b = 4;return a+b;}"
assert 7 "int main(){return bar(3);} int bar(int a){return a+4;}"

assert 0 "int main(){return bar(0);} int bar(int a) {return a;}"
assert 1 "int main(){return bar(0,1);} int bar(int a,int b) {return b;}"
assert 2 "int main(){return bar(0,1,2);} int bar(int a,int b,int c) {return c;}"
assert 3 "int main(){return bar(0,1,2,3);} int bar(int a,int b,int c,int d) {return d;}"
assert 4 "int main(){return bar(0,1,2,3,4);} int bar(int a,int b,int c,int d,int e) {return e;}"
assert 5 "int main(){return bar(0,1,2,3,4,5);} int bar(int a,int b,int c,int d,int e,int f) {return f;}"
assert 0 "int main(){return bar(0,1,2,3,4,5);} int bar(int a,int b,int c,int d,int e,int f) {return a;}"
assert 1 "int main(){return bar(0,1,2,3,4,5);} int bar(int a,int b,int c,int d,int e,int f) {return b;}"
assert 2 "int main(){return bar(0,1,2,3,4,5);} int bar(int a,int b,int c,int d,int e,int f) {return c;}"
assert 3 "int main(){return bar(0,1,2,3,4,5);} int bar(int a,int b,int c,int d,int e,int f) {return d;}"
assert 4 "int main(){return bar(0,1,2,3,4,5);} int bar(int a,int b,int c,int d,int e,int f) {return e;}"
assert 5 "int main(){return bar(0,1,2,3,4,5);} int bar(int a,int b,int c,int d,int e,int f) {return f;}"

assert 3 "int main(){int x; int *y; x = 3; y = &x; return *y;}"
assert 3 "int main(){int x; int y; int *z; x = 3; y = 5; z = &y + 8; return *z;}"
assert 3 "int main(){int x; int *y; y = &x; *y = 3; return x;}"
assert 5 "int main(){int x; int *y; int **z; y = &x; z = &y; **z = 5; return x;}"

assert 4 "int main(){int *p; alloc4(&p, 1, 2, 4, 8); int *q; q = p + 2; *q;}"
assert 8 "int main(){int *p; alloc4(&p, 1, 2, 4, 8); int *q; q = p + 2; *q; q = p + 3; return *q;}"
assert 2 "int main(){int *p; alloc4(&p, 1, 2, 4, 8); int *q; q = p + 3; q = q - 2; return *q;}"

# ./9cc "int main(){int x; y = 3;}" # errorになる
# ./9cc "main(){int x; x = 3;}" # errorになる
# ./9cc "int main(){bar();} bar() { return 3; }" # errorになる

#assert 6 "int main(){return bar(0,1,2,3,4,5,6);}       int bar(int a,int b,int c,int d,int e,int f,int g) {return g;}"
#assert 7 "int main(){return bar(0,1,2,3,4,5,6,7);}     int bar(int a,int b,int c,int d,int e,int f,int g,int h) {return h;}"
#assert 0 "int main(){return bar(0,1,2,3,4,5,6,7,8,9);} int bar(int a,int b,int c,int d,int e,int f,int g,int h,int i,int j) {return a;}"
#assert 1 "int main(){return bar(0,1,2,3,4,5,6,7,8,9);} int bar(int a,int b,int c,int d,int e,int f,int g,int h,int i,int j) {return b;}"
#assert 2 "int main(){return bar(0,1,2,3,4,5,6,7,8,9);} int bar(int a,int b,int c,int d,int e,int f,int g,int h,int i,int j) {return c;}"
#assert 3 "int main(){return bar(0,1,2,3,4,5,6,7,8,9);} int bar(int a,int b,int c,int d,int e,int f,int g,int h,int i,int j) {return d;}"
#assert 4 "int main(){return bar(0,1,2,3,4,5,6,7,8,9);} int bar(int a,int b,int c,int d,int e,int f,int g,int h,int i,int j) {return e;}"
#assert 5 "int main(){return bar(0,1,2,3,4,5,6,7,8,9);} int bar(int a,int b,int c,int d,int e,int f,int g,int h,int i,int j) {return f;}"
#assert 6 "int main(){return bar(0,1,2,3,4,5,6,7,8,9);} int bar(int a,int b,int c,int d,int e,int f,int g,int h,int i,int j) {return g;}"
#assert 7 "int main(){return bar(0,1,2,3,4,5,6,7,8,9);} int bar(int a,int b,int c,int d,int e,int f,int g,int h,int i,int j) {return h;}"
#assert 8 "int main(){return bar(0,1,2,3,4,5,6,7,8,9);} int bar(int a,int b,int c,int d,int e,int f,int g,int h,int i,int j) {return i;}"
#assert 9 "int main(){return bar(0,1,2,3,4,5,6,7,8,9);} int bar(int a,int b,int c,int d,int e,int f,int g,int h,int i,int j) {return j;}"

echo OK
