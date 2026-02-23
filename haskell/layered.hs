-- Replacement logic for layered.c, we turn a list of layers like [4,4,4,4]
-- into the text that would be in the .net file. This text can be piped into
-- the training process calls without having to create a temp .net file.
module Layered (
    createFullyConnectedNetwork
) where

import Text.Printf

-- Takes the first two layer widths from the list, and a start index, and
-- returns a string for all lines for edges between the layers. Start index is
-- the index of the first input node.
createFullyConnectedLayer :: Int -> [Int] -> String
createFullyConnectedLayer start_idx layer_widths =
    let num_inputs   = head layer_widths
        num_outputs  = head (tail layer_widths)
        input_nodes  = 0 : [start_idx + n | n <- [0..num_inputs-1]]
        output_nodes = [start_idx + num_inputs + n | n <- [0..num_outputs-1]]
        edges = [(outs, ins) | outs <- output_nodes, ins <- input_nodes]
    in
       concatMap (uncurry (printf "%5d%5d    0.00000000\n")) edges

createFullyConnectedLayersIter :: Int -> [Int] -> String
createFullyConnectedLayersIter start_acc layer_widths =
    if null (tail layer_widths) then ""
    else createFullyConnectedLayer start_acc layer_widths ++ createFullyConnectedLayersIter (start_acc + head layer_widths) (tail layer_widths)

createFullyConnectedNetwork :: [Int] -> String
createFullyConnectedNetwork layer_widths =
    let layers   = length layer_widths - 1
        innodes  = 1 + head layer_widths
        outnodes = last layer_widths
        nodes    = 1 + sum layer_widths
        edges    = nodes - innodes + sum (zipWith (*) layer_widths (tail layer_widths))
    in
        show nodes ++ "  " ++ show innodes ++ "  " ++ show outnodes ++ "  "  ++ show edges ++ "\n"
        ++ createFullyConnectedLayersIter 1 layer_widths
