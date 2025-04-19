# t2r-dc-disasm
Tool for decompiling The Last Of Us: Part II PC scripts

# Tutorial
To decompile a script first find a script file that starts with `ss-\<name>.bin`
- The program will look for following folder: `sid1/sidbase.bin` in the same directory of the executable so **make sure its there otherwise there will only be a partial resolution of the stringIds**
- In this example we\'re going to use `ss-asoria-test-interactions.bin`
Open the terminal and run the following command:
```
.\t2r-dc-disasm.exe .\ss-asoria-test-interactions.bin > .\ss-asoria-test-interactions.txt
```

This will redirect the output of the program to the file ss-asoria-test-interactions.txt

# Notes
- Please let me know if a state name is not resolved.

# Special Thanks
- Chandler Threepwood for providing a list of all the states, animation and spawners names
- samsepi0l for providing a partial list of natives names