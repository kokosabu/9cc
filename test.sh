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

assert 0 "0;"
assert 42 "42;"
assert 21 "5+20-4;"
assert 41 " 12 + 34 - 5 ;"
assert 47 '5+6*7;'
assert 15 '5*(9-6);'
assert 4 '(3+5)/2;'
assert 10 '-10+20;'
assert 30 '+10+20;'
assert 10 '- -10;'
assert 10 '- - +10;'
assert 1 "5+20-4 == 21;"
assert 0 "- -10 == 21;"
assert 1 " 12 + 34 - 5 != 40;"
assert 0 '5+6*7 != 47;'
assert 1 "5*(9-6) < 16;"
assert 0 "5*(9-6) < 15;"
assert 1 "16 >= 5*(9-6);"
assert 0 "15-1 >= 5*(9-6);"
assert 1 '4 <= (3+5)/2;'
assert 0 '5 <= (3+5)/2;'
assert 1 '(3+5)/2 > 3;'
assert 0 '(3+5)/2 > 4;'
assert 3 "a=3;"
assert 6 "a=3+3;"
assert 4 "a=4; a;"
assert 4 "foo=4; foo;"
assert 7 "foo=4; bar=3; foo+bar;"
assert 7 "foo=4; fox=3; foo+fox;"
assert 7 "return 7; return 5;"
assert 3 "foo=3; return foo;"
assert 4 "return (3+5)/2;"
assert 3 "ifa=1;if(ifa==1) return 3;"
assert 3 "elsea=1;if (elsea==1) return 3; else return 5;"
assert 5 "elsea=0;if(elsea==1 ) return 3; else return 5;"
assert 2 "a=2; if(a==0) return 0; else if(a==1) return 1; else if(a==2) return 2; else return 3;"
assert 3 "a=3; if(a==0) return 0; else if(a==1) return 1; else if(a==2) return 2; else return 3;"
assert 6 "a=0; b=1; c=1; while(a<5) a = a + 2; a;"
assert 8 "for(3;a<7;a = a + 2) 3; a;"

#assert 0 '0==1'
#assert 1 '42==42'
#assert 1 '0!=1'
#assert 0 '42!=42'

#assert 1 '0<1'
#assert 0 '1<1'
#assert 0 '2<1'
#assert 1 '0<=1'
#assert 1 '1<=1'
#assert 0 '2<=1'

#assert 1 '1>0'
#assert 0 '1>1'
#assert 0 '1>2'
#assert 1 '1>=0'
#assert 1 '1>=1'
#assert 0 '1>=2'

echo OK
