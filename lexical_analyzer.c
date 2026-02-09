#include <stdio.h>
#include <ctype.h>
#include <string.h>

char keywords[32][10] = {
    "auto","break","case","char","const","continue","default","do","double",
    "else","enum","extern","float","for","goto","if","int","long","register",
    "return","short","signed","sizeof","static","struct","switch","typedef",
    "union","unsigned","void","volatile","while"
};

char stdFunctions[][10] = {"printf", "scanf", "main", "gets", "puts"};

char operators[] = "+-*/%=<>&|!^";
char delimiters[] = "(),;{}[]\"\'#<>";



int isKeyword(char *buffer) {
    for (int i = 0; i < 32; i++) {
        if (strcmp(keywords[i], buffer) == 0)
            return 1;
    }
    return 0;
}

int isStdFunction(char *buffer) {
    for (int i = 0; i < 5; i++) {
        if (strcmp(stdFunctions[i], buffer) == 0)
            return 1;
    }
    return 0;
}

int isOperator(char ch) {
    for (int i = 0; i < strlen(operators); i++)
        if (ch == operators[i])
            return 1;
    return 0;
}

int isDelimiter(char ch) {
    for (int i = 0; i < strlen(delimiters); i++)
        if (ch == delimiters[i])
            return 1;
    return 0;
}

void printToken(char *type, char *value) {
    printf("<%s, %s>\n", type, value);
}



int main() {
    FILE *fp;
    char ch, buffer[100];
    int i = 0;

    fp = fopen("input.c", "r");   
    if (!fp) {
        printf("Error: Cannot open file!\n");
        return 0;
    }

    while ((ch = fgetc(fp)) != EOF) {

       
        if (ch == '"') {
            i = 0;
            buffer[i++] = ch;
            while ((ch = fgetc(fp)) != EOF && ch != '"') {
                buffer[i++] = ch;
            }
            buffer[i++] = '"';
            buffer[i] = '\0';
            printToken("String Literal", buffer);
            continue;
        }

      
        if (ch == '\'') {
            i = 0;
            buffer[i++] = ch;
            while ((ch = fgetc(fp)) != EOF && ch != '\'') {
                buffer[i++] = ch;
            }
            buffer[i++] = '\'';
            buffer[i] = '\0';
            printToken("Character Literal", buffer);
            continue;
        }

        
        if (ch == '#') {
            i = 0;
            buffer[i++] = ch;
            while ((ch = fgetc(fp)) != EOF && ch != '\n') {
                buffer[i++] = ch;
            }
            buffer[i] = '\0';
            printToken("Preprocessor", buffer);
            continue;
        }

        
        if (ch == '/') {
            char next = fgetc(fp);
            if (next == '/') { 
                while ((ch = fgetc(fp)) != EOF && ch != '\n');
                continue;
            } else if (next == '*') { 
                while ((ch = fgetc(fp)) != EOF) {
                    if (ch == '*' && (ch = fgetc(fp)) == '/')
                        break;
                }
                continue;
            } else {
                ungetc(next, fp); 
            }
        }

        
        if (isalnum(ch) || ch == '_') {
            i = 0;
            while (isalnum(ch) || ch == '_') {
                buffer[i++] = ch;
                ch = fgetc(fp);
            }
            buffer[i] = '\0';
            ungetc(ch, fp);

            if (isKeyword(buffer))
    printToken("Keyword", buffer);
else if (isStdFunction(buffer))
    printToken("Standard Function", buffer);

// FIRST char rules for identifiers and numbers
else if (isalpha(buffer[0]) || buffer[0] == '_')
    printToken("Identifier", buffer);

else if (isdigit(buffer[0])) {

    // check if purely numeric (if you want stricter behavior)
    int j = 0, valid = 1;
    while (buffer[j] != '\0') {
        if (!isdigit(buffer[j])) {
            valid = 0;
            break;
        }
        j++;
    }

    if (valid)
        printToken("Number", buffer);
    else
        printToken("Invalid Identifier", buffer);
}

else
    printToken("Invalid Identifier", buffer);

        }

       
        else if (isOperator(ch)) {
            char next = fgetc(fp);
            char temp[3];
            temp[0] = ch;
            temp[1] = '\0';

            
            if ((ch == '+' && next == '+') ||
                (ch == '-' && next == '-') ||
                (ch == '=' && next == '=') ||
                (ch == '!' && next == '=') ||
                (ch == '<' && next == '=') ||
                (ch == '>' && next == '=') ||
                (ch == '&' && next == '&') ||
                (ch == '|' && next == '|')) {
                temp[1] = next;
                temp[2] = '\0';
            } else {
                ungetc(next, fp);
            }
            printToken("Operator", temp);
        }

       
        else if (isDelimiter(ch)) {
            char temp[2] = {ch, '\0'};
            printToken("Delimiter", temp);
        }

       
        else if (isspace(ch)) {
            continue;
        }

        
        else {
            char temp[2] = {ch, '\0'};
            printToken("Invalid", temp);
        }
    }

    fclose(fp);
    return 0;
}
