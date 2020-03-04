@echo off

rem --- Make Directories
if not exist generated mkdir generated
if not exist unmapped_drawings\generated mkdir unmapped_drawings\generated

rem --- Generate HTML
start /b /wait "" "xcopy" .\data .\generated\data\ /y /s /e /q
set files= 
for %%i in (*.rxw) do ( call set "files=%%files%% %%i" )
..\..\generator\build\generator.exe --main_title "Ryan Fleury" --author "Ryan Fleury" --canonical_url "https://ryanfleury.net" --twitter_handle "@ryanjfleury" --html --html_header header.html --html_footer footer.html %files%

rem --- Generate color-mapped textures
pushd unmapped_drawings
set files=
for %%i in (*.png) do ( call set "files=%%files%% %%i" )
start /b /wait "" "..\..\..\color_mapper\build\color_mapper.exe" --primary 1 1 1 --secondary 0.8 0.8 0.8 --highlight 1.0 0.6823529 0.2588235 %files%
popd