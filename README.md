A terribly made C++ program that uses SFML library. At first when I started programming, I've always wanted a 
"How to make Chess" tutorial. Unfortunately, Youtube got me nowhere. The closest I had was from Sebastian Lague.
But what he made was a Chess AI, though, of course he walked everyone through how he made the chess game. But
I'm not content

# Credits
- Massively stolen code from [Sebastian Lague](https://github.com/SebLague/Chess-AI)
- Sounds and sprites taken from [lichess](https://lichess.org)

~~It's actually called plagiarize~~

# Features

- Normal chess stuff
- Epic gamer code that can make your eyes explode (I hope not :)), feel free to suggest your epic improvements 
in issues, I'm open to hear them!)
- Idk

# Why not use SDL

SDL = headache = big nono

# Dependencies

* g++
* SFML library
* cmake
* make

# How to compile

Simply run the following command (assuming you're on linux cuz fuck Microsoft)

```sh
cd build
cmake .. && make
```

# Running

The texture will fail to load if you run it in the build directory, sorry :<. A way around
that is to run it in the project directory instead

```sh
./build/cpp-chess
```

# Change logs

## Wednesday, 1st of December, 2021
- Added core mechanics

## Saturday, 15th of January, 2022 
- Code cleanup
- Fen support!
- Fix being able to thanos snap pieces by right clicking them
- Fix this dumb README.md

## Tuesday, 25th of January, 2022
- Fixed this dumb README.md again
- Legal moves ! (But only for Rooks, Bishops, Queen, King)
- Code is messed up again xd
- Uh

## Wednesday, 31st of August, 2022
- I can't seem to write a proper README.md file
- Complete development reset! It's been a while and I did not use that amount of time to waste ;)

## Saturday, 3rd of September, 2022
- Fixed this README.md again (BRUH)
- Added drag and drop (very cool)
- School is coming back so I'm ded
- Yes

## Monday, 5th of September, 2022
- School has came back and I'm ded inside
- Fixed this dumb README.md again (omg)
- Added legal moves for Rooks, Bishops and Queens ;)

## Tuesday, 6th of September, 2022
- First day of school was alright
- I didn't have to fix README.md again woo
- Added legal moves for Knights ;)
- Added highlights for squares that the pieces move to
- Some other code improvements

## Wednesday, 7th of September, 2022 
- I lied, I had to fix README.md the SECOND I PUSH IT TO GIITAISADIOHDSPINDA
- Some fixes regarding includes since coc-nvim was being stupid and I didn't realize it
- Some comments cleanup
- Added legal moves for Kings!
- Added epic finite state machine for rendering and updating in preparation for legal moves for pawns
- Fuc

## Friday, 23th of September, 2022
- School sucks

> I even got the hottest new moves from the 15th century like double pawn pushes... and en passant

- Code cleanup and stuff teeheehehee (as per usual)
- Why am I always fking tired

## Saturday, 24th of September, 2022
- I realized I totally butchered the square colors lmfao
- I also realized pawns can captures pieces of their same color, and I need to fix it to prevent pieces from having cannibalism (That'd be seriously bad)
- I also found out that en passant is completely broken due to the way I reset the en passant file data (Why is the pawn the most broken-ass piece in this entire game)
- Added castle moves! ;)
- Some clean up as usual (not much though xd)
- Yes

# Todos
- Stop being an idiot
- Fix this README.md because I know the second I commit and push it to Github I will find a typo
- h
