
func :: Double -> Double -> Double -> Double -> Double
func pns qns cns u = (pns*(1+u^2) + qns*u)/(1-u^2)^2 - cns

dFunc :: Double -> Double -> Double -> Double
dFunc pns qns u = (2*pns*u + qns)/(1-u^2)^2 + 4*u*(pns*(1.0+u^2) + qns*u)/(1-u^2)^3

marginProjIter :: (Double, Double, Double, Double) -> Int -> (Double, Double, Double) -> (Double, Double, Double)
marginProjIter (pns, qns, cns, stop) count (ua, ub, f) =
    let df = dFunc pns qns ua
        u = ua - f/df
        ua' = if (ub > ua && u > ub) || (ub <= ua && u < ub) then 0.5*(ua+ub) else u
        f' = func pns qns cns ua'
        ub' = if (ub>ua && f'>0) || (ub<=ua && f'<0) then ua else ub
    in
        if abs (ua' - ua) < stop || count == 0 then
            (ua', ub', f')
        else
            marginProjIter (pns, qns, cns, stop) (count-1) (ua', ub', f')

marginProj :: Double -> Double -> ([Double], [Double]) -> (Double, [Double], [Double])
marginProj cns w_dot_x (wn, xn) =
    let pns = w_dot_x
        qns = sum $ zipWith (+) (map (^2) wn) (map (^2) xn)
        f_init = func pns qns cns 0
        (u, _, _) = marginProjIter (pns, qns, cns, 1e-12) 50 (0, -(signum f_init), f_init)
        -- update wn, xn with new u
        den = 1 - u^2
        dw = zipWith (\wn xn -> (wn + u*xn)/den - wn) wn xn
        dx = zipWith (\wn xn -> (xn + u*wn)/den - xn) wn xn
        dist = sum $ zipWith (\dw dx -> dw^2 + dx^2) dw dx
        wn' = zipWith (+) wn dw
        xn' = zipWith (+) xn dx
    in
        (dist, wn', xn')


neuronProj :: Double -> Double -> ([Double], [Double], Double) -> ([Double], [Double], Double)
neuronProj sigma deg (wn, xn, yn) =
    let mu_m = sqrt (sigma * deg)
        w_dot_x = sum $ zipWith (*) wn xn
    in
        if yn * w_dot_x >= 0 then
            let (wn', xn') = if abs w_dot_x < mu_m then marginProj (mu_m * signum yn) w_dot_x (wn, xn) else (wn, xn)
            in (wn', xn', signum yn)
        else
            ([0.0], [0.0], 0.0) -- TODO