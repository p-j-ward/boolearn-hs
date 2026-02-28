
func :: Double -> Double -> Double -> Double -> Double
func pns qns cns u = (pns*(1+u^2) + qns*u)/(1-u^2)^2 - cns

dFunc :: Double -> Double -> Double -> Double
dFunc pns qns u = (2*pns*u + qns)/(1-u^2)^2 + 4*u*(pns*(1.0+u^2) + qns*u)/(1-u^2)^3

marginProj :: Double -> Double -> ([Double], [Double]) -> ([Double], [Double])
marginProj cns w_dot_x (wn, xn) = 
    let pns = w_dot_x --sum $ zipWith (*) wn xn
        qns = sum $ zipWith (+) (map (^2) wn) (map (^2) xn)
    in
        ([0.0],[0.0])


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