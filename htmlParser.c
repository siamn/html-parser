#include <stdio.h>

#define MAXLINELENGTH 400


int stack[100];
int pointer = 0;
int tagCounter = 0;
int body = 0;
int head = 0;


// compare function to compare strings up to a certain limit
// the limit has been included as a parameter instead of being automatically calculated to provide a small performance
// boost.
int compare(const char *s1, const char *s2, int lim)
{
    for (int i=0; i<lim; i++)
    {
        if (s1[i] != s2[i])
        {
            return 0;
        }
    }
    if (s1[lim] != '\0' && s1[lim] != ' ')  // allow an optional space for tag attributes
    {
        return 0;
    }
    return 1;
}

int parse(char *tag)
{
    if (compare(tag, "html", 4))
    {
        if (tagCounter != 0 && pointer != 0)
        {
            printf("<html> tag must be the first tag in the document and surround all other tags.\n");
            return 0;
        }
        stack[pointer] = 1;
        pointer++;
        tagCounter++;
        return 1;
    }
    else if (compare(tag, "/html", 5))
    {
        if (stack[pointer-1] == 1)
        {
            stack[pointer-1] = 0;
            return 1;
        }
        printf("</html> tag in wrong position\n");
        return 0;
    }
    else if (pointer <= 0)
    {
        printf("<html> tag must be the first tag in the document and surround all other tags.\n");
        return 0;
    }
    else if (compare(tag, "head", 4))
    {
        if (stack[pointer-1] != 1)
        {
            printf("<head> tag placed outside of <html> tag\n");
            return 0;
        }
        if (head != 0)
        {
            printf("Multiple <head> tags detected.\n");
            return 0;
        }
        if (body > 0)
        {
            printf("<head> tag should precede <body> tag.\n");
            return 0;
        }
        stack[pointer] = 2;
        pointer++;
        tagCounter++;
        head++;
        return 1;
    }
    else if (compare(tag, "/head", 5))
    {
        if (stack[pointer-1] == 2)
        {
            pointer--;
            return 1;
        }
        printf("</head> tag placed incorrectly.\n");
        return 0;
    }
    else if (compare(tag, "body", 4))
    {
        if (stack[pointer-1] != 1)
        {
            printf("<html> tag must surround and precede <body> tag.\n");
            return 0;
        }
        if (body != 0)
        {
            printf("Multiple <head> tags detected.\n");
        }
        stack[pointer] = 3;
        pointer++;
        tagCounter++;
        body++;
        return 1;
    }
    else if (compare(tag, "/body", 5))
    {
        if (stack[pointer-1] == 3)
        {
            pointer--;
            body++;
            return 1;
        }
        printf("</body> tag in wrong position.\n");
        return 0;
    }
    else if (compare(tag, "title", 5) == 1)
    {
        if (stack[pointer-1] != 2)
        {
            printf("<head> tag must surround and directly precede the <title> tag.\n");
            return 0;
        }
        stack[pointer] = 4;
        pointer++;
        tagCounter++;
        return 1;
    }
    else if (compare(tag, "/title", 6))
    {
        if (stack[pointer-1] == 4)
        {
            pointer--;
            return 1;
        }
        printf("</title> tag in wrong position.\n");
        return 0;
    }
    else if (body != 1)
    {
        printf("<body> tag must surround all relevant tags.\n");
        return 0;
    }
    else if (compare(tag, "h1", 2))
    {
        stack[pointer] = 5;
        pointer++;
        tagCounter++;
        return 1;
    }
    else if (compare(tag, "/h1", 3))
    {
        if (stack[pointer-1] == 5)
        {
            pointer--;
            return 1;
        }
        printf("</h1> tag in wrong position.\n");
        return 0;
    }
    else if (compare(tag, "h2", 2))
    {
        stack[pointer] = 6;
        pointer++;
        tagCounter++;
        return 1;
    }
    else if (compare(tag, "/h2",3))
    {
        if (stack[pointer-1] == 6)
        {
            pointer--;
            return 1;
        }
        printf("</h2> tag in wrong position.\n");
        return 0;
    }
    else if (compare(tag, "h3", 2))
    {
        stack[pointer] = 7;
        pointer++;
        tagCounter++;
        return 1;
    }
    else if (compare(tag, "/h3", 3))
    {
        if (stack[pointer-1] == 7)
        {
            pointer--;
            return 1;
        }
        printf("</h3> tag in wrong position.\n");
        return 0;
    }
    else if (compare(tag, "p", 1))
    {
        if (stack[pointer-1] == 8)
        {
            printf("<p> tag cannot be nested inside a <p> tag.\n");
            return 0;
        }
        stack[pointer] = 8;
        pointer++;
        tagCounter++;
        return 1;
    }
    else if (compare(tag, "/p", 2))
    {
        if (stack[pointer-1] == 8)
        {
            pointer--;
            return 1;
        }
        printf("</p> tag in wrong position.\n");
        return 0;
    }
    else if (compare(tag, "ul", 2))
    {
        stack[pointer] = 9;
        pointer++;
        tagCounter++;
        return 1;
    }
    else if (compare(tag, "/ul", 3))
    {
        if (stack[pointer-1] == 9)
        {
            pointer--;
            return 1;
        }
        printf("</ul> tag in wrong position.\n");
        return 0;
    }
    else if (compare(tag, "li", 2))
    {
        stack[pointer] = 10;
        pointer++;
        tagCounter++;
        return 1;
    }
    else if (compare(tag, "/li", 3))
    {
        if (stack[pointer-1] == 10)
        {
            pointer--;
            return 1;
        }
        printf("</li> tag in wrong position.\n");
        return 0;
    }
    else if (compare(tag, "a", 1))
    {
        stack[pointer] = 11;
        pointer++;
        tagCounter++;
        return 1;
    }
    else if (compare(tag, "/a", 2))
    {
        if (stack[pointer-1] == 11)
        {
            pointer--;
            return 1;
        }
        printf("</a> tag in wrong position.\n");
        return 0;
    }
    else if (compare(tag, "div", 3))
    {
        if (stack[pointer-1] == 8)
        {
            printf("<div> tag cannot be nested inside a <p> tag.\n");
            return 0;
        }
        stack[pointer] = 12;
        pointer++;
        tagCounter++;
        return 1;
    }
    else if (compare(tag, "/div", 4))
    {
        if (stack[pointer-1] == 12)
        {
            pointer--;
            return 1;
        }
        printf("</div> tag in wrong position.\n");
        return 0;
    }
    else if (compare(tag, "br", 2) || compare(tag, "hr", 2))
    {
        return 1;
    }
    else
    {
        printf("Invalid tag name detected: %s\n", tag);
        return 0;
    }

}

