#include <stdio.h>

#define INTEGER_TYPE 1
#define OPERATOR_TYPE 2
#define EOL_TYPE 3

int ptr = 0;

typedef struct
{
    int integer;
    char op;
    int type;
} Token;

char* remove_spaces(char* line, char* no_spaces)
{
    int count = 0;

    for(int i = 0; line[i] != '\0'; i++)
    {
        if(line[i] != ' ')
        {
            no_spaces[count] = line[i];
            count++;
        }
    }
    no_spaces[count] = '\0'; // gotta put this back in to end it

    printf("the original word is: '%s'\n"
           "and the word without spaces is: %s\n"
            , line, no_spaces);

    return no_spaces;
}//end of remove_spaces

void gettoken(Token *token, char* line) {
    if (line[ptr] == '\0')
        token->type = EOL_TYPE;

        //can use ASCII or char type
    else if (line[ptr] >= 48 && line[ptr] <= '9')
    {
        token->type = INTEGER_TYPE;
        token->integer = line[ptr] - '0';
    }
    else
    {
        token->type = OPERATOR_TYPE;
        token->op = line[ptr];
    }
}//end of gettoken

void advance() {
    ptr++;
}//end of advance

int getpow(int leftterm, int rightterm)
{
    int value = leftterm;
    for (int i = 0; i < rightterm - 1; i++)
    {
        value *= leftterm;
    }

    return value;
}

int power(char* line, int firstvalue)
{

    // move past ^
    advance();

    // pointer should be at an integer
    // get a number
    Token firsttoken;
    gettoken(&firsttoken, line);

    // move past number
    advance();
    Token nexttoken;
    gettoken(&nexttoken, line);

    printf("type: %d\n", nexttoken.type);

    if (nexttoken.type == OPERATOR_TYPE && nexttoken.op == '^')
    {
        // recursive case
        int secondvalue = power(line, firstvalue);
        return getpow(firstvalue, secondvalue);

    }
    else
    {
        printf("---%d\n", firsttoken.integer);
        return firsttoken.integer;
    }
}

int factor(char* line)
{

    // firsttoken could be either an integer, (, or ^
    Token firsttoken;
    gettoken(&firsttoken, line);

    advance();
    Token nexttoken;
    gettoken(&nexttoken, line);

    if (firsttoken.type == OPERATOR_TYPE && firsttoken.op == '(')
    {
        //recursive case
        //found open parenthesis - move forward
//        advance();
        int value = expression(line);   //parenthesis opens a new expression until close )
        advance(); // move past the close )
        return value;
    }
    else if (nexttoken.type == OPERATOR_TYPE && nexttoken.op == '^')
    {
        int powervalue = power(line, firsttoken.integer);    //firsttoken is left value ^

        return getpow(firsttoken.integer, powervalue);
    }
    else    //base case, number is returned
    {

        // build the whole number
        while (nexttoken.type == INTEGER_TYPE)
        {
            gettoken(&nexttoken, line);     // get second digit
            advance();                      // move past
            int seconddigit = nexttoken.integer;
            gettoken(&nexttoken, line);
            firsttoken.integer = firsttoken.integer * 10 + seconddigit;
            printf("ptr: %d\n", firsttoken.integer);
        } // by this point, ptr should be pointing an operator

        return firsttoken.integer;
    }
}//end of factor

int term(char* line)
{
    int firstvalue = factor(line);

    while(1)
    {
        //see what the next token is without advancing
        Token optoken;
        gettoken(&optoken, line);
//        printf("%c\n", optoken.op);

        if (optoken.type == OPERATOR_TYPE && (optoken.op == '*' || optoken.op == '/'))
        {
            //found an operator so move forward and get next value
            advance();
            int secondvalue = factor(line);
            printf("%d *// %d\n", firstvalue, secondvalue);
            if(optoken.op == '*')
                firstvalue = firstvalue * secondvalue;
            else
                firstvalue = firstvalue / secondvalue;
        }
        else
            break;
    }
    return firstvalue;
}// end of term

int expression(char* line)
{
    //get first value
    int firstvalue = term(line);

    while(1)
    {
        //get the next operator
        Token optoken;
        gettoken(&optoken, line);

        //base case: if end of line or end of parenthesis
        if(optoken.type==EOL_TYPE || (optoken.type==OPERATOR_TYPE && optoken.op==')'))
            break;

        // move past operator - got either + or -
        advance();
        int secondvalue = term(line);
        printf("%d +/- %d\n", firstvalue, secondvalue);
        if(optoken.op == '+')
            firstvalue = firstvalue + secondvalue;
        else
            firstvalue = firstvalue - secondvalue;

    }
    return firstvalue;
}

void parse(char* line)
{
    char processed_line [100];
    remove_spaces(line, processed_line);
    int answer = expression(processed_line);
    printf("the answer is: %d\n", answer);
}

int main(int argc, char* argv[])
{
    char* line1 = "51   +3 -   1004+16  "; // -934
    char* line2 = "5 + 2 * 3 + 1 * 3 + 8"; // 22
    char* line3 = "  5*   2 *6  -5 4  * 2 - 4";    // 29    12 spaces
    char* line4 = "  5  - 3  * 2 + 4 * 2 -   4"; // 3
    char* line5 = "51   +3 -   1004+16  ";// -934
    char* line6 = "5   +3 -    4 + 6 - 2 - 3 + 8"; // 13
    char* line7 = "1012 + 20 + 300"; // 1332
    char* line8 = "101.2446+6.4  ";
    char* line9 = "5*(1+1)+((1+2)+(3*2))*2";
    char* line10 = "5+4-2+8"; // 15
    char* line11 = "(4- 2) +3 +  9 / 3"; // 8
    char* line12 = "2^10"; // 1024
    char* line13 = "2^3+(4+3)*2"; //22
    char* line14 = "2^2^3"; // 256
    char* line15 = "242 - (18-7) + 2 * 12 - 300"; // -45
    char* line16 = "2^(2+1)"; // 8

    parse("20^2");

    return 0;
}