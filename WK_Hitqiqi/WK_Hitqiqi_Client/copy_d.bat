@echo off

mkdir ..\..\bin\debug\xml
for /f %%i in ('"dir /b/on xml\*.*"') do (  
xcopy xml\%%i ..\..\bin\debug\xml /s /e /d /y 
)

mkdir ..\..\bin\debug\image
for /f %%i in ('"dir /b/on image\*.*"') do (  
xcopy image\%%i ..\..\bin\debug\image /s /e /d /y 
)

for /f %%i in ('"dir /b/on dll\*.*"') do (  
xcopy dll\%%i ..\..\bin\debug\ /s /e /d /y 
)

for /f %%i in ('"dir /b/on readme\*.*"') do (  
xcopy readme\%%i ..\..\bin\debug\ /s /e /d /y 
)