int checkValidity(int opened)
{
    if (opened)
    {
        printf("Missing '>' symbol in file.\n");
        return 0;
    }
    if (pointer != 1)
    {
        printf("Detected unclosed tag(s) in HTML file.\n");
        return 0;
    }
    if (tagCounter == 0)
    {
        printf("HTML file does not contain any valid html tags.\n");
        return 0;
    }
    if (body != 2)
    {
        printf("The file must contain a single body section and be surrounded by <html>...</html>.\n");
        return 0;
    }
    return 1;
}

int getInputText()
{
    char tag[MAXLINELENGTH];
    int i = 0;
    FILE *inputFile = fopen("file.html", "r");
    int opened = 0;
    while (!feof(inputFile))
    {
        char c = (char) fgetc(inputFile);
        if (c == '<')
        {
            if (!opened)
            {
                opened = 1;
            }
            else
            {
                printf("Consecutive '<' symbols seen.\n");
                return 0;
            }
        }
        else if (c == '>')
        {
            if (opened)
            {
                opened = 0;
                tag[i] = '\0'; // add null byte char to denote the end of the string
                int valid = parse(tag);
                if (!valid) { return 0;}
                i = 0;
            }
            else
            {
                printf("No matching '<' symbol seen for '>'.\n");
                return 0;
            }
        }
        else if (opened)
        {
            tag[i] = c;
            i++;
        }
    }
    fclose(inputFile);
    return checkValidity(opened);
}

int main() {
    if (getInputText())
    {
        printf("HTML file successfully parsed.\n");
    }
}
