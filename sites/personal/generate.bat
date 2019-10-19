@echo off
if not exist generated mkdir generated
start /b /wait "" "xcopy" .\data .\generated\data\ /y /s /e /q
set files= 
for %%i in (*.rxw) do ( call set "files=%%files%% %%i" )
..\..\generator\build\generator.exe --main_title "Ryan Fleury" --author "Ryan Fleury" --canonical_url "https://ryanfleury.net" --twitter_handle "@ryanjfleury" --html --html_header header.html --html_footer footer.html %files%