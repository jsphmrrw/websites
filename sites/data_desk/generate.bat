@echo off

rem --- Make Directories
if not exist generated mkdir generated

rem --- Generate HTML
start /b "" "xcopy" .\data .\generated\data\ /y /s /e /q
start /b /wait "" "xcopy" search.js generated\ /y
set files=
for %%i in (*.rxw) do ( call set "files=%%files%% %%i" )
..\..\generator\build\generator.exe --main_title "Data Desk" --author "Ryan Fleury" --canonical_url "https://datadesk.net" --twitter_handle "@ryanjfleury" --html --html_header header.html --html_footer footer.html %files% custom_layer_api.html