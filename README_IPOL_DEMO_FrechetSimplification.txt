
This is the source code and tools corresponding to the "A Near-Linear Time Guaranteed Algorithm for Digital Curve Simplification Under the Fréchet Distance" version 1.1 IPOL article ( http://dx.doi.org/10.5201/ipol.2014.70 )

Author: Isabelle Sivignon

Copyright (c) 2014 by Isabelle Sivignon
License: GNU LESSER GENERAL PUBLIC LICENSE (the "LGPL").

Version 1.3 26/05/2024:


The program by default doesn't need to import external libraries (the static boost lib is already in the archive)

--------------------------
Compilation instructions:
--------------------------
To compile the demonstration code given in (demoIPOL_FrechetSimplification), you just have to do:

cd FrechetAndConnectedCompDemo
mkdir build
cd build
cmake ..
make 
You can install the files if you want:
sudo make install

Then the binary file "FrechetSimplification" will be available in the "FrechetAndConnectedCompDemo/build/demoIPOL_FrechetSimplification".

------------
Basic Usage:
------------
* From the "FrechetAndConnectedCompDemo/build/demoIPOL_FrechetSimplification" directory you can test:

./frechetSimplification -error 4 -sdp ../../demoIPOL_FrechetSimplification/Data/Plants/Plant019.sdp

It will produces the resulting polygon given as vertex set (output.txt) and displayed in output.eps


* If you want directly apply your algorithm from an grayscale image, you can do it by using the pgm2freeman executable given here:
 ../demoIPOL_ExtrConnectedReg/pgm2freeman -min_size 100.0 -image inputNG.pgm -outputSDPAll > inputContour.txt

All these contours can then be processed:
 ./frechetSimplification -error 4 -sdp inputContour.txt -allContours

The -allContours is important since the input format is supposed to be one polygon per line.


credits and acknowledgments:
Image from data are given from LEMS Vision Group at Brown University, under Professor Ben Kimia (http://www.lems.brown.edu/~dmc)/ 


Version 1.3 26/05/2024:
        - Fix an error related to element erase and iterator in the FrechetShortcut class.

Version 1.2 11/02/2021:
        - Fix various compilation errors from the DGtal Library (./src/DGtal; ./examples;
          mainly in ./tests (more details https://github.com/kerautret/DGtal0.6-ForIPOL/pull/21/files))
        - Update the included boost library to last version 1.75
        - Add C++11 as requirements in cmake configuration file.
        - Addition of a new small script (convertFig.sh).sh in order to call directly gs instead using the "convert"
          program that is not yet updated to take into account the security issue of gs that is fixed.
        - Add an option to change to resulting image size (used in the online demonstration)

Version 1.1 28/02/2014
