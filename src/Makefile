# part of the makefile
calculator: calculatorLexicalDefinitions.l calculatorBisonDefinitions.y symbolTable.c errorManager.c tableInterface.c
						bison -d calculatorBisonDefinitions.y
						flex calculatorLexicalDefinitions.l
						gcc -o $@ calculatorBisonDefinitions.tab.c symbolTable.c errorManager.c tableInterface.c -lfl -lm
