-- Equality and Disequality

remove : (a -> Bool) -> (a -> b) -> (a -> b) ;
(remove s \x.d_SV/CONST/) = \x.d_SV ;
     -- where d_SV is a default term (FIX THIS)

(remove s \x.if u_SV then v else w_SV) =
               \x.if (&& u_SV (not (s x))) then v else ((remove s \x.w_SV) x) ;

-- = : a -> a -> Bool ;
import sets.es ;

(= \x.u_SV \y.v_SV) = 
        (&& (less \x.u_SV \y.v_SV) (less \y.v_SV \x.u_SV)) ;

less : (a -> b) -> (a -> b) -> Bool ;
(less \x.d_SV/CONST/ z) = True ;
     -- where d_SV is a default term (FIX THIS)
(less \x.if u_SV then v else w_SV z) =
             \forall x.(&& (implies u_SV (= v (z x))) 
                           (less (remove \x.u_SV \x.w_SV) z)) ;


ite : (Bool * a * a) -> a ;
if True then u else v = u ;
if False then u else v = v ;
if x then x_SV else y_SV/EQUAL,x_SV/ = x_SV ;

if if x then y else w then True else z = if x then y else (glueite w z) ;

glueite : b -> a -> b ;
(glueite False w) = w ;
(glueite 0.0 w) = w ;
(glueite if x then y else z w) = if x then y else (glueite z w) ; Eager ; 
  -- this Eagerness is necessary to ensure correctness

-- if (if u then True else False) then True else v = if u then True else v ;
-- if u_SV then False else v_SV/EQUAL,u_SV/ = False ;
-- if u_SV then True else (if v_SV/EQUAL,u_SV/ then True else w_SV = 
--                   if v_SV then True else w_SV ;

-- this something useful to convert if-then-else's to ||'s and &&'s
-- if u then v else w = (|| (&& u v) (&& (not u) w)) ;

-- (if x then y else z w) = if x then (y w) else (z w) ; LastResort ;
-- \forall w.\forall x.\forall y.\forall z.(= (w if x then y else z) if x then (w y) else (w z)) ; LastResort ;

&& : Bool -> Bool -> Bool ;
(&& True x) = x ; 
(&& x True) = x ;
(&& False x) = False ;
(&& x False) = False ;
-- Do we really need these? Apparently permute need them.
(&& (|| x y) z) = (|| (&& x z) (&& y z)) ; LastResort ;
(&& x (|| y z)) = (|| (&& x y) (&& x z)) ; LastResort ;
(&& if u then v else w t) = if (&& u t) then v else (&& w t) ; LastResort ;
(&& t if u then v else w) = if (&& t u) then v else (&& t w) ; LastResort ;

-- The following are specialized versions of the two rules above.
-- In computations, I find that the two rules given above tend to work 
-- (really) badly when used in conjunction with Escher's leftmost outermost 
-- reduction order. A more in-depth analysis of this phenomenon is called for. 
(&& if (= z u) then v else w t) = 
                  if (&& (= z u) t) then v else (&& w t) ; LastResort ;
(&& t if (= x u) then v else w) = 
                  if (&& t (= x u)) then v else (&& t w) ; LastResort ;

|| : Bool -> Bool -> Bool ;
(|| True x) = True ;
(|| x True) = True ;
(|| False x) = x ;
(|| x False) = x ;

(|| if u then True else w t) = if u then True else (|| w t) ; LastResort ;
(|| if u then False else w t) = (|| (&& (not u) w) t) ; LastResort ;

(|| t if u then True else w) = if u then True else (|| t w) ; LastResort ;
(|| t if u then False else w) = (|| t (&& (not u) w)) ; LastResort ;

-- this is needed when using rmdup2
(|| (= x_SV u_SV) (= y_SV/EQUAL,x_SV/ v_SV/EQUAL,u_SV/)) = (= x_SV u_SV) ;

not : Bool -> Bool ;
(not False) = True ;
(not True) = False ;
(not (not x)) = x ;
(not (&& x y)) = (|| (not x) (not y)) ; LastResort ;
(not (|| x y)) = (&& (not x) (not y)) ; LastResort ;
(not if u then v else w) = if u then (not v) else (not w) ; LastResort ;

sigma : (a -> Bool) -> Bool ;
(sigma \x.(|| u_SV v_SV)) = (|| (sigma \x.u_SV) (sigma \x.v_SV)) ;
(sigma \x1.(sigma \x2.(sigma \x3.(|| u_SV v_SV)) ) ) =
        (|| (sigma \x1.(sigma \x2.(sigma \x3.u_SV))) 
            (sigma \x1.(sigma \x2.(sigma \x3.v_SV)))) ;

(sigma \x.if u_SV then True else v_SV) = 
                  if \exists x.u_SV then True else \exists x.v_SV ;
(sigma \x1.(sigma \x2.(sigma \x3.if u_SV then True else v_SV))) = 
       if (sigma \x1.(sigma \x2.(sigma \x3.u_SV))) then True 
       else (sigma \x1.(sigma \x2.(sigma \x3.v_SV))) ;

(sigma \x.if u_SV then False else v_SV) = (sigma \x.(|| (not u_SV) v_SV)) ;
(sigma \x2.(sigma \x1.(sigma \x.if u_SV then False else v_SV))) = 
       (sigma \x2.(sigma \x1.(sigma \x.(|| (not u_SV) v_SV)))) ;
(sigma \x.if u_SV then v_SV else w_SV) = 
             if (sigma \x.(&& u_SV v_SV)) then True
             else (sigma \x.(&& (not u_SV) w_SV)) ; LastResort ;

-- \exists x.if u_SV then v_SV else w_SV = 
--  if \exists x.(&& u_SV v_SV) then True else \exists x.(&& (not u_SV) w_SV) ; 


pi : (a -> Bool) -> Bool ;
(pi \x.False) = False ;
(pi \x.True) = True ;
(pi \x1.(implies (|| u_SV v_SV) t_SV)) =
      (&& (pi \x1.(implies u_SV t_SV)) (pi \x1.(implies v_SV t_SV))) ;
(pi \x1.(implies if u_SV then True else v_SV t_SV)) =
      (&& (pi \x1.(implies u_SV t_SV)) (pi \x1.(implies v_SV t_SV))) ;

(pi \x.(&& (pi \x1.u_SV) (pi \x2.v_SV))) = 
      (&& (pi \x.(pi \x1.u_SV)) (pi \x.(pi \x2.v_SV))) ;

(pi \x.(implies if u_SV then True else v_SV t_SV)) =
      (pi \x.(implies (&& (not u_SV) v_SV) t_SV)) ;

-- if u then True else v = (|| u v)
-- if u then False else v = (&& (not u) v)

implies : Bool -> Bool -> Bool ;
(implies True x) = x ;    -- these are needed by queries 8 and 9 in 
(implies False x) = True ; -- the database example
-- (implies p q) = (|| (not p) q) ; LastResort ; -- this affects pi, bad.

/= : a -> a -> Bool ;
(/= x y) = (not (= x y)) ;

comp : (a -> b) -> (b -> c) -> a -> c ;
-- (comp p1 p2) = \x.(p2 (p1 x)) ;
(comp p1 p2 x) = (p2 (p1 x)) ; 

proj1 : (a * b) -> a ;
(proj1 (t1,t2)) = t1 ;

proj2 : (a * b) -> b ;
(proj2 (t1,t2)) = t2 ;

identity : a -> a ;
(identity x) = x ; LastResort ;

-- These are used by the theorem prover
TpTag : ProveStatus -> Bool -> Bool ;
DontKnow : ProveStatus ;

