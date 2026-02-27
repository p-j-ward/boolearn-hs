-- Wrapper for training calls, we also generate the intermediate .net file
-- using layered.hs based on the network widths list given as argument.
-- Currently this is just a wrapper for calling train, also extracting the
-- number of data items from the data file. Thus usage is e.g.:
--   ./train-hs "[4,4,4,4]" 2x2.dat 3. .2 1e-3 10 1e4 .01 5 2x2a
import System.Environment
import System.Process
import System.IO
import Data.Maybe
import Layered

main = do
    -- read path of train executable from environment variable
    maybeTrainPath <- lookupEnv "BOOLEARN_TRAIN_PATH"
    let trainPath = fromJust maybeTrainPath -- TODO: error handling

    args <- getArgs
    let widths = read (head args)
    let datPath = args !! 1
    let restArgs = tail (tail args)

    -- create fully connected network, which is echo'd into command in place of netfile
    let netStr = createFullyConnectedNetwork widths

    -- get number of training data from first line of dat file
    datStr <- readFile datPath
    let numData = head (lines datStr)

    -- call train command, passing netfile string through stdin
    (Just hin, _, _, ph) <- createProcess (proc trainPath (["-", datPath, numData]++restArgs)) { std_in = CreatePipe }
    hPutStr hin netStr
    hClose hin
    waitForProcess ph
    return ()
