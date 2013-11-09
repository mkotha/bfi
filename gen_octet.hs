import Data.Word
import Text.Printf
import Data.List

main = mapM_ putStrLn $ concat $ intersperse [""] $
  [ prolog
  , [ "# define BFI_ADD(x, y) BFI_PCAT(BFI_ADD_, x)(y)"
    , "# define BFI_MUL(x, y) BFI_PCAT(BFI_MUL_, x)(y)"]
  , forAll addTable
  , forAll add10Table
  , forAll mulTable
  , forAll mul10
  , forAll add1_t
  , forAll mul1
  , map chooser [0..0xf]
  ]
  -- ++ map (\n -> forAll (add n)) ([-8.. -1] ++ [1..7]++[16, 32..240])
  ++ map (\n -> forAll (add n)) [-1,1]
  ++ [epilog]

prolog =
  [ "# ifndef BFI_OCTET_H"
  , "# define BFI_OCTET_H"
  ]

epilog = ["# endif"]

inc n = def "INC" [n] (n + 1)
dec n = def "DEC" [n] (n - 1)
add m n = def "ADD" [m, n] (m + n)

addTable :: Word8 -> String
addTable m = table (pname "ADD_TABLE" [m]) (m-8+)

add10Table :: Word8 -> String
add10Table m = table (pname "ADD10_TABLE" [m]) ((+m) . (*0x10))

mulTable m = table (pname "MUL_TABLE" [m]) (*m)

mul10 m = "# define " ++ pname "MUL10" [m] ++ " " ++ pr (0x10*m)

table :: String -> (Word8 -> Word8) -> String
table name c = "# define " ++ name ++ "(c) c(" ++ tbl ++ ")"
  where
    tbl = sep "," $ map (pr . c) [0..0xf]

chooser m = "# define " ++ pname "LOOKUP" [m] ++ "(" ++ sep "," params ++ ") " ++ params !! fromIntegral m
  where
    params = map (:[]) ['a'..'p']

mul1 m = "# define " ++ pname "MUL" [m] ++ "(x) " ++ plus (mul10 (mul hi "x")) (mul lo "x")
  where
    mul c t = case c of
      0 -> "0x00"
      1 -> t
      _ -> call ("BFI_MUL_TABLE_##" ++ t) [pname "LOOKUP" [c]]
    mul10 x 
      | x == "0x00" = "0x00"
      | otherwise = prefix "BFI_MUL10_" x
    plus x y
      | x == "0x00" = y
      | y == "0x00" = x
      | otherwise = call "BFI_ADD" [x, y]
    (hi, lo) = divMod m 16

add1_t :: Word8 -> String
add1_t m = "# define " ++ pname "ADD" [m] ++ "(x) " ++ addHi (addLo "x")
  where
    addLo t
      | lo == 0 = t
      | otherwise = call ("BFI_ADD_TABLE_##" ++ t) [pname "LOOKUP" [lo + 8]]
    addHi t
      | hi == 0 = t
      | otherwise = call (prefix "BFI_ADD10_TABLE_" t) [pname "LOOKUP" [div hi 16]]
    (lo, hi) = (m - hi, (m+8) `div` 16 * 16)

add1 m = "# define " ++ pname "ADD" [m] ++ "(x) " ++ addHi (addLo "x")
  where
    addLo t
      | lo == 0 = t
      | otherwise = pname "ADD" [lo] ++ "_##" ++ t
    addHi t
      | hi == 0 = t
      | t == "x" = pname "ADD" [hi] ++ "_##x"
      | otherwise = call "BFI_CAT" [pname "ADD" [hi] ++ "_", t]
    (lo, hi) = (m - hi, (m + 8) `div` 16 * 16)

prefix :: String -> String -> String
prefix a b
  | length b == 1 = a ++ "##" ++ b
  | otherwise = call "BFI_CAT" [a, b]

sep :: String -> [String] -> String
sep c = concat . intersperse c

call :: String -> [String] -> String
call f args = f ++ "(" ++ sep ", " args ++ ")"

pname :: String -> [Word8] -> String
pname prefix params = "BFI_" ++ prefix ++ concat (map (("_"++) . pr) params)

def :: String -> [Word8] -> Word8 -> String
def name params ans = "# define " ++ pname name params ++ " " ++ pr ans

pr :: Word8 -> String
pr n = printf "0x%02x" n

forAll f = map f [0 .. 0xff]
