# Purr Data
**(Sound design + music)**

_Purr Data is integrated with the chessboard. 
So no external computer is needed._

### What it is able to:
  - Link sound to moves/chess pieces
  - Judge moves by sound
  - Produce music based on the state of the game

### It's goal:
The ultimate goal is to help people practice chess in a fun way. 
Due to sound and music they can easily and quickly sense when they make a good move, or a less good one, 
without having to look at a screen. 

Overview
To give a clear overview we have devided the sound design part of this project in three 

### Moves:
To give the player an indication about wether a move is good or bad we would first have to dwell on the idea what sounds
'good' or 'bad' or even 'neutral'. For this we have done some simple research about what is often as perceived as a pleasant or an unpleasant sound.

We discovered that, which is probably common knowledge, minor intervals are perceived as 'unpleasant' and major intervals as 'uplifting'
and 'happy'. So for the neutral move we would use a single rootnote which on itself doesn't say a lot, it's neutral. 
For good: major, bad: minor.

The sound itself has been made with a simple self-build FM synth. The intention was to make a simple but affirming sound.
One that doesn't require too much attention from the player and by it's simpleness stands out from the music. Thereby making it
recognizable as an indicator. 

### Chess Pieces
We had to keep in mind that these PD patches should be able to run on the Pi.
Because of this we chose to keep them relatively 'simple'. For every piece-category (rook, king etc.) we made a simple synth that we would later manipulate by filters to make them fit our aesthetic. 
These synths vary between synthesis methods.

The sound design choices for the pieces weren't thought out very thoroughly, but arose mostly from 
gut-instinct and practical limitations. For example we thought the rook should definitely be a bass
because a tower would weight a ton and bass is usually as perceived as heavy and deep. Similar ideas arose with the other pieces.

### Music
The chessboard is perceived as a music grid that Purr Data analysis and produces music from. The chessboard consists of eight vertical lines consisting of eight squares (8 by 8 grid) that contain their own information. The information per line consists of pitch and the placed chess pieces with their own sound. The pitch of the chess pieces is influenced by the square where this piece is placed. The pitch drops from c to c an octave lower (the top square to the bottom square are exactly eight squares). Purr Data descends these eight lines in steps, creating music. This results in a harmony being generated per step with the placed pieces aligned.

Aesthetically, we went for a somewhat floaty and relaxed atmosphere and pace that should not distract the players but should promote concentration. The harmonies created from the sounds are not obvious which will make the players want to participate more in the game to discover new harmonies and sounds.
