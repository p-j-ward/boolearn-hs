-- Wrapper for training calls, we also generate the intermediate .net file
-- using layered.hs based on the network widths list given as argument.
-- Currently this is just a wrapper for calling train, also extracting the
-- number of data items from the data file. Thus usage is e.g.:
--   ./train-hs "[4,4,4,4]" 2x2.dat 3. .2 1e-3 10 1e4 .01 5 2x2a
import System.Environment
import System.Process
import Data.Maybe
import Layered


genExecStr :: String -> String -> String -> String -> String -> String -> String -> String -> String -> String -> String -> String -> String
genExecStr netStr trainPath datPath numData sigma beta gamma numCheckpoints maxIters stopGap numRuns outIdent =
    --unwords ["echo \""++netStr++"\" | ", trainPath, "<(cat)", datPath, numData, sigma, beta, gamma, numCheckpoints, maxIters, stopGap, numRuns, outIdent]
    unwords ["echo '"++netStr++"' |", trainPath, "-", datPath, numData, sigma, beta, gamma, numCheckpoints, maxIters, stopGap, numRuns, outIdent]

main = do
    -- read path of train executable from environment variable
    maybeTrainPath <- lookupEnv "BOOLEARN_TRAIN_PATH"
    let trainPath = fromJust maybeTrainPath -- TODO: error handling

    args <- getArgs
    let widths = read (head args)
    let datPath = args !! 1
    let (sigma:beta:gamma:numCheckpoints:maxIters:stopGap:numRuns:outIdent:rest) = tail (tail args)

    -- create fully connected network, which is echo'd into command in place of .net
    let netStr = createFullyConnectedNetwork widths

    -- get number of training data from first line of dat file
    datStr <- readFile datPath
    let numData = head (lines datStr)

    -- generate the command to call
    let execStr = genExecStr netStr trainPath datPath numData sigma beta gamma numCheckpoints maxIters stopGap numRuns outIdent
    
    putStrLn "Running..."
    print execStr
    callCommand execStr
    putStrLn "Done."
