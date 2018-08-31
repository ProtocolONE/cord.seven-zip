set buildTool=jom.exe
%buildTool% clean
%buildTool%

%buildTool% clean -f makefile.debug
%buildTool% -f makefile.debug
