list1 : (List Int) ;
list1 = [] ;

list2 : (List Int) ;
list2 = (# 1 []) ;

list3 : (List Int) ;
list3 = (# 1 (# 2 [])) ;

list4 : (List Int) ;
list4 = (# 1 list3) ;

list5 : (List Int) ;
list5 = (# 2 list4) ;

list6 : (List Int) ;
list6 = (# 1 (# 1 (# 2 (# 3 (# 4 []))))) ;

list7 : (List Int) ;
list7 = (# 1 (# 1 (# 2 (# 3 (# 3 (# 4 (# 4 (# 4 [])))))))) ;

list8 : (List Int) ;
list8 = (# 1 (# 1 (# 2 (# 3 (# 3 (# 4 (# 4 (# 3 [])))))))) ;

list88 : (List Int) ;
list88 = (# 1 (# 1 (# 2 (# 3 (# 3 (# 4 (# 4 (# 3 [])))))))) ;

list9 : (List Int) ;
list9 = (# 1 (# 1 (# 2 (# 3 (# 3 []))))) ;

us1 : (List Int) ;
us1 = (# 199 (# 3 (# 2 (# 1 (# 99 (# 12 (# 20 (# 21 (# 51 (# 42 [])))))))))) ; 

us2 : (List Int) ;
us2 = (# 7 (# 33 (# 120 (# 1 (# 199 (# 1012 (# 1120 (# 821 (# 851 (# 542 us1)))))))))) ; 

us3 : (List Int) ;
us3 = (# 0 (# 44 (# 12 (# 15 (# 990 (# 125 (# 2220 (# 921 (# 511 (# 442 us2)))))))))) ;

us4 : (List Int) ;
us4 = (# 20 (# 98 (# 290 (# 10 (# 90 (# 123 (# 2300 (# 210 (# 513 (# 342 us3)))))))))) ;

us5 : (List Int) ;
us5 = (# 13 (# 32 (# 29 (# 9 (# 299 (# 122 (# 200 (# 219 (# 5134 (# 242 us4)))))))))) ;

us6 : (List Int) ;
us6 = (# 180 (# 39 (# 27 (# 13 (# 91 (# 112 (# 25 (# 211 (# 151 (# 142 us5)))))))))) ;

-- This is an example of a function with arity 1 but effective arity 0
set1 : Int -> Bool ;
set1 = \x.if (= x 1) then True else if (= x 2) then True else False ;

set2 : Int -> Bool ;
set2 = \x.if (= x 2) then True else if (= x 1) then True else False ;

set3 : Int -> Bool ;
set3 = \x.if (= x 1) then True else False ;

set4 : Int -> Bool ;
set4 = \y.if (= y 1) then True else False ;

set5 : Int -> Bool ;
set5 = \x.if (= x 2) then True else if (= x 3) then True else False ;

{-
Annie, Bill, Mary, Joe, Harry, Ginny : People ;
prod0 : (People * People * People * People) ;
prod0 = (Annie, Bill, Mary, Joe) ;
prod1 : (People * People * People * People) ;
prod1 = (Annie, Harry, Ginny, Joe) ;
prod2 : (People * People * People * People) ;
prod2 = (Annie, Harry, Ginny, Joe) ;
-}
