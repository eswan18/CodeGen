#include "token.h"
#include "parser.tab.h"

const char* token_string(int t) {
	switch(t) {
		//case TOKEN_EOF:
		//	return "EOF";
		case TOKEN_PRINT:
			return "PRINT";
		case TOKEN_FUNCTION:
			return "FUNCTION";
		//case TOKEN_WHILE:
		//	return "WHILE";
		case TOKEN_FOR:
			return "FOR";
		case TOKEN_IF:
			return "IF";
		case TOKEN_ELSE:
			return "ELSE";
		case TOKEN_RETURN:
			return "RETURN";
		case TOKEN_LEFT_BRACE:
			return "LEFT_BRACE";
		case TOKEN_RIGHT_BRACE:
			return "RIGHT_BRACE";
		case TOKEN_LEFT_PAREN:
			return "LEFT_PAREN";
		case TOKEN_RIGHT_PAREN:
			return "RIGHT_PAREN";
		case TOKEN_LEFT_BRACKET:
			return "LEFT_BRACKET";
		case TOKEN_RIGHT_BRACKET:
			return "RIGHT_BRACKET";
		case TOKEN_STRING:
			return "STRING";
		case TOKEN_INTEGER:
			return "INTEGER";
		case TOKEN_CHAR:
			return "CHARACTER";
		case TOKEN_VOID:
			return "VOID";
		case TOKEN_BOOLEAN:
			return "BOOLEAN";
		case TOKEN_ARRAY:
			return "ARRAY";
		case TOKEN_GE:
			return "GE";
		case TOKEN_LE:
			return "LE";
		case TOKEN_EQ:
			return "EQ";
		case TOKEN_NE:
			return "NE";
		case TOKEN_LT:
			return "LT";
		case TOKEN_GT:
			return "GT";
		case TOKEN_AND:
			return "AND";
		case TOKEN_OR:
			return "OR";
		case TOKEN_NOT:
			return "NOT";
		case TOKEN_INCREMENT:
			return "INCREMENT";
		case TOKEN_DECREMENT:
			return "DECREMENT";
		case TOKEN_ADD:
			return "ADD";
		case TOKEN_SUBTRACT:
			return "SUBTRACT";
		case TOKEN_MULTIPLY:
			return "MULTIPLY";
		case TOKEN_DIVIDE:
			return "DIVIDE";
		case TOKEN_MODULUS:
			return "MODULUS";
		case TOKEN_EXPONENTIATE:
			return "EXPONENTIATE";
		case TOKEN_ASSIGN:
			return "ASSIGN";
		case TOKEN_COMMA:
			return "COMMA";
		case TOKEN_COLON:
			return "COLON";
		case TOKEN_SEMICOLON:
			return "SEMICOLON";
		case TOKEN_TRUE:
			return "TRUE";
		case TOKEN_FALSE:
			return "FALSE";
		case TOKEN_CHAR_LITERAL:
			return "CHARACTER_LITERAL";
		case TOKEN_STRING_LITERAL:
			return "STRING_LITERAL";
		case TOKEN_INTEGER_LITERAL:
			return "INTEGER_LITERAL";
		case TOKEN_IDENTIFIER:
			return "IDENTIFIER";
		//case TOKEN_COMMENT:
		//	return "COMMENT";
		default:
			fprintf(stderr,"scan error: invalid input character\n");
			exit(1);
	}
}
