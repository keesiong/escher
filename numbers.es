Infinity : a ;
NegInfinity : a ;

(add x_SV/CONST/ Infinity) = Infinity ;
(add Infinity x_SV/CONST/) = Infinity ;
(sub x_SV/CONST/ Infinity) = NegInfinity ;
(sub Infinity x_SV/CONST/) = Infinity ;

(min x_SV/CONST/ Infinity) = x_SV ;
(min Infinity x_SV/CONST/) = x_SV ;

(add x 0.0) = x;
(add 0.0 x) = x ;
(mul x 0.0) = 0.0 ;
(mul 0.0 x) = 0.0 ;

(div 0 x) = 0 ;
(div 0.0 x) = 0.0 ;
(mul 1 x) = x ;
(mul 1.0 x) = x ;

Power : (number1 * Int) -> number2 ;
power : (number1 * Int) -> number3 ;
(power (1,n)) = 1;
(power (m_SV/CONST/,0)) = 1 ;
(power (m_SV/CONST/,1)) = m_SV ;
(power (m_SV/CONST/,n_SV/CONST/)) = if (&& (<= m_SV 16) (< n_SV 16)) then
                                        (power2 (m_SV,n_SV)) 
                                    else (Power (m_SV,n_SV)) ;

power2 : (number1 * Int) -> number2 ;
(power2 (m_SV/CONST/,n_SV/CONST/)) = (mul m_SV (power (m_SV,(sub n_SV 1)))) ;

integer : number1 -> number2 -> number3 ;
(integer x y) = (sub (div x y) (remainder x y)) ;

remainder : number1 -> number2 -> number3 ;
(remainder x y) = (div (mod x y) y) ;

monus : number1 -> number2 -> number3 ;
(monus x_SV/CONST/ y_SV/CONST/) = (max 0 (sub x_SV y_SV)) ;

-- this produces a loop
-- (> (add u_SV/CONST/ (card v)) v_SV/CONST/) =
--      if (> u_SV v_SV) then True else (> (add u_SV (card v)) v_SV) ;

-- > : number -> number -> Bool ;
-- (> if u then v_SV/CONST/ else w_SV/CONST/ x_SV/CONST/) = 
--                       if u then (> v_SV x_SV) else (> w_SV x_SV) ;

-- >= : number -> number -> Bool ;
-- (>= if u then v_SV/CONST/ else w_SV/CONST/ x_SV/CONST/) = 
--                        if u then (>= v_SV x_SV) else (>= w_SV x_SV) ;

-- < : number -> number -> Bool ;
-- (< if u then v_SV/CONST/ else w_SV/CONST/ x_SV/CONST/) = 
--                        if u then (< v_SV x_SV) else (< w_SV x_SV) ;

-- <= : number -> number -> Bool ;
-- (<= if u then v_SV/CONST/ else w_SV/CONST/ x_SV/CONST/) = 
--                        if u then (<= v_SV x_SV) else (<= w_SV x_SV) ;

-- (< x_SV/CONST/ Infinity) = 
--             if (/= x_SV Infinity) then True else (< x_SV Infinity) ;

(< Infinity x_SV/CONST/) = False ;
(< x_SV/CONST/ Infinity) = True ;

abs : number -> number ;
(abs x_SV/CONST/) = if (>= x_SV 0) then x_SV else (add x_SV (mul -2 x_SV)) ;

fabs : number -> number ;
(fabs x_SV/CONST/) = if (>= x_SV 0.0) then x_SV else (add x_SV (mul -2.0 x_SV));

mChooseN : Int -> Int -> Int ;
(mChooseN m_SV/CONST/ 0) = 1 ;
(mChooseN m_SV/CONST/ n_SV/CONST/) = 
                      (div (facl m_SV (sub m_SV n_SV)) (fac n_SV)) ; 

facl : Int -> Int -> Int ;
(facl m_SV/CONST/ n_SV/CONST/) = if (> m_SV n_SV)
                                 then (mul m_SV (facl (sub m_SV 1) n_SV))
                                 else 1 ;

fac : Int -> Int ;
(fac 0) = 1 ;
(fac 1) = 1 ;
(fac m_SV/CONST/) = (mul m_SV (fac (sub m_SV 1))) ;
