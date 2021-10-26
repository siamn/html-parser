main :: IO ()
main = do
    contents <- readFile "file.html"
    scan contents 0 [] ([],[0,0,0])

scan :: (Num t, Num a1, Num a2, Eq t, Eq a1, Eq a2) => [Char] -> t -> [Char] -> ([a2], [a1]) -> IO ()
scan [] open string (stack, [a,b,c])
    | open /= 0 = errorWithoutStackTrace "Missing '>' symbol in file."
    | not (null stack) = errorWithoutStackTrace "Detected unclosed tag(s) in HTML file."
    | a == 0 = errorWithoutStackTrace "HTML file does not contain any valid html tags."
    | c /= 2 = errorWithoutStackTrace "The file must contain a single body section and be surrounded by <html>...</html>."
    | otherwise = putStrLn "HTML file successfully parsed."

scan (x:xs) open string (stack, [a,b,c])
    | x == '<' = if open /= 0 
        then errorWithoutStackTrace "Consecutive '<' symbols seen."
        else scan xs 1 [] (stack, [a,b,c])
    
    | x == '>' = if open /= 1 
        then errorWithoutStackTrace "No matching '<' symbol seen for '>'."
        else scan xs 0 string (parse string stack [a,b,c])

    | x /= '>' && open == 1 = scan xs 1 (x:string) (stack, [a,b,c])
    | otherwise = scan xs open string (stack, [a,b,c])


-- Since cons is used to build the tag string, the string is in reverse order
-- Although the function could be called with the other parameter also manually written in reverse order, it would reduce readability.
-- Due to this, I have decided to use the reverse function despite a slight performance impact.
compare' :: String -> String -> Bool
compare' tag string = x == string && (length tag == num || y == ' ')
    where
    num = length string
    a = reverse tag      
    x = take num a
    y = a !! num


-- a = number of open tags detected
-- b = number of head tags detected
-- c = number of body tags detected
parse :: (Num a1, Num a2, Eq a1, Eq a2) => String -> [a2] -> [a1] -> ([a2], [a1])
parse tag [] [a,b,c]
    | compare' tag "html" = if a /= 0
        then errorWithoutStackTrace "<html> tag must be the first tag in the document and surround all other tags."
        else ([1], [1, b, c])
    
    | otherwise = errorWithoutStackTrace ("Invalid tag name detected: " ++ reverse tag)

parse tag (y:ys) [a,b,c]
    | compare' tag "/html" = if y /= 1
        then errorWithoutStackTrace "</html> in wrong position"
        else (ys, [a,b,c])

    | compare' tag "head" = if y /= 1 
        then errorWithoutStackTrace "<html> tag must directly precede <head> tag."
        else if b /= 0 
            then errorWithoutStackTrace "Multiple <head> tags detected."
            else if c /= 0 
                then errorWithoutStackTrace "<head> tag should precede <body> tag"
                else (2:(y:ys), [a+1,b+1,c])

    | compare' tag "/head" = if y /= 2 
        then errorWithoutStackTrace "</head> tag placed incorrectly."
        else (ys, [a,b,c])

    | compare' tag "body" = if y /= 1 
        then errorWithoutStackTrace "<html> tag must surround and precede <body> tag."
        else if c /= 0 
            then errorWithoutStackTrace "Multiple <body> tags detected."
            else (3:(y:ys), [a+1,b,c+1])

    | compare' tag "/body" = if y /= 3 
        then errorWithoutStackTrace "</body> tag in wrong position."
        else (ys, [a,b,c+1])

    | compare' tag "title" = if y /= 2 
        then errorWithoutStackTrace "<head> tag must surround and directly precede the <title> tag."
        else (4:(y:ys), [a+1,b,c])

    | compare' tag "/title" = if y /= 4 
        then errorWithoutStackTrace "</title> tag in wrong position."
        else (ys, [a,b,c])

    | c /= 1 = errorWithoutStackTrace "<body> tag must surround all relevant tags."

    | compare' tag "h1" = (5:(y:ys), [a+1,b,c])
    | compare' tag "/h1" = if y /= 5 
        then errorWithoutStackTrace "</h1> tag in wrong position."
        else (ys, [a,b,c])

    | compare' tag "h2" = (6:(y:ys), [a+1,b,c])
    | compare' tag "/h2" = if y /= 6 
        then errorWithoutStackTrace "</h1> tag in wrong position."
        else (ys, [a,b,c])

    | compare' tag "h3" = (7:(y:ys), [a+1,b,c])
    | compare' tag "/h3" = if y /= 7 
        then errorWithoutStackTrace "</h3> tag in wrong position."
        else (ys, [a,b,c])

    | compare' tag "p" = if y == 8 
        then errorWithoutStackTrace "<p> tag cannot be nested inside a <p> tag."
        else (8:(y:ys), [a+1,b,c])

    | compare' tag "/p" =  if y /= 8 
        then errorWithoutStackTrace "</p> tag in wrong position."
        else (ys, [a,b,c])

    | compare' tag "ul" = (9:(y:ys), [a+1,b,c])
    | compare' tag "/ul" = if y /= 9 
        then errorWithoutStackTrace "</ul> tag in wrong position."
        else (ys, [a,b,c])

    | compare' tag "li" = (10:(y:ys), [a+1,b,c])
    | compare' tag "/li" = if y /= 10 
        then errorWithoutStackTrace "</li> tag in wrong position."
        else (ys, [a,b,c])

    | compare' tag "a" = (11:(y:ys), [a+1,b,c])
    | compare' tag "/a" = if y /= 11 
        then errorWithoutStackTrace "</a> tag in wrong position."
        else (ys, [a,b,c])

    | compare' tag "div" = if y == 8 
        then errorWithoutStackTrace "<div> tag cannot be nested inside a <p> tag."
        else (12:(y:ys), [a+1,b,c])

    | compare' tag "/div" = if y /= 12 
        then errorWithoutStackTrace "</div> tag in wrong position."
        else (ys, [a,b,c])

    --ignore br and hr tags and make no change
    | compare' tag "br" || compare' tag "hr" = (y:ys, [a,b,c])
    | otherwise = errorWithoutStackTrace ("Invalid tag name detected: " ++ reverse tag)

