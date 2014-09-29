##########################################
####
# Makefile for project 4
# Modified 2/2011 Raphael Finkel
# Modified 4/2014 Ashkan Hosseini
#
# All classes are compiled into the ./classes directory
#
# The default is to make all .class files.
#
#    clean    -- removes all generated files
#
#    test     -- Tests proj4 by running P4.class with inputs test[123].lite
#
###

DESTPATH = ./classes
COURSEDIR = /homes/raphael/courses/cs541/public
CUPPATH = $(COURSEDIR)/cup
CLASSPATH = $(DESTPATH):$(CUPPATH)/java-cup-10k.jar
VPATH = $(DESTPATH)
JCFLAGS = -Xlint:all -deprecation -classpath $(CLASSPATH) -d $(DESTPATH)
JFLAGS = -classpath $(CLASSPATH)
TESTDIR = $(COURSEDIR)/proj4/tests

%.class: %.java
	javac $(JCFLAGS) $<

.PHONY: all 
all: P4.class Yylex.class parser.class 

# don't use CUP 0.11a; I couldn't get it to work right.  -- Raphael 2/2011
sym.java parser.java: csx.cup 
	java $(JFLAGS) java_cup/Main < csx.cup
Yylex.java: csx.jlex
	jflex csx.jlex
parser.class: ASTNode.class Scanner.class SyntaxErrorException.class
Yylex.class: sym.class 
SymbolTable.class: Symb.class EmptySTException.class DuplicateException.class
Scanner.class: Yylex.class Types.class
SymbolInfo.class: Kinds.class Types.class
SymbolInfoMethod.class: SymbolInfo.class
ASTNode.class: ast.java sym.class SymbolTable.class SymbolInfo.class SymbolInfoMethod.class
	javac $(JCFLAGS) ast.java
P4.class: Yylex.class Scanner.class parser.class ASTNode.class

test: all
	java $(JFLAGS) P4 test1.csx
	java $(JFLAGS) P4 test2.csx
	java $(JFLAGS) P4 test3.csx

###
# style check
###

HOW = basic,braces,clone,design,finalizers,migrating,unusedcode,imports,optimizations,strictexception,strings,sunsecure,typeresolution

style: 
	t=`pwd`; cd ~raphael/source/pmd-4.2.5/bin; \
		./pmd.sh $$t text $(HOW)

###
# lint check
###

lint: all
	jlint +all classes

###
# clean
###

clean: 
	rm -f $(DESTPATH)/*.class Yylex.java* sym.java parser.java
