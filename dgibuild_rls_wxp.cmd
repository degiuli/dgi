@echo off
echo.
echo.----------------------------------------------------------------------
echo.### Building dgitracer library ###
echo.----------------------------------------------------------------------
echo.
cd dgitracer
build /gc sources
mkdir i386
copy .\objfre_wxp_x86\i386\dgitracer.lib .\i386\dgitracer.lib
cd..

echo.
echo.----------------------------------------------------------------------
echo.### Building dgiminidump library ###
echo.----------------------------------------------------------------------
echo.
cd dgiminidump
build /gc sources
mkdir i386
copy .\objfre_wxp_x86\i386\dgiminidump.lib .\i386\dgiminidump.lib
cd..

echo.
echo.----------------------------------------------------------------------
echo.### Building dgicrash app ###
echo.----------------------------------------------------------------------
echo.
cd dgicrash
build /gc sources
cd..

echo.
echo.----------------------------------------------------------------------
echo.### Building dgidebug app ###
echo.----------------------------------------------------------------------
echo.
cd dgidebug
build /gc sources
cd..

echo.
echo.----------------------------------------------------------------------
echo.### Building dgisysst app ###
echo.----------------------------------------------------------------------
echo.
cd dgisysst
build /gc sources
cd..

echo.
echo.----------------------------------------------------------------------
echo.### Building dgidevcon app ###
echo.----------------------------------------------------------------------
echo.
cd dgidevcon
build /gc sources
cd..
