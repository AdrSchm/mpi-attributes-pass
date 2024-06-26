# Small Python program to count amount of instructions using memory
# accesses in x86 and x86-64 assembly. Uses Intel syntax as default.

# Takes any number of filenames as commandline arguments
# Optional argument -v to show the found instructions
# Optional argument -a to use files using AT&T syntax

import re
import sys

# Returns the index of the character beginning the comment
# Returns -1 if no comment exists in this line
def findCommentStart(line):
    # Extend this list to use other characters as well
    commentChars = ['#', ';']
    commentStarts = -1
    # Find positions of the characters in the current line
    commentCharPos = [line.find(commentChar) for commentChar in commentChars]
    # Find the smallest position, ignore -1 from non-existent characters
    for pos in commentCharPos:
        if (commentStarts == -1) or (pos != -1 and pos < commentStarts):
            commentStarts = pos
    return commentStarts

if __name__ == "__main__":
    # Test whether filenames have been supplied
    if len(sys.argv) == 1:
        print("Missing arguments")
        sys.exit(1)

    # Parse arguments
    args = sys.argv[1:]
    filenames = []
    verbose = False
    attSyntax = False
    for arg in args:
        if arg.lower() == "-v":
            verbose = True
        elif arg.lower() == "-a":
            attSyntax = True
        elif arg[0] == "-":
            print("Unknown argument: " + arg)
            sys.exit(2)
        else:
            filenames.append(arg)

    # Matches instructions with a pair of brackets
    matcher = re.compile(r"\[.*\]")
    closingChar = ']'
    if attSyntax:
        matcher = re.compile(r"\(.*\)")  
        closingChar = ')'

    # Iterate through list of filenames
    if len(filenames) == 0:
        print("No filename supplied")
        sys.exit(3)
    for filename in filenames:
        file = open(filename, "r")
        memInsts = []

        # Iterate through the lines of the file
        for line in file:
            line = line.strip()
            splitLine = re.split(r"\s", line)
            # Check whether it's from (llvm-)objdump and remove leading binary encoding
            # Also removes lables, but no problem with that
            if len(splitLine) == 0 or len(splitLine[0]) == 0:
                continue
            if splitLine[0][-1] == ':':
                # Remove address and binary encoding
                while len(splitLine) > 0 and splitLine[0] != '':
                    del splitLine[0]
                # Remove separating whitespace
                while len(splitLine) > 0 and splitLine[0] == '':
                    del splitLine[0]
                # Sometimes added before nops
                while len(splitLine) > 0 and splitLine[0] == "data16":
                    del splitLine[0]
            if len(splitLine) == 0:
                continue

            # From here on splitLine has at least one element
            # Cases where we have push or pop
            if splitLine[0][0:4] == "push":
                commentStarts = findCommentStart(line)
                pushPos = line.find("push")
                # Only count it if is not in a comment
                if commentStarts == -1 or commentStarts > pushPos:
                    memInsts.append(line)
                continue
            
            if splitLine[0][0:3] == "pop":
                commentStarts = findCommentStart(line)
                popPos = line.find("pop")
                # Only count it if is not in a comment
                if commentStarts == -1 or commentStarts > popPos:
                    memInsts.append(line)
                continue

            # Find pairs of brackets
            matches = matcher.findall(line)
            if len(matches) == 0:
                continue

            # Ignore lines where this is part of a string literal or directive
            if splitLine[0][0] == '.':
                continue

            # Ignore nops
            if splitLine[0][0:3] == "nop":
                continue

            # Only instruction where brackets don't mean memory access
            if splitLine[0][0:3] == "lea":
                continue
            
            # Ignore lines where this is part of a comment
            bracketClosed = line.find(closingChar)
            commentStarts = findCommentStart(line)
            if commentStarts != -1 and commentStarts < bracketClosed:
                continue

            # Once we are here we add it to our list of instructions
            memInsts.append(line)

        # Output
        print("File: " + filename)
        print("Amount of memory instructions: " + str(len(memInsts)))
        if verbose:
            print("Instructions: ")
            for memInst in memInsts:
                print(memInst)
        print()
        file.close()
