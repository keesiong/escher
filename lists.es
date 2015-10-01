-- [] : (List a) ;
-- # : a -> (List a) -> (List a) ;

import numbers.es ;

head : (List a) -> a ;
(head (# x y)) = x ;

tail : (List a) -> (List a) ;
(tail (# x y)) = y ;

last : (List a) -> a ;
(last (# x [])) = x;
(last (# x (# y z))) = (last (# y z)) ;

elem : Int -> (List a) -> a ;
(elem 1 (# x y)) = x ;
(elem z_SV/CONST/ (# x y)) = (elem (sub z_SV 1) y) ;

enumList : Int -> (List Int) ;
(enumList x_SV/CONST/) = (enumList2 x_SV x_SV) ;

enumList2 : Int -> Int -> (List Int) ;
(enumList2 0 x) = [] ;
(enumList2 x_SV/CONST/ y_SV/CONST/) = 
          (# (add (sub y_SV x_SV) 1) (enumList2 (sub x_SV 1) y_SV)) ;

inList : a -> (List a) -> Bool ;
(inList x []) = False ;
(inList x (# y z)) = if (= x y) then True else (inList x z) ;

length : (List a) -> Int ;
(length []) = 0 ;
(length (# x y)) = (add 1 (length y)) ;

zip : (List a) -> (List b) -> (List (a * b)) ;
(zip [] []) = [] ;
(zip (# x1 y1) (# x2 y2)) = (# (x1,x2) (zip y1 y2)) ;

zipWith : a -> (List b) -> (List (a * b)) ;
(zipWith x []) = [] ;
(zipWith x (# y z)) = (# (x,y) (zipWith x z)) ;

concat : ((List a) * (List a)) -> (List a) ;
(concat ([],x)) = x ;
(concat ((# u x), y)) = (# u (concat (x, y))) ;

concat2 : (List a) -> (List a) -> (List a) ;
(concat2 [] x) = x ;
(concat2 (# u x) y) = (# u (concat2 x y)) ;

reverse : (List a) -> (List a) ;
(reverse []) = [] ;
(reverse (# x y)) = (concat ((reverse y),[x])) ;

append : ((List a) * (List a) * (List a)) -> Bool ;
(append (u,v,w)) = 
     (|| (&& (= u []) (= v w))
         (sigma \r.
          (sigma \x.
           (sigma \y.(&& (&& (= u (# r x)) (= w (# r y))) 
                         (append (x,v,y))))))) ;

permute : ((List a) * (List a)) -> Bool ;
(permute ([], x)) = (= x []) ;
(permute ((# x y), w)) =
    (sigma \u.(sigma \v.(sigma \z.
       (&& (= w (# u v)) (&& (delete (u,(# x y),z)) (permute (z,v))))))) ;

delete : (a * (List a) * (List a)) -> Bool ;             
(delete (x,[],y)) = False ;
(delete (x,(# y z),w)) = 
    (|| (&& (= x y) (= w z))
         (sigma \v.(&& (= w (# y v)) (delete (x,z,v))))) ;

sorted : (List a) -> Bool ;
(sorted []) = True ;
(sorted (# x y)) = 
   if (= y []) then True
   else (sigma \u.(sigma \v.(&& (&& (= y (# u v)) (<= x u)) (sorted y)))) ;

isort : (List a) -> (List a) ;
(isort []) = [] ;
(isort (# x y)) = (ins x (isort y)) ;

ins : a -> (List a) -> (List a) ;
(ins x []) = (# x []) ;
(ins x (# y z)) = if (<= x y) then (# x (# y z)) else (# y (ins x z)) ;

isort2 : (a -> a -> Bool) -> (List a) -> (List a) ;
(isort2 p []) = [] ;
(isort2 p (# x y)) = (ins2 p x (isort2 p y)) ;

ins2 : (a -> a -> Bool) -> a -> (List a) -> (List a) ;
(ins2 p x []) = (# x []) ;
(ins2 p x (# y z)) = if (p x y) then (# x (# y z)) else (# y (ins2 p x z)) ;

fold : (a -> b -> b) -> b -> (List a) -> b ;
(fold m v []) = v ;
(fold m v (# x y)) = (m x (fold m v y)) ;

foldr : (a -> b -> b) -> b -> (List a) -> b;
(foldr m s []) = s ;
(foldr m s (# x y)) = (m x (foldr m s y)) ;

filter : (a -> Bool) -> (List a) -> (List a) ;
(filter p []) = [] ;
(filter p (# x y)) = if (p x) then (# x (filter p y)) else (filter p y) ;

map : (a -> b) -> (List a) -> (List b) ;
(map m []) = [] ;
(map m (# x [])) = (# (m x) []) ;
(map m (# x y)) = (# (m x) (map m y)) ;

rmduplicates : (List a) -> (List a) ;
(rmduplicates []) = [] ;
(rmduplicates (# x y)) = (# x (rmduplicates (removeListEle x y))) ;

removeListEle : a -> (List a) -> (List a) ;
(removeListEle x []) = [] ;
(removeListEle x (# y z)) = if (= x y) then (removeListEle x z) 
                            else (# y (removeListEle x z)) ;

neg : (a -> Bool) -> a -> Bool ;
(neg p x) = (not (p x)) ;

qsort : (List a) -> (List a) ;
(qsort []) = [] ;
(qsort (# x y)) = 
  (concat ((qsort (filter (neg (< x)) y)),
           (# x (qsort (filter (< x) y))))) ;

listExists : (a -> Bool) -> (List a) -> Bool ;
(listExists p []) = False ;
(listExists p (# x y)) = if (p x) then True else (listExists p y) ;

sublist : Int -> (List a) -> (List a) ;
(sublist n []) = [] ;
(sublist n (# x y)) = if (> n 0) then (# x (sublist (sub n 1) y)) else [] ; 

isSublist : (List a) -> (List a) -> Bool;
(isSublist [] x) = True ;
(isSublist (# x y) []) = False ;
(isSublist (# x1 y1) (# x2 y2)) = 
              if (= x1 x2) then (isSublist y1 y2) else False ;

ints : Int -> Int -> (List Int) ;
(ints x y) = if (< x y) then (# x (ints (add x 1) y)) else (# x []) ;
