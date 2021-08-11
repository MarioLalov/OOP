
Folders:
Image, PBM, PGM, PPM, Edior, Commands - hold header and cpp files
Tests.zip - unit tests
docs.zip - documentation

Compile commands for GCC:

Image tests:
g++ Image/image.cpp Tests/image_test.cpp

PBM tests:
g++ Image/image.cpp PBM/pbm.cpp PGM/pgm.cpp PPM/ppm.cpp Tests/pbm_test.cpp Editor/image_editor.cpp

PGM tests:
g++ Image/image.cpp PBM/pbm.cpp PGM/pgm.cpp PPM/ppm.cpp Tests/pgm_test.cpp Editor/image_editor.cpp

PPM tests:
g++ Image/image.cpp PBM/pbm.cpp PGM/pgm.cpp PPM/ppm.cpp Tests/ppm_test.cpp Editor/image_editor.cpp

Full compilation:
g++ Image/image.cpp PBM/pbm.cpp PGM/pgm.cpp PPM/ppm.cpp Editor/image_editor.cpp main.cpp Commands/commands.cpp
