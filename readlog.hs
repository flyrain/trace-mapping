module Main where
import qualified Data.ByteString.Char8 as C

printList xs = mapM_ C.putStrLn xs

getPC line =  (C.splitWith (== ('\t')) line) !! 1

cmpPC (x:xs) (y:ys) n
  | getPC x  == (getPC y) = cmpPC xs ys (n+1)
  | otherwise = n

str base offset = show base ++ " to " ++ (show  (base + offset))

readTextFile = do
  file <- C.readFile "no-red.source"
  let lines = C.splitWith (== ('\n')) file
  let base = 11462
  let rest = drop base lines
  redFile <- C.readFile "red.source"
  let redBase = 5047
  let redRest = drop redBase ( C.splitWith (== ('\n')) redFile)

  let offset = cmpPC rest redRest 0
  print $ str base offset
  print $ str redBase offset
  
--  let movs = [ b | b <- lines, C.isInfixOf (C.pack "pop    %ebp") b]
--  printList movs

main = do
  readTextFile
