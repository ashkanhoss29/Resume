####
# Makefile for project 2
# Modified 1/2011 Raphael Finkel
#
#
# All classes are compiled into the ./classes directory
#
# The default is to make all .class files.
#
#    clean    -- removes all generated files
#
#    test     -- Tests proj2 by running P2.class with input tiny.csx
#
###

DESTPATH = ./classes
CLASSPATH = $(DESTPATH)
VPATH = $(DESTPATH)
JCFLAGS = -Xlint:all -deprecation -classpath $(CLASSPATH) -d $(DESTPATH)
JFLAGS = -classpath $(CLASSPATH)

%.class: %.java
	javac $(JCFLAGS) $<

all : P2.class sym.class Yylex.class 

P2.class : P2.java Yylex.class
sym.class: sym.java
Yylex.class: sym.class
Yylex.java: csx.jlex
	rm -f Yylex.java
	jflex csx.jlex

###
# test
###

test:   P2.class Yylex.class tiny.csx
	java $(JFLAGS) P2 tiny.csx

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
	rm -f $(DESTPATH)/*.class Yylex.java

