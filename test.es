import booleans.es ;
import sets.es ;
import numbers.es ;

import lists.es ;

Node : a -> (List (Tree a)) -> (Tree a) ;
type Individual = (Tree Int) ;

listToSet : (List (List Int)) -> ((List Int) -> Bool) ;
(listToSet []) = \x.False ;
(listToSet (# y z)) = (union \x.(= x y) (listToSet z)) ;

branches : Individual -> (List (List Int)) ;
(branches (Node x [])) = (# (# x []) []) ;
(branches (Node x (# y z))) = 
       (map (# x) 
        (concatall (map branches (# y z)))) ;

concatall : (List (List a)) -> (List a) ;
(concatall []) = [] ;
(concatall (# x y)) = (concat (x,(concatall y))) ;

setExists1 : (a -> Bool) -> (a -> Bool) -> Bool ;
(setExists1 p t) = (sigma \x.(&& (t x) (p x))) ;

tree1 : (Tree Int) ;
tree1 = (Node 89 (# (Node 6 
                 (# (Node 8 []) (# (Node 22 []) (# (Node 30 []) 
                 (# (Node 31 []) (# (Node 32 []) []))))))
           (# (Node 10 []) 
           (# (Node 78 []) [])))) ;

top : free -> Bool ;
(top x) = True ;


{-
: (listToSet ((# ((# 89) ((# 6) ((# 8) [])))) 
	   ((# ((# 89) ((# 6) ((# 22) [])))) 
           ((# ((# 89) ((# 6) ((# 30) [])))) 
	   ((# ((# 89) ((# 6) ((# 31) [])))) 
	   ((# ((# 89) ((# 6) ((# 32) [])))) 
	   ((# ((# 89) ((# 10) []))) 
	   ((# ((# 89) ((# 78) []))) [])))))))) ;
-}
{-
: \exists x.(listToSet ((# ((# 89) ((# 6) ((# 8) [])))) ((# ((# 89) ((# 6) ((# 22) [])))) ((# ((# 89) ((# 6) ((# 30) [])))) ((# ((# 89) ((# 6) ((# 31) [])))) ((# ((# 89) ((# 6) ((# 32) [])))) ((# ((# 89) ((# 10) []))) ((# ((# 89) ((# 78) []))) []))))))) x) ;
-}

-- : (setExists1 top (listToSet (branches tree1))) ;

: ((comp (comp branches listToSet) (setExists1 top)) ((Node 89) ((# ((Node 6) ((# ((Node 8) [])) ((# ((Node 22) [])) ((# ((Node 30) [])) ((# ((Node 31) [])) ((# ((Node 32) [])) []))))))) ((# ((Node 10) [])) ((# ((Node 78) [])) []))))) ;

-- ----

range : number -> Bool ;
range = \x.(|| (= x 1) (|| (= x 2) (|| (= x 3) (|| (= x 4) (= x 5))))) ;

-- : \exists n.(&& (range n) (= 496 (mul (power (2,(sub n 1))) (sub (power (2,n)) 1)))) ;

-- : (&& (&& (|| t (w x)) (= x 5)) (|| (y x) (z x))) ;
-- : (&& (y x) (&& (|| (= x 5) (t x)) (z x))) ;


-- : (permute ([1,2,x,y,5],[1,2,3,4,5])) ;
-- : (permute ([1,2,3,4,5],[1,2,x,y,5])) ;


and2 : (a -> Bool) -> (a -> Bool) -> a -> Bool ;
(and2 top p) = p ;
(and2 p top) = p ;
(and2 p q x) = (&& (p x) (q x)) ;

-- : (and2 p top) ;

-- : (and2 p q x) ;
{-
set1 : Int -> Bool ;
set1 = \x.(ite (= x 7) True
          (ite (= x 3) True 
          (ite (= x 5) True 
          (ite (= x 2) True
	  (ite (= x 8) True
	  (ite (= x 12) True
	  (ite (= x 17) True
          (ite (= x 23) True 
          (ite (= x 54) True 
          (ite (= x 24) True
	  (ite (= x 38) True
	  (ite (= x 22) True
	  (ite (= x 27) True
          (ite (= x 33) True 
          (ite (= x 55) True 
          (ite (= x 209) True
	  (ite (= x 813) True
	  (ite (= x 122) True
	  (ite (= x 175) True
          (ite (= x 232) True 
          (ite (= x 542) True 
          (ite (= x 29) True
	  (ite (= x 380) True
	  (ite (= x 212) True
          (ite (= x 10) True False))))))))))))))))))))))))) ;
-}
{-
: (cardBool (removeBound set1)) ;
: (integer (cardBool (removeBound set1)) 2) ;
: (get 2.0 (removeBound set1)) ;
: (get (integer 4 2) (removeBound set1)) ;
: (get (integer 5 2) (removeBound set1)) ;
: (get (integer (cardBool (removeBound set1)) 2) (removeBound set1)) ;
: (midEle (removeBound set1)) ;
-}

-- : (sortIte (removeBound set1)) ;

-- : (makeBTree set1) ;

set2 : Int -> Bool ;
set2 = \x.((((switch (((ite ((= x) 27)) 1) (((ite ((< x) 27)) 2) 3))) True) ((((switch (((ite ((= x) 8)) 1) (((ite ((< x) 8)) 2) 3))) True) 
((((switch (((ite ((= x) 3)) 1) (((ite ((< x) 3)) 2) 3))) True) ((= x) 2)) ((((switch (((ite ((= x) 5)) 1) (((ite ((< x) 5)) 2) 3))) True) False) ((= x) 7)))) ((((switch (((ite ((= x) 17)) 1) (((ite ((< x) 17)) 2) 3))) True) 
((((switch (((ite ((= x) 10)) 1) (((ite ((< x) 10)) 2) 3))) True) False) ((= x) 12))) 
((((switch (((ite ((= x) 22)) 1) (((ite ((< x) 22)) 2) 3))) True) False) ((((switch (((ite ((= x) 23)) 1) (((ite ((< x) 23)) 2) 3))) True) False) ((= x) 24)))))) 
((((switch (((ite ((= x) 122)) 1) (((ite ((< x) 122)) 2) 3))) True) ((((switch (((ite ((= x) 33)) 1) (((ite ((< x) 33)) 2) 3))) True) ((= x) 29)) ((((switch (((ite ((= x) 38)) 1) (((ite ((< x) 38)) 2) 3))) True) False) 
((((switch (((ite ((= x) 54)) 1) (((ite ((< x) 54)) 2) 3))) True) False) ((= x) 55))))) 
((((switch (((ite ((= x) 212)) 1) (((ite ((< x) 212)) 2) 3))) True) ((((switch (((ite ((= x) 175)) 1) (((ite ((< x) 175)) 2) 3))) True) False) ((= x) 209))) ((((switch (((ite ((= x) 380)) 1) (((ite ((< x) 380)) 2) 3))) True) ((= x) 232)) ((((switch (((ite ((= x) 542)) 1) (((ite ((< x) 542)) 2) 3))) True) False) ((= x) 813)))))) ;

{-
: (set1 10) ;
: (set2 10) ;

: (set1 27) ;
: (set2 27) ;

: (set1 7) ;
: (set2 7) ;

: (set1 11) ;
: (set2 11) ;
-}

str1 : String ;
str1 = "Peter";
str2 : String ;
str2 = "Vadim" ;

-- : (= str1 str2) ;

type Mytype = (Int * Int * Int) ;

d1 : Mytype ;
d1 = (3,2,5) ;

d2 : Mytype ;
d2 = (9,9,9) ;

(= (x1_SV/CONST/,x2,x3) (x4_SV/NOTEQUAL,x1_SV/,x5,x6)) = False ;

-- : (= d1 d2) ;

{-
add3 : Int -> Int -> Int -> (Int -> Int) ;
(add3 x y z) = (add3 (add x y) z) ;

: (add3 2 3 4 5) ;
-}


