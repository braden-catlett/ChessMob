The following guidelines are to be followed as much as possible when writing code for the ChessMob project. Please feel free to comment on these guidelines if you feel your way of doing something is better.

--------------------------------------
## White Space ##

### Indentation ###
Code should be indented using the tab character in all files with a tab size of 4. In Visual Studio, this can be made the default through the following process: Tools > Options > Text Editor > [language to change or "All Languages" > Tabs > select "Keep tabs" and set "Tab size" to 4.

When using multiple lines of code for a single statement, the first line is to have the regular indentation for that block of code while every subsequent line should receive one additional indent. When instantiating a multi-dimensional collection, have each "row" be on its own line. Also, when using multiple lines for function arguments, each line of arguments should be indented with the closing parenthesis and semi-colon being on their own line and aligned with the first line of the statement.

```
#!C++
// like this
int someInts[2][2] = {
	{ 1, 2 },
	{ 3, 4 }
};

DoSomething(var1, var2, var3,
	var4, var5, var6
);
```

Blocks of code should be indented one more time than the exterior code. The one exception to this is that case statements within a switch statement should be aligned with the exterior code.

```
#!C++
// like this
switch (someInt)
{
case 0:
	DoSomething(0);
	break;
default:
	DoSomethingElse();
}
```

### New Lines ###
For new line characters, use the default setting for the appropriate tool. For the Android code use Eclipse's default, Visual Studio's default for C++, and XCode's default for iOS code.

### Empty Lines ###
Empty lines should be used liberally to aid readability. There should be two blank lines between functions and one blank line between smaller blocks of code.

```
#!C++
// do this
Pawn p1 = Pawn();
p1.GeneratePossibleMoves();

Pawn p2 = Pawn();
p2.GeneratePossibleMoves();

// don't do this
Pawn p1 = Pawn();
p1.GeneratePossibleMoves();
Pawn p2 = Pawn();
p2.GeneratePossibleMoves();
```

### Code Blocks ###
Anything with a following code block (loop, class, etc.) should be on its own line with the code block following on the lines below. This applies even if the code block is only one line long.

```
#!C++
// do this
if (someInt == 0)
	DoSomething();

// don't do this
if (someInt == 0) DoSomething();
```

### Spaces ###
Spaces should precede all opening parentheses with the exception of function calls and definitions. Closing parentheses should have neither a preceding nor a following space. 

Spaces should follow but not precede commas in any collection instantiation. The same applies to semi-colons in for loop statments.

All arithmetic and logic operators should be preceded and followed by a space. The one exception being the increment and decrement operators which should not have any surrounding spacing.

```
#!C++
// like this
void DoSomething(int someInt)
{
	int moreInts[3] = { 1, 2, 3 };
	
	for (int count = 0; count < someInt; count++)
	{
		Something();
	}
}
```

--------------------------------------
## Curly brackets ##
As a general rule, all curly brackets should be on their own line with nothing else except white space. Also, anything inside curly brackets should be indented one level more than the curly brackets themselves.

The only exception to this rule is when instantiating a collection, the first curly bracket can be on the same line as the collection declaration. In this case, the first bracket should be proceeded and followed by a single space. Also, unless the instantiation only takes one line, the closing bracket should be aligned with the first line of the code statment on its own line with the semi-colon. In the case the instantiation only takes one line, put a space before the closing bracket also.

```
#!C++
// like this
Pawns pawns1[8] = { Pawn(), Pawn(), Pawn(),
	Pawn(), Pawn(), Pawn(), Pawn(), Pawn()
};

// this is also okay
Pawns pawns2[3] = { Pawn(), Pawn(), Pawn() };
```

Also, curly brackets should be used to surround all code blocks except for one-line if statements. Should an if statement be followed by an else statement, both of their code blocks should have curly brackets.

--------------------------------------
## Naming Conventions ##
As a general rule, name things as descriptively as possible without making the name overly long. This includes for loop iterator names.

Regarding capitalization, Classes and structs and their member variables and functions should be named using PascalCase. Local variables should receive names using camelCase.

```
#!C++
// like this
class Piece
{
	vector<Move> PossibleMoves;
	
	void DoSomething()
	{
		int someInt = 0;
		
		for (int moveNum = 0; moveNum < PossibleMoves.size(); moveNum++)
		{
			Something();
		}
	}
};
```

If you use Hungarian Notation or something similar to it, please know that you will be hunted down and, after several years of gruesome torture, killed.

--------------------------------------
## Comments ##
The preferred comment behavior is to simply write readable code. However, comments should be used if there is any doubt about the readability of a section of code. Also, all classes and structs and their member variables and functions should have a brief summary comment on the line prior to their definition.

Single line comments should reside on their own line and have a space after the comment character(s). The start and end of multi-line comments should be on their own lines with the comments indented on the lines in between. Also note that it is preferrable to precede comments with an empty line.

```
#!C++
// single line comment
DoSomething();

/*
	This is a multiple line comment.
	Notice the indentation.
*/
```