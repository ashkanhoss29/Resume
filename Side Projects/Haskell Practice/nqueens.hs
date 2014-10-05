-- Name: Ashkan Hosseini
-- Date: 10/01/2014

-- N Queens solver: this program will find one solution to the nqueens problem.

-- Return False if Q(x, y) conflicts with any Q(xl, yl) in the list
checkQ :: Int -> Int -> Int -> Int -> [Int] -> Bool
checkQ y p m n [] = True
checkQ y p m n (yl:xs) =
	let
		is = (y /= yl) && (p /= n + yl) && (m /= n - yl)
	in is && checkQ y p m (n-1) xs

-- Find y values associated with x values (indices) s.t. we get a solution
nqueens :: Int -> Int -> [Int] -> [Int]
nqueens y n l
	| y > n = l
	| not (checkQ y (x+y) (x-y) len l) = xd
	| x == n = y:l
	| length xs - 1 == len = xd
	| otherwise = xs
	where 
		len = length l
		x  = len + 1
		xd = nqueens (y + 1) n l
		xs = nqueens 1 n (y:l)
