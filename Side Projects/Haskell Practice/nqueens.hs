-- N Queens solvers: this program will find one solution to the nqueens problem.
-- This is my first Haskell program from scratch. IT IS NOT GOOD! Many improvements can be made.
-- Written for learning/practicing purposes only!

-- Check if Q(x, y) conflicts with any Q(xl, yl) in the list
checkM x y [] = True
checkM x y (yl:xs) =
	let
		xl = length xs + 1
		is = (x /= xl) && (y /= yl) && (x + y /= xl + yl) && (x - y /= xl - yl)
	in is && checkM x y (xs)

-- Find y values associated with x values (indices) s.t. we get a solution
findy y n l =
	if y > n
		then 
			l
		else
			if checkM (length l + 1) y l
				then 
					if length l + 1 == n
						then
							y:l
						else
							let
								xs = findy 1 n (y:l)
							in if ((length xs) - 1) == length l
								then
									findy (y+1) n l
								else
									xs
				else
					findy (y + 1) n l