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
assert  3 "main() { a=3; }"
assert  6 "main() { a=3+3; }"
assert  4 "main() { a=4; a; }"
assert  4 "main() { foo=4; foo; }"
assert  7 "main() { foo=4; bar=3; foo+bar; }"
assert  7 "main() { foo=4; fox=3; foo+fox; }"
assert  7 "main() { return 7; return 5; }"
assert  3 "main() { foo=3; return foo; }"
assert  4 "main() { return (3+5)/2; }"
assert  3 "main() { ifa=1;if(ifa==1) return 3; }"
assert  3 "main() { elsea=1;if (elsea==1) return 3; else return 5; }"
assert  5 "main() { elsea=0;if(elsea==1 ) return 3; else return 5; }"
assert  2 "main() { a=2; if(a==0) return 0; else if(a==1) return 1; else if(a==2) return 2; else return 3; }"
assert  3 "main() { a=3; if(a==0) return 0; else if(a==1) return 1; else if(a==2) return 2; else return 3; }"
assert  6 "main() { a=0; b=1; c=1; while(a<5) a = a + 2; a; }"
assert  8 "main() { for(3;a<7;a = a + 2) 3; a; }"
assert  3 "main() { { a = 0; a = a + 1; a = a * 3; } }"
assert  8 "main() { a=0; b=1; while(a<3) {a = a + 1; b = 2 * b;} b; }"

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

assert 7 "main(){a = bar(); a;} bar(){return 3+4;}"
assert 7 "main(){return bar();} bar(){return foo();} foo(){return 7;}"

echo OK
