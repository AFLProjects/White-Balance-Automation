#Common errors

<br></br>
##CRT_SECURE_NO_WARNINGS
Some times you can get 
Severity	Code	Description	Project	File	Line	Suppression State
Error	C4996	'fopen': This function or variable may be unsafe. Consider using fopen_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.	upng_vs	c:\users\cfluder\desktop\github\uvideo\master_repo\uvideo\src\upng_vs\upng_vs\upng_vs.cpp	11	

go to Projects -> Properties -> Configuration properties -> C/C++ -> Preprocessor -> Preprocessor -> add _CRT_SECURE_NO_WARNINGS
