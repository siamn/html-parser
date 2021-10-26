# HTML Parser
## Details
This repository contains a HTML parser written completely in C and another written completely in Haskell.

The program has been designed to parse only the `file.html` file. Therefore, all HTML content that needs to be parsed must be placed inside this file and the file must be present in the same directory as the program.

The parsers are designed to behave in the same way and are limited to a custom HTML specification which includes the following HTML tags: `html, head, body, title, h1, h2, h3, p, ul, li, a, div, br, hr`

## Instructions
### C
To compile the C program, you can use the command:
  * `gcc -o htmlParser htmlParser.c`

To run the C program, you can use the command:
  * `./htmlParser`
### Haskell
To compile and run the Haskell program:
  * `runhaskell htmlParser.hs`
