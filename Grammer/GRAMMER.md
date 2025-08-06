# ===== Start Rule & Top-Level Structure =====
file_input: (NEWLINE | statement)* ENDMARKER
statement: stmt_list NEWLINE | compound_stmt
stmt_list: simple_stmt (';' simple_stmt)* [';']

# ===== Simple Statements =====
simple_stmt: expression_stmt
           | assert_stmt
           | pass_stmt
           | del_stmt
           | return_stmt
           | yield_stmt
           | raise_stmt
           | break_stmt
           | continue_stmt
           | import_stmt
           | future_stmt
           | global_stmt
           | nonlocal_stmt
           | declaration_stmt

# ===== Declaration Rules =====

# Main rule for a declaration statement
declaration_stmt: type_specifier NAME ['=' test]

# Defines the available type specifiers
type_specifier: primitive_type | list_type

# Defines primitive data types
primitive_type: 'int8' | 'float' | 'double' | 'char' | 'bool'

# Defines a generic list type
list_type: 'list' '<' type_specifier '>'

# ===== Other Statements =====

# Expression statement (for assignment, calls, etc.)
expression_stmt: testlist_star_expr (augassign (yield_expr | testlist) |
                 ('=' (yield_expr | testlist_star_expr))*) |
                 postfix_expression

# Compound Statements
compound_stmt: if_stmt
             | while_stmt
             | for_stmt
             | try_stmt
             | with_stmt
             | funcdef
             | classdef
             | struct_stmt

# Block definition (Suite) - Using curly braces
suite: '{' NEWLINE? (statement)+ '}'

# Control flow statements (with mandatory parentheses)
if_stmt: 'if' '(' test ')' suite ('elif' '(' test ')' suite)* ['else' suite]
while_stmt: 'while' '(' test ')' suite ['else' suite]
for_stmt: 'for' '(' exprlist 'in' testlist ')' suite ['else' suite]
        | 'for' '(' [testlist] ';' [test] ';' [testlist] ')' suite

# Function and Class definitions
funcdef: 'def' NAME parameters ['->' test] suite
classdef: 'class' NAME ['(' [arglist] ')'] suite

# 'struct' statement definition
struct_stmt: 'struct' NAME '{' (struct_field)+ '}'
struct_field: NAME ':' test NEWLINE

# Increment/Decrement operator definition
postfix_expression: primary ('++' | '--')

# ===== Expressions =====
test: or_test ['if' or_test 'else' test] | lambdef
# ... (rest of expression rules, standard Python base)
power: primary ['**' factor]
primary: atom | postfix_expression
atom: ('(' [yield_expr|testlist_comp] ')' |
       '[' [testlist_comp] ']' |
       '{' [dictorsetmaker] '}' |
       NAME | NUMBER | STRING+ | '...' | 'None' | 'true' | 'false')