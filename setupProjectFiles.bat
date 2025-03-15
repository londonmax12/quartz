mkdir build
cd build
cmake ../projects

if not exist "qz_shell\examples" mkdir qz_shell\examples
xcopy /E /I /Y "..\examples" "qz_shell\examples\"

if not exist "qz_interpreter\examples" mkdir qz_interpreter\examples
xcopy /E /I /Y "..\examples" "qz_interpreter\examples\"
pause