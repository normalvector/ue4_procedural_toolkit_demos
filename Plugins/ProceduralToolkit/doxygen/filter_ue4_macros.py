#!env python3
import fileinput;
import re;

is_ue4_macro = re.compile('^\s*(UCLASS|UPROPERTY|UFUNCTION)')
for line in fileinput.input():

    if is_ue4_macro.match(line):
        print ("");
    else:
        print(line, end='')
